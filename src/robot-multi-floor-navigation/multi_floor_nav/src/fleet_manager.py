#!/usr/bin/env python3
"""
fleet_manager.py — Gestionnaire central de missions v1.0
═══════════════════════════════════════════════════════════
Architecture :
  Tkinter / MQTT  →  /fleet/mission_request  →  FleetManager
                                                      │
                              ┌───────────────────────┤
                              ▼                       ▼
                        Sélection robot          Mutex ascenseur
                              │                       │
                              ▼                       ▼
                        send_goal.py          FIFO elevator queue
                              │
                              ▼
                        Nav2 BT Navigator

Topics ROS2 publiés :
  /fleet/status          → état complet (robots + ascenseur) → Tkinter
  /robot1/robot_status   → état robot1
  /robot2/robot_status   → état robot2
  /robot1/current_floor  → étage robot1
  /robot2/current_floor  → étage robot2

Topics ROS2 souscrits :
  /fleet/mission_request → missions entrantes depuis Tkinter
  /robot1/amcl_pose      → position robot1
  /robot2/amcl_pose      → position robot2
  /robot1/odometry/filtered
  /robot2/odometry/filtered
  /world/default/model/elevator/joint_state → position cabine

MQTT publié :
  fleet/status           → dashboard Tkinter (complet JSON)
  fleet/robot1/status    → statut robot1
  fleet/robot2/status    → statut robot2
  fleet/elevator/status  → statut ascenseur

MQTT souscrit :
  fleet/mission/assign   → mission depuis UI externe
  fleet/elevator/request → demande ascenseur externe
"""

import json
import math
import os
import subprocess
import sys
import threading
import time
from collections import deque
from datetime import datetime
from enum import Enum, auto

import paho.mqtt.client as mqtt
import rclpy
from geometry_msgs.msg import PoseWithCovarianceStamped
from nav_msgs.msg import Odometry
from rclpy.node import Node
from rclpy.qos import QoSHistoryPolicy, QoSProfile, QoSReliabilityPolicy
from sensor_msgs.msg import JointState
from std_msgs.msg import String

# ═══════════════════════════════════════════════════════════════════════════════
#  Configuration
# ═══════════════════════════════════════════════════════════════════════════════

ROBOTS         = ["robot1", "robot2"]
BROKER         = "localhost"
MQTT_PORT      = 1883

# Chemin vers send_goal.py — adapter si nécessaire
SEND_GOAL_SCRIPT = "/home/hm/ros2_ws/src/jackal_elevator/scripts/send_goal.py"

# Ascenseur
LIFT_JOINT_TOPIC = "/world/default/model/elevator/joint_state"
LIFT_JOINT_NAMES = ["lift", "lift_joint", "elevator_lift", "elevator_joint"]
FLOOR1_THRESHOLD = 1.5          # m — seuil détection étage 1
FLOOR_HEIGHTS    = {"0": 0.0, "1": 3.09}

# Seuils sélection automatique du robot
SAME_FLOOR_BONUS   = 10.0       # bonus distance si déjà au bon étage
MAX_MISSION_WAIT   = 300.0      # secondes avant timeout mission (5 min)

# MQTT topics
MQTT_PUB_STATUS         = "fleet/status"
MQTT_PUB_ROBOT          = "fleet/{robot}/status"
MQTT_PUB_ELEVATOR       = "fleet/elevator/status"
MQTT_SUB_ASSIGN         = "fleet/mission/assign"
MQTT_SUB_ELEVATOR_REQ   = "fleet/elevator/request"

ROBOT_INITIAL_POSES = {
    "robot1": {"x": 0.0,  "y": 0.0,   "yaw": 0.0},
    "robot2": {"x": -0.3, "y": -7.06, "yaw": 0.0},
}

# ═══════════════════════════════════════════════════════════════════════════════
#  Énumérations
# ═══════════════════════════════════════════════════════════════════════════════

class RobotState(str, Enum):
    IDLE             = "idle"
    BUSY             = "busy"
    WAITING_ELEVATOR = "waiting_elevator"
    GOING_TO_ELEVATOR= "going_to_elevator"
    IN_ELEVATOR      = "in_elevator"
    CHARGING         = "charging"
    ERROR            = "error"
    RECOVERY         = "recovery"


class MissionStatus(str, Enum):
    QUEUED     = "queued"
    ASSIGNED   = "assigned"
    RUNNING    = "running"
    DONE       = "done"
    FAILED     = "failed"
    CANCELLED  = "cancelled"


# ═══════════════════════════════════════════════════════════════════════════════
#  Structures de données
# ═══════════════════════════════════════════════════════════════════════════════

class Mission:
    _counter = 0

    def __init__(self, robot_id: str, target_floor: int,
                 x: float, y: float, yaw: float,
                 priority: int = 5):
        Mission._counter += 1
        self.id           = Mission._counter
        self.robot_id     = robot_id        # "auto" ou "robot1"/"robot2"
        self.target_floor = target_floor
        self.x            = x
        self.y            = y
        self.yaw          = yaw
        self.priority     = priority        # 0=max, 9=min
        self.status       = MissionStatus.QUEUED
        self.assigned_to  = None
        self.created_at   = datetime.now()
        self.started_at   = None
        self.finished_at  = None
        self.process      = None            # subprocess send_goal

    def to_dict(self) -> dict:
        return {
            "id":           self.id,
            "robot_id":     self.robot_id,
            "assigned_to":  self.assigned_to,
            "target_floor": self.target_floor,
            "x":            round(self.x, 3),
            "y":            round(self.y, 3),
            "yaw":          round(self.yaw, 4),
            "priority":     self.priority,
            "status":       self.status.value,
        }


class RobotInfo:
    def __init__(self, robot_id: str):
        self.id           = robot_id
        self.state        = RobotState.IDLE
        self.floor        = 0
        self.floor_updated_at = None
        self.x            = 0.0
        self.y            = 0.0
        self.yaw          = 0.0
        self.vx           = 0.0           # vitesse linéaire X (m/s)
        self.vy           = 0.0           # vitesse linéaire Y (m/s)
        self.vyaw         = 0.0           # vitesse angulaire (rad/s)
        self.moving       = False         # True si |v| > seuil
        self.pose_src     = "---"
        self.pose_time    = None
        self.odom_time    = None          # timestamp dernière odom
        self.current_mission: Mission | None = None
        self.battery      = 100
        self.lock         = threading.Lock()

    def distance_to(self, x: float, y: float) -> float:
        return math.sqrt((self.x - x)**2 + (self.y - y)**2)

    def is_available(self) -> bool:
        return self.state == RobotState.IDLE

    def to_dict(self) -> dict:
        with self.lock:
            return {
                "id":       self.id,
                "state":    self.state.value,
                "floor":    self.floor,
                "x":        round(self.x,    3),
                "y":        round(self.y,    3),
                "yaw":      round(self.yaw,  4),
                "vx":       round(self.vx,   3),
                "vy":       round(self.vy,   3),
                "vyaw":     round(self.vyaw,  3),
                "moving":   self.moving,
                "battery":  self.battery,
                "mission":  self.current_mission.id if self.current_mission else None,
                "pose_src": self.pose_src,
            }


# ═══════════════════════════════════════════════════════════════════════════════
#  FleetManager — nœud ROS2 principal
# ═══════════════════════════════════════════════════════════════════════════════

class FleetManager(Node):
    """
    Gestionnaire central :
      1. Reçoit missions depuis /fleet/mission_request
      2. Sélectionne le meilleur robot disponible
      3. Gère la file FIFO de l'ascenseur (mutex)
      4. Lance send_goal.py en sous-processus
      5. Publie l'état complet sur /fleet/status et MQTT
    """

    def __init__(self):
        super().__init__("fleet_manager")
        self.get_logger().info("═══ FleetManager démarrage ═══")

        # ── État des robots ───────────────────────────────────────────────────
        self.robots: dict[str, RobotInfo] = {r: RobotInfo(r) for r in ROBOTS}

        # ── File de missions (triée par priorité) ─────────────────────────────
        self._mission_queue: deque[Mission] = deque()
        self._mission_history: list[Mission] = []
        self._queue_lock = threading.Lock()

        # ── Mutex ascenseur ───────────────────────────────────────────────────
        self._elevator_lock   = threading.Lock()
        self._elevator_queue: deque[dict] = deque()   # {robot_id, floor, event}
        self._elevator_busy         = False
        self._elevator_floor        = 0
        self._elevator_pos          = 0.0
        self._elevator_moving       = False
        self._elevator_last_update  = 0.0
        self._elevator_floor_logged = -1
        self._elevator_mqtt_queue   = []   # file vue depuis MQTT
        self._elev_lock             = threading.Lock()

        # ── QoS ───────────────────────────────────────────────────────────────
        best_effort_qos = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=10
        )

        # ── Subscribers ROS2 ─────────────────────────────────────────────────
        for robot in ROBOTS:
            self.create_subscription(
                PoseWithCovarianceStamped,
                f"/{robot}/amcl_pose",
                lambda msg, r=robot: self._cb_amcl(msg, r),
                10
            )
            # FIX 2 : souscrire aux DEUX topics odom (filtered + raw)
            # Le premier reçu gagne — évite le (0,0) si filtered absent
            self.create_subscription(
                Odometry,
                f"/{robot}/odometry/filtered",
                lambda msg, r=robot: self._cb_odom(msg, r),
                best_effort_qos
            )
            self.create_subscription(
                Odometry,
                f"/{robot}/odom",                     # fallback raw odom
                lambda msg, r=robot: self._cb_odom(msg, r),
                best_effort_qos
            )
            self.create_subscription(
                String,
                f"/{robot}/robot_status",
                lambda msg, r=robot: self._cb_robot_status_external(msg, r),
                10
            )

        # Mission request depuis Tkinter (v3) ou directement
        self.create_subscription(
            String, "/fleet/mission_request",
            self._cb_mission_request, 10
        )

        # Lift
        self.create_subscription(
            JointState, LIFT_JOINT_TOPIC,
            self._cb_lift, best_effort_qos
        )

        # ── Publishers ROS2 ───────────────────────────────────────────────────
        self._fleet_status_pub = self.create_publisher(String, "/fleet/status", 10)

        for robot in ROBOTS:
            setattr(self, f"_pub_status_{robot}",
                    self.create_publisher(String, f"/{robot}/robot_status", 10))
            setattr(self, f"_pub_floor_{robot}",
                    self.create_publisher(String, f"/{robot}/current_floor", 10))

        # ── MQTT ─────────────────────────────────────────────────────────────
        self._mqtt = mqtt.Client(client_id="fleet_manager")
        self._mqtt.on_connect    = self._mqtt_on_connect
        self._mqtt.on_disconnect = self._mqtt_on_disconnect
        self._mqtt.on_message    = self._mqtt_on_message
        self._mqtt_connected     = False
        threading.Thread(target=self._mqtt_connect_loop, daemon=True).start()

        # ── Timers ───────────────────────────────────────────────────────────
        self.create_timer(0.5,  self._timer_publish_status)   # 2 Hz
        self.create_timer(1.0,  self._timer_process_queue)    # 1 Hz
        self.create_timer(5.0,  self._timer_watchdog)         # watchdog missions

        # ── Worker thread ascenseur ───────────────────────────────────────────
        self._elev_worker_thread = threading.Thread(
            target=self._elevator_worker, daemon=True)
        self._elev_worker_thread.start()

        self.get_logger().info(f"✅ FleetManager prêt — send_goal: {SEND_GOAL_SCRIPT}")
        self.get_logger().info(f"📡 Topics souscrits : /fleet/mission_request, /{'{robot}'}/amcl_pose, ...")
        self.get_logger().info(f"📤 Topics publiés   : /fleet/status, /{'{robot}'}/robot_status, ...")

    # ══════════════════════════════════════════════════════════════════════════
    #  Callbacks position
    # ══════════════════════════════════════════════════════════════════════════

    def _cb_amcl(self, msg: PoseWithCovarianceStamped, robot: str):
        info = self.robots[robot]
        with info.lock:
            info.x        = msg.pose.pose.position.x
            info.y        = msg.pose.pose.position.y
            info.yaw      = self._quat_to_yaw(msg.pose.pose.orientation)
            info.pose_src = "AMCL"
            info.pose_time = time.time()

    def _cb_odom(self, msg: Odometry, robot: str):
        info = self.robots[robot]
        vx   = msg.twist.twist.linear.x
        vy   = msg.twist.twist.linear.y
        vyaw = msg.twist.twist.angular.z
        speed = math.sqrt(vx * vx + vy * vy)
        moving = speed > 0.01 or abs(vyaw) > 0.01

        with info.lock:
            info.vx     = vx
            info.vy     = vy
            info.vyaw   = vyaw
            info.moving = moving
            info.odom_time = time.time()

            if info.pose_src == "AMCL" and info.pose_time:
                if time.time() - info.pose_time < 10.0:
                    return

            # Odom + offset initial
            init = ROBOT_INITIAL_POSES.get(robot, {"x": 0.0, "y": 0.0, "yaw": 0.0})
            info.x        = msg.pose.pose.position.x + init["x"]   # ← ajout offset
            info.y        = msg.pose.pose.position.y + init["y"]   # ← ajout offset
            info.yaw      = self._quat_to_yaw(msg.pose.pose.orientation)
            info.pose_src = "Odom"
            info.pose_time = time.time()

    def _cb_lift(self, msg: JointState):
        pos = None
        # Chercher spécifiquement le joint "lift"
        for i, name in enumerate(msg.name):
            if name == "lift":
                pos = msg.position[i] if i < len(msg.position) else None
                break
        # Fallback premier joint
        if pos is None and msg.position:
            pos = msg.position[0]
        if pos is None:
            return

        with self._elev_lock:
            old = self._elevator_pos
            self._elevator_pos = pos
            self._elevator_floor = 1 if pos > FLOOR1_THRESHOLD else 0
            self._elevator_moving = abs(pos - old) > 0.002
            self._elevator_last_update = time.time()

        new_floor = 1 if pos > FLOOR1_THRESHOLD else 0
        if new_floor != self._elevator_floor_logged:
            self._elevator_floor_logged = new_floor
            self.get_logger().info(
                f"🛗 Ascenseur → étage {new_floor} (pos={pos:.3f}m)")

    def _cb_robot_status_external(self, msg: String, robot: str):
        """
        Reçoit les mises à jour depuis le BT du robot.
        NE déclenche PAS _on_mission_complete :
          → c'est send_goal.py qui est la source de vérité sur la fin de mission.
          → évite la race condition BT-idle vs send_goal en cours.
        Seul usage : mettre à jour robot.floor (pour la détection ascenseur).
        """
        try:
            data = json.loads(msg.data)
            info = self.robots[robot]
            with info.lock:
                # Mettre à jour le floor uniquement — pas l'état
                if "floor" in data:
                    new_floor = int(data["floor"])
                    if new_floor != info.floor:
                        self.get_logger().info(
                            f"  [{robot}] Floor mis à jour: {info.floor} → {new_floor} "
                            f"(via /robot_status)")
                    info.floor = new_floor
                    info.floor_updated_at = time.time()
                # Logger l'état reçu sans l'appliquer (info only)
                state = data.get("state", "")
                if state:
                    self.get_logger().debug(
                        f"  [{robot}] BT state={state} floor={data.get('floor','?')} "
                        f"(FM gère l'état — ignoré)")
        except Exception as e:
            self.get_logger().debug(f"_cb_robot_status: {e}")

    # ══════════════════════════════════════════════════════════════════════════
    #  Réception et traitement des missions
    # ══════════════════════════════════════════════════════════════════════════

    def _cb_mission_request(self, msg: String):
        """Reçoit une mission depuis /fleet/mission_request (Tkinter v3)."""
        try:
            data = json.loads(msg.data)
        except json.JSONDecodeError as e:
            self.get_logger().error(f"mission_request JSON invalide: {e}")
            return

        self.get_logger().info(f"📥 Mission reçue: {data}")
        self._enqueue_mission_from_dict(data)

    def _enqueue_mission_from_dict(self, data: dict):
        robot_id     = data.get("robot_id", "auto")
        target_floor = int(data.get("target_floor", 0))
        x            = float(data.get("x", 0.0))
        y            = float(data.get("y", 0.0))
        priority     = int(data.get("priority", 5))

        # ── Normalisation du yaw ───────────────────────────────────────────
        # Tkinter v3 : envoie yaw en RADIANS  (math.radians() dans get_mission_params)
        # Tkinter v4 : envoie yaw en DEGRÉS   (pas de conversion)
        # send_goal.py attend des RADIANS
        # Heuristique : si |yaw| > 2π (~6.28) → c'est des degrés
        raw_yaw = float(data.get("yaw", 0.0))
        if abs(raw_yaw) > 2 * math.pi:
            yaw = math.radians(raw_yaw)
            self.get_logger().debug(
                f"  yaw converti degrés→radians : {raw_yaw}° → {yaw:.4f} rad")
        else:
            yaw = raw_yaw   # déjà en radians (v3) ou 0

        m = Mission(robot_id, target_floor, x, y, yaw, priority)

        with self._queue_lock:
            self._mission_queue.append(m)
            # Trier par priorité (0 = plus haute)
            sorted_q = sorted(self._mission_queue, key=lambda m: m.priority)
            self._mission_queue = deque(sorted_q)

        self.get_logger().info(
            f"📋 Mission #{m.id} ajoutée  robot={robot_id}  "
            f"floor={target_floor}  x={x:.2f} y={y:.2f}  prio={priority}  "
            f"[file: {len(self._mission_queue)}]"
        )

    # ══════════════════════════════════════════════════════════════════════════
    #  Timer : traitement de la file de missions
    # ══════════════════════════════════════════════════════════════════════════

    def _timer_process_queue(self):
        with self._queue_lock:
            if not self._mission_queue:
                return
            # Chercher la première mission qu'on peut assigner
            for i, mission in enumerate(self._mission_queue):
                robot = self._select_robot(mission)
                if robot:
                    # Retirer de la file
                    self._mission_queue.remove(mission)
                    break
            else:
                return   # aucun robot libre

        self._assign_mission(mission, robot)

    def _select_robot(self, mission: Mission) -> RobotInfo | None:
        """
        Sélection intelligente :
          1. Si robot_id explicite → utiliser ce robot s'il est libre
          2. Sinon → choisir parmi les robots libres :
             a. déjà au bon étage  (bonus distance)
             b. le plus proche de la destination
        """
        if mission.robot_id not in ("auto", None, ""):
            robot = self.robots.get(mission.robot_id)
            if robot and robot.is_available():
                return robot
            else:
                self.get_logger().info(
                    f"Mission #{mission.id} : robot {mission.robot_id} non disponible "
                    f"({robot.state.value if robot else 'inconnu'})")
                return None

        # Sélection automatique
        candidates = [r for r in self.robots.values() if r.is_available()]
        if not candidates:
            return None

        def score(r: RobotInfo) -> float:
            d = r.distance_to(mission.x, mission.y)
            bonus = SAME_FLOOR_BONUS if r.floor == mission.target_floor else 0.0
            return d - bonus   # plus bas = meilleur

        best = min(candidates, key=score)
        self.get_logger().info(
            f"🤖 Sélection auto : {best.id}  "
            f"(distance={best.distance_to(mission.x, mission.y):.2f}m  "
            f"floor={best.floor}→{mission.target_floor})"
        )
        return best

    # ══════════════════════════════════════════════════════════════════════════
    #  Assignation et exécution d'une mission
    # ══════════════════════════════════════════════════════════════════════════

    def _assign_mission(self, mission: Mission, robot: RobotInfo):
        with robot.lock:
            robot.state           = RobotState.BUSY
            robot.current_mission = mission

        mission.assigned_to = robot.id
        mission.status      = MissionStatus.ASSIGNED
        mission.started_at  = datetime.now()

        self.get_logger().info(
            f"▶ Mission #{mission.id} assignée à {robot.id}  "
            f"floor={mission.target_floor}  x={mission.x:.2f} y={mission.y:.2f}"
        )

        # Publier l'état robot
        self._publish_robot_status(robot, mission)

        # Cas 1 vs Cas 2 : déterminer si ascenseur nécessaire
        # FIX : robot.floor n'est fiable que si mis à jour récemment
        # (via /robot_status externe). Sans feedback, on considère que
        # le robot est au même étage que la destination sauf si floor_change
        # est explicitement demandé ET que le floor robot est connu et différent.
        with robot.lock:
            robot_floor_known = robot.floor_updated_at is not None
            floor_age = (time.time() - robot.floor_updated_at) if robot.floor_updated_at else 999
            robot_floor = robot.floor

        need_elevator = (
         robot_floor_known       # ← False si floor jamais reçu !
         and floor_age < 30.0
         and robot_floor != mission.target_floor
)

        if need_elevator:
            self.get_logger().info(
                f"  [{robot.id}] Changement d'étage {robot_floor}→{mission.target_floor} "
                f"(info âgée de {floor_age:.1f}s) → demande ascenseur")
            with robot.lock:
                robot.state = RobotState.GOING_TO_ELEVATOR
            threading.Thread(
                target=self._run_elevator_mission,
                args=(mission, robot),
                daemon=True
            ).start()
        else:
            if not robot_floor_known or floor_age >= 30.0:
                self.get_logger().info(
                    f"  [{robot.id}] Étage inconnu ou info ancienne ({floor_age:.0f}s) "
                    f"→ navigation directe vers floor={mission.target_floor}")
            else:
                self.get_logger().info(
                    f"  [{robot.id}] Même étage ({mission.target_floor}) → navigation directe")
            threading.Thread(
                target=self._run_direct_navigation,
                args=(mission, robot),
                daemon=True
            ).start()

    def _run_direct_navigation(self, mission: Mission, robot: RobotInfo):
        """Navigation simple sur le même étage."""
        mission.status = MissionStatus.RUNNING

        # FIX : publier BUSY immédiatement pour que le dashboard soit correct
        with robot.lock:
            robot.state = RobotState.BUSY
        self._publish_robot_status(robot, mission)

        self.get_logger().info(
            f"  [{robot.id}] Navigation directe → "
            f"x={mission.x:.2f} y={mission.y:.2f} yaw={mission.yaw:.3f}rad "
            f"floor={mission.target_floor}")

        ok = self._call_send_goal(
            namespace=robot.id,
            floor=mission.target_floor,
            x=mission.x, y=mission.y, yaw=mission.yaw
        )
        self._on_mission_complete(robot.id, success=ok, mission=mission)

    def _run_elevator_mission(self, mission: Mission, robot: RobotInfo):
        """
        Séquence complète avec ascenseur :
          1. Naviguer vers l'entrée de l'ascenseur
          2. Demander l'ascenseur (mutex FIFO)
          3. Attendre que l'ascenseur soit à notre étage
          4. Naviguer vers destination finale
        """
        mission.status = MissionStatus.RUNNING

        # ── Étape 1 : aller vers l'ascenseur ─────────────────────────────────
        # Coordonnées d'entrée de l'ascenseur selon l'étage actuel
        # ⚠ Adapter ces coordonnées à votre map Gazebo
        elevator_entry = {
            0: {"x": 0.0,  "y": 0.0,  "yaw": 0.0},
            1: {"x": 0.0,  "y": 0.0,  "yaw": 3.14},
        }
        entry = elevator_entry.get(robot.floor, {"x": 0.0, "y": 0.0, "yaw": 0.0})

        self.get_logger().info(
            f"  [{robot.id}] Étape 1 : Navigation vers ascenseur "
            f"x={entry['x']} y={entry['y']}")

        ok = self._call_send_goal(
            namespace=robot.id,
            floor=robot.floor,
            x=entry["x"], y=entry["y"], yaw=entry["yaw"]
        )
        if not ok:
            self.get_logger().error(f"  [{robot.id}] Échec navigation vers ascenseur")
            self._on_mission_complete(robot.id, success=False, mission=mission)
            return

        # ── Étape 2 : demander l'ascenseur (mutex FIFO) ───────────────────────
        with robot.lock:
            robot.state = RobotState.WAITING_ELEVATOR

        self.get_logger().info(
            f"  [{robot.id}] Étape 2 : Demande ascenseur → étage {mission.target_floor}")

        arrived_event = threading.Event()
        req = {
            "robot_id": robot.id,
            "floor":    str(mission.target_floor),
            "event":    arrived_event
        }

        with self._elev_lock:
            self._elevator_queue.append(req)
            self.get_logger().info(
                f"  [{robot.id}] File ascenseur : "
                f"{[r['robot_id'] for r in self._elevator_queue]}")

        # Attendre que l'ascenseur soit disponible (max 120s)
        got_elevator = arrived_event.wait(timeout=MAX_MISSION_WAIT)

        if not got_elevator:
            self.get_logger().error(f"  [{robot.id}] Timeout attente ascenseur")
            with robot.lock:
                robot.state = RobotState.ERROR
            self._on_mission_complete(robot.id, success=False, mission=mission)
            return

        # ── Étape 3 : dans l'ascenseur ────────────────────────────────────────
        with robot.lock:
            robot.state           = RobotState.IN_ELEVATOR
            robot.floor           = mission.target_floor
            robot.floor_updated_at = time.time()   # FIX : horodater le changement d'étage

        self.get_logger().info(
            f"  [{robot.id}] Étape 3 : Dans l'ascenseur → arrivé étage {mission.target_floor}")

        # Notifier le bridge MQTT que ce robot est sorti
        time.sleep(1.0)   # laisser le temps au robot de sortir
        self._mqtt_publish("fleet/elevator/exit_done", json.dumps({"robot_id": robot.id}))

        # ── Étape 4 : navigation finale ───────────────────────────────────────
        with robot.lock:
            robot.state = RobotState.BUSY

        self.get_logger().info(
            f"  [{robot.id}] Étape 4 : Navigation finale "
            f"x={mission.x:.2f} y={mission.y:.2f}")

        ok = self._call_send_goal(
            namespace=robot.id,
            floor=mission.target_floor,
            x=mission.x, y=mission.y, yaw=mission.yaw
        )
        self._on_mission_complete(robot.id, success=ok, mission=mission)

    # ══════════════════════════════════════════════════════════════════════════
    #  Mutex ascenseur — worker FIFO
    # ══════════════════════════════════════════════════════════════════════════

    def _elevator_worker(self):
        """
        Thread dédié au mutex ascenseur.
        Traite les demandes FIFO — un seul robot à la fois.
        """
        self.get_logger().info("🛗 Elevator worker démarré")
        while rclpy.ok():
            req = None
            with self._elev_lock:
                if self._elevator_queue:
                    req = self._elevator_queue[0]   # peek

            if req is None:
                time.sleep(0.1)
                continue

            # Traiter cette demande
            robot_id = req["robot_id"]
            floor    = req["floor"]

            self.get_logger().info(
                f"🛗 Traitement ascenseur : {robot_id} → étage {floor}")

            # Publier demande MQTT vers ElevatorMQTTBridge
            payload = json.dumps({"floor": floor, "robot_id": robot_id})
            self._mqtt_publish("elevator/request", payload)
            self.get_logger().info(f"🛗 elevator/request → RPi : {payload}")

            # Attendre que l'ascenseur soit arrivé à l'étage
            # (le ElevatorMQTTBridge publie /{robot}/elevator/ready)
            # On attend via un subscriber ROS2 ou timeout
            arrived = self._wait_elevator_ready(robot_id, floor, timeout=60.0)

            if arrived:
                self.get_logger().info(
                    f"🛗 Ascenseur prêt pour {robot_id} @ étage {floor}")
            else:
                self.get_logger().warn(
                    f"🛗 Timeout ascenseur pour {robot_id}")

            # Débloquer le robot
            req["event"].set()

            # Retirer de la file seulement maintenant
            with self._elev_lock:
                if self._elevator_queue and self._elevator_queue[0] is req:
                    self._elevator_queue.popleft()

            # Petit délai entre deux robots
            time.sleep(2.0)

    def _wait_elevator_ready(self, robot_id: str, floor: str,
                             timeout: float = 60.0) -> bool:
        """
        Attend que /{robot_id}/elevator/ready soit publié
        par l'ElevatorMQTTBridge.
        Crée un subscriber temporaire.
        """
        ready_event = threading.Event()

        def cb(msg: String):
            if msg.data == str(floor):
                ready_event.set()

        sub = self.create_subscription(
            String,
            f"/{robot_id}/elevator/ready",
            cb,
            10
        )

        result = ready_event.wait(timeout=timeout)
        self.destroy_subscription(sub)
        return result

    # ══════════════════════════════════════════════════════════════════════════
    #  Appel send_goal.py
    # ══════════════════════════════════════════════════════════════════════════

    def _call_send_goal(self, namespace: str, floor: int,
                        x: float, y: float, yaw: float,
                        timeout: float = 300.0) -> bool:
        """
        Lance send_goal.py en sous-processus avec l'environnement ROS2 complet.
        Retourne True si succès (stdout contient "accomplie").
        """
        if not os.path.exists(SEND_GOAL_SCRIPT):
            self.get_logger().error(
                f"send_goal.py introuvable : {SEND_GOAL_SCRIPT}")
            return False

        cmd = [
            sys.executable, SEND_GOAL_SCRIPT,
            "--namespace", namespace,
            str(floor),
            str(round(x,   4)),
            str(round(y,   4)),
            str(round(yaw, 6))
        ]

        # ── Construire l'environnement avec ROS2 sourcé ───────────────────────
        # C'est la cause principale du TIMEOUT quand send_goal marche
        # manuellement mais pas depuis le FM (env ROS2 manquant dans subprocess)
        ros_env = os.environ.copy()

        # Source automatique du workspace si pas déjà fait
        ws_setup = os.path.expanduser(
            "~/ros2_ws/install/setup.bash")
        ros2_setup = "/opt/ros/humble/setup.bash"   # adapter si Iron/Jazzy

        if "AMENT_PREFIX_PATH" not in ros_env:
            # ROS2 pas sourcé → sourcer via bash -c
            self.get_logger().warn(
                f"[{namespace}] AMENT_PREFIX_PATH absent — "
                f"lancement via bash avec source")
            bash_cmd = (
                f"source {ros2_setup} 2>/dev/null; "
                f"source {ws_setup} 2>/dev/null; "
                + " ".join(cmd)
            )
            cmd_final  = ["bash", "-c", bash_cmd]
            env_final  = ros_env
        else:
            # Env déjà sourcé → lancer directement
            cmd_final = cmd
            env_final = ros_env

        # ── Vérifier que l'action server existe ──────────────────────────────
        try:
            check = subprocess.run(
                ["ros2", "action", "list"],
                capture_output=True, text=True, timeout=5,
                env=env_final
            )
            action_name = f"/{namespace}/navigate_to_pose"
            if action_name not in check.stdout:
                available = [l for l in check.stdout.splitlines()
                             if "navigate" in l.lower()]
                self.get_logger().error(
                    f"❌ [{namespace}] Action '{action_name}' introuvable ! "
                    f"Disponibles: {available} "
                    f"→ Adapter action_name dans send_goal.py si besoin")
        except Exception:
            pass   # pas bloquant

        self.get_logger().info(
            f"🚀 [{namespace}] send_goal → floor={floor} "
            f"x={x:.3f} y={y:.3f} yaw={yaw:.4f}rad")

        try:
            proc = subprocess.Popen(
                cmd_final,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                env=env_final
            )
            # Stocker le process pour que watchdog puisse le tuer
            for r in self.robots.values():
                with r.lock:
                    if r.id == namespace and r.current_mission:
                        r.current_mission.process = proc
                        break

            try:
                stdout, stderr = proc.communicate(timeout=timeout)
            except subprocess.TimeoutExpired:
                proc.kill()
                proc.communicate()
                self.get_logger().error(
                    f"❌ [{namespace}] send_goal TIMEOUT après {timeout}s")
                return False

            # Log ligne par ligne
            for line in stdout.strip().splitlines():
                self.get_logger().info(f"  [send_goal/{namespace}] {line}")
            if stderr.strip():
                for line in stderr.strip().splitlines():
                    self.get_logger().warn(f"  [send_goal/{namespace}] ERR: {line}")

            # send_goal.py print "Mission accomplie !" si status==4
            success = (proc.returncode == 0) and ("accomplie" in stdout)
            if success:
                self.get_logger().info(f"✅ [{namespace}] Nav2 mission accomplie")
            else:
                self.get_logger().error(
                    f"❌ [{namespace}] Nav2 mission échouée (rc={proc.returncode})")
            return success

        except Exception as e:
            self.get_logger().error(f"❌ [{namespace}] send_goal exception: {e}")
            return False

    # ══════════════════════════════════════════════════════════════════════════
    #  Fin de mission
    # ══════════════════════════════════════════════════════════════════════════

    def _on_mission_complete(self, robot_id: str, success: bool,
                             mission: Mission | None = None):
        robot = self.robots.get(robot_id)
        if robot is None:
            return

        if mission is None:
            mission = robot.current_mission

        with robot.lock:
            robot.state           = RobotState.IDLE
            robot.current_mission = None

        if mission:
            mission.status      = MissionStatus.DONE if success else MissionStatus.FAILED
            mission.finished_at = datetime.now()
            self._mission_history.append(mission)

            duration = (mission.finished_at - mission.started_at).total_seconds() \
                       if mission.started_at else 0
            self.get_logger().info(
                f"{'✅' if success else '❌'} Mission #{mission.id} terminée "
                f"({robot_id})  statut={mission.status.value}  "
                f"durée={duration:.1f}s"
            )

        self._publish_robot_status(robot, mission)

    # ══════════════════════════════════════════════════════════════════════════
    #  Watchdog missions bloquées
    # ══════════════════════════════════════════════════════════════════════════

    def _timer_watchdog(self):
        try:
            now = datetime.now()
            for robot in self.robots.values():
                with robot.lock:
                    m      = robot.current_mission
                    moving = robot.moving
                    state  = robot.state
                    if m and m.started_at:
                        elapsed = (now - m.started_at).total_seconds()
 
                        # Timeout absolu — tuer dans tous les cas
                        if elapsed > MAX_MISSION_WAIT:
                            self.get_logger().warn(
                                f"⚠️  Watchdog ABSOLU : Mission #{m.id} "
                                f"({robot.id}, {elapsed:.0f}s) → kill")
                            robot.state           = RobotState.RECOVERY
                            robot.current_mission = None
                            m.status              = MissionStatus.FAILED
                            m.finished_at         = now
                            self._mission_history.append(m)
                            if m.process and m.process.poll() is None:
                                self.get_logger().warn(
                                    f"  kill send_goal pid={m.process.pid}")
                                m.process.kill()
                            continue
 
                        # ← Ne pas tuer si robot attend/utilise ascenseur
                        if state in (RobotState.WAITING_ELEVATOR,
                                     RobotState.IN_ELEVATOR,
                                     RobotState.GOING_TO_ELEVATOR):
                            if hasattr(m, '_immobile_since'):
                                m._immobile_since = None
                            continue
 
                        # Watchdog intermédiaire — tuer seulement si immobile
                        IMMOBILE_TIMEOUT = 60.0
                        if elapsed > 30.0 and not moving:
                            if not hasattr(m, '_immobile_since') or m._immobile_since is None:
                                m._immobile_since = now
                            else:
                                immobile_s = (now - m._immobile_since).total_seconds()
                                if immobile_s > IMMOBILE_TIMEOUT:
                                    self.get_logger().warn(
                                        f"⚠️  Watchdog IMMOBILE : Mission #{m.id} "
                                        f"({robot.id}) immobile depuis {immobile_s:.0f}s → kill")
                                    robot.state           = RobotState.RECOVERY
                                    robot.current_mission = None
                                    m.status              = MissionStatus.FAILED
                                    m.finished_at         = now
                                    self._mission_history.append(m)
                                    if m.process and m.process.poll() is None:
                                        m.process.kill()
                        else:
                            if hasattr(m, '_immobile_since'):
                                m._immobile_since = None
                            if elapsed > 30.0 and int(elapsed) % 30 == 0:
                                self.get_logger().info(
                                    f"  [{robot.id}] Mission #{m.id} en cours "
                                    f"{elapsed:.0f}s — robot moving ✓")
        except Exception as e:
            self.get_logger().error(f'[watchdog] Exception: {e}')

    # ══════════════════════════════════════════════════════════════════════════
    #  Publications ROS2 + MQTT
    # ══════════════════════════════════════════════════════════════════════════

    def _timer_publish_status(self):
        """Publie l'état complet à 2 Hz → Tkinter + MQTT.
        Aussi : remet RECOVERY → IDLE après 30s.
        """
        now = time.time()
        for robot in self.robots.values():
            with robot.lock:
                if robot.state == RobotState.RECOVERY:
                    # Remettre idle après 30s pour accepter de nouvelles missions
                    if not hasattr(robot, "_recovery_since"):
                        robot._recovery_since = now
                    elif now - robot._recovery_since > 30.0:
                        robot.state = RobotState.IDLE
                        robot._recovery_since = None
                        self.get_logger().info(
                            f"  [{robot.id}] RECOVERY → IDLE (30s écoulées)")
                else:
                    robot._recovery_since = None
        with self._elev_lock:
            elev_queue_snapshot = [
                {"robot_id": r["robot_id"], "floor": r["floor"]}
                for r in self._elevator_queue
            ]
            elev_state = {
                "floor":   self._elevator_floor,
                "pos":     round(self._elevator_pos, 4),
                "moving":  self._elevator_moving,
                "busy":    self._elevator_busy,
                "waiting": elev_queue_snapshot,
            }

        with self._queue_lock:
            queue_snapshot = [m.to_dict() for m in self._mission_queue]

        robots_state = {r: info.to_dict() for r, info in self.robots.items()}

        payload = {
            "robots":   robots_state,
            "elevator": elev_state,
            "queue":    queue_snapshot,
            "ts":       time.time()
        }

        # ROS2 → /fleet/status
        msg       = String()
        msg.data  = json.dumps(payload)
        self._fleet_status_pub.publish(msg)

        # MQTT → fleet/status
        self._mqtt_publish(MQTT_PUB_STATUS, msg.data)

        # MQTT → fleet/{robot}/status (granulaire)
        for robot_id, state in robots_state.items():
            self._mqtt_publish(
                MQTT_PUB_ROBOT.format(robot=robot_id),
                json.dumps(state)
            )

        # MQTT → fleet/elevator/status
        self._mqtt_publish(MQTT_PUB_ELEVATOR, json.dumps(elev_state))

        # ROS2 → /{robot}/robot_status et /{robot}/current_floor
        for robot_id, info in self.robots.items():
            d = info.to_dict()

            status_msg      = String()
            status_msg.data = json.dumps({
                "state":      d["state"],
                "mission_id": d["mission"],
            })
            pub = getattr(self, f"_pub_status_{robot_id}", None)
            if pub:
                pub.publish(status_msg)

            floor_msg      = String()
            floor_msg.data = json.dumps({"floor": d["floor"]})
            pub2 = getattr(self, f"_pub_floor_{robot_id}", None)
            if pub2:
                pub2.publish(floor_msg)

    def _publish_robot_status(self, robot: RobotInfo,
                               mission: Mission | None = None):
        """Publication immédiate (hors timer) pour réactivité."""
        pub = getattr(self, f"_pub_status_{robot.id}", None)
        if pub:
            msg      = String()
            msg.data = json.dumps({
                "state":      robot.state.value,
                "mission_id": mission.id if mission else None,
            })
            pub.publish(msg)

    # ══════════════════════════════════════════════════════════════════════════
    #  MQTT
    # ══════════════════════════════════════════════════════════════════════════

    def _mqtt_connect_loop(self):
        while True:
            try:
                self._mqtt.connect(BROKER, MQTT_PORT, 60)
                self._mqtt.loop_forever()
            except Exception as e:
                self.get_logger().warn(f"MQTT déconnecté: {e} — reconnexion dans 5s")
                time.sleep(5)

    def _mqtt_on_connect(self, client, userdata, flags, rc):
        self._mqtt_connected = True
        self.get_logger().info(f"✅ MQTT connecté (rc={rc})")
        client.subscribe(MQTT_SUB_ASSIGN)
        client.subscribe(MQTT_SUB_ELEVATOR_REQ)
        client.subscribe("fleet/mission/cancel")
        # FIX : écouter l'état complet depuis ElevatorMQTTBridge
        # elevator/state publie pos + floor + moving + queue toutes les 200ms
        client.subscribe("elevator/state")
        client.subscribe("elevator/status")
        self.get_logger().info("📡 MQTT subscriptions: elevator/state + elevator/status")

    def _mqtt_on_disconnect(self, client, userdata, rc):
        self._mqtt_connected = False
        self.get_logger().warn(f"⚠️  MQTT déconnecté (rc={rc})")

    def _mqtt_on_message(self, client, userdata, msg):
        topic   = msg.topic
        payload = msg.payload.decode()
        self.get_logger().debug(f"MQTT ← {topic}: {payload}")

        try:
            data = json.loads(payload)
        except json.JSONDecodeError:
            data = {}

        if topic == MQTT_SUB_ASSIGN:
            self._enqueue_mission_from_dict(data)

        elif topic in ("elevator/state", "elevator/status"):
            # FIX : mise à jour ascenseur depuis ElevatorMQTTBridge
            # Format elevator/state :
            #   {"elevator": {"lift_height": 1.5, "is_moving": true,
            #                 "current_floor": "1", "queue": [...]}}
            elev = data.get("elevator", data)   # compatibilité ancien format
            with self._elev_lock:
                if "lift_height" in elev:
                    self._elevator_pos    = float(elev["lift_height"])
                    self._elevator_floor  = 1 if self._elevator_pos > FLOOR1_THRESHOLD else 0
                if "is_moving" in elev:
                    self._elevator_moving = bool(elev["is_moving"])
                elif "moving" in elev:
                    self._elevator_moving = bool(elev["moving"])
                if "current_floor" in elev:
                    try:
                        self._elevator_floor = int(elev["current_floor"])
                    except (ValueError, TypeError):
                        pass
                self._elevator_last_update = time.time()
                # File d'attente depuis MQTT
                queue = elev.get("queue", [])
                if queue and not self._elevator_queue:
                    # Mettre à jour l'affichage seulement (pas les events)
                    self._elevator_mqtt_queue = queue
                elif not queue:
                    self._elevator_mqtt_queue = []

        elif topic == MQTT_SUB_ELEVATOR_REQ:
            floor    = str(data.get("floor", "0"))
            robot_id = data.get("robot_id", ROBOTS[0])
            arrived  = threading.Event()
            req = {"robot_id": robot_id, "floor": floor, "event": arrived}
            with self._elev_lock:
                self._elevator_queue.append(req)
            self.get_logger().info(
                f"🛗 Demande MQTT ascenseur : {robot_id} → étage {floor}")

        elif topic == "fleet/mission/cancel":
            mission_id = data.get("mission_id")
            robot_id   = data.get("robot_id")
            self.get_logger().info(
                f"🚫 Annulation demandée : mission={mission_id} robot={robot_id}")
            # Retirer de la file si pas encore assignée
            with self._queue_lock:
                for m in list(self._mission_queue):
                    if (mission_id and m.id == mission_id) or \
                       (robot_id and m.robot_id == robot_id):
                        self._mission_queue.remove(m)
                        m.status = MissionStatus.CANCELLED
                        self.get_logger().info(f"  Mission #{m.id} annulée")

    def _mqtt_publish(self, topic: str, payload: str):
        if self._mqtt_connected:
            try:
                self._mqtt.publish(topic, payload, retain=False)
            except Exception as e:
                self.get_logger().debug(f"MQTT publish err: {e}")

    # ══════════════════════════════════════════════════════════════════════════
    #  Utilitaires
    # ══════════════════════════════════════════════════════════════════════════

    @staticmethod
    def _quat_to_yaw(q) -> float:
        siny = 2.0 * (q.w * q.z + q.x * q.y)
        cosy = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
        return math.atan2(siny, cosy)

    def get_status_summary(self) -> str:
        lines = ["═══ FLEET STATUS ═══"]
        for r in self.robots.values():
            d = r.to_dict()
            moving_icon = "🟡" if d["moving"] else "⚫"
            speed = math.sqrt(d["vx"]**2 + d["vy"]**2)
            # Avertir si la pose n'a jamais été reçue
            pose_warn = " ⚠️ NO POSE" if (d["x"] == 0.0 and d["y"] == 0.0
                                           and d["pose_src"] == "---") else ""
            lines.append(
                f"  {r.id:8s} | {d['state']:20s} | F{d['floor']} "
                f"| {moving_icon} {speed:.2f}m/s "
                f"| ({d['x']:+.2f}, {d['y']:+.2f}) yaw={math.degrees(d['yaw']):+.1f}° "
                f"| src={d['pose_src']:4s} | mission={d['mission']}"
                f"{pose_warn}"
            )
        with self._elev_lock:
            elev_age = time.time() - self._elevator_last_update                        if self._elevator_last_update else 999
            # File interne FM (en attente ascenseur)
            fm_queue   = [r["robot_id"] for r in self._elevator_queue]
            # File vue depuis MQTT (ElevatorBridge)
            mqtt_queue = [q.get("robot_id", "?")
                          for q in self._elevator_mqtt_queue]
            moving_icon = "🟡" if self._elevator_moving else "⚫"
            age_warn = f" ⚠️ NO DATA ({elev_age:.0f}s)"                        if elev_age > 5.0 else f" (age={elev_age:.1f}s)"
            lines.append(
                f"  🛗 Ascenseur | F{self._elevator_floor} "
                f"| {moving_icon} pos={self._elevator_pos:.3f}m"
                f"{age_warn} "
                f"| file_FM={fm_queue} "
                f"| file_MQTT={mqtt_queue}"
            )
        with self._queue_lock:
            lines.append(f"  Missions en attente: {len(self._mission_queue)}")
        return "\n".join(lines)


# ═══════════════════════════════════════════════════════════════════════════════
#  Point d'entrée
# ═══════════════════════════════════════════════════════════════════════════════

def main(args=None):
    rclpy.init(args=args)
    node = FleetManager()

    # ── MultiThreadedExecutor : callbacks AMCL/Odom continuent
    #    même pendant que send_goal.py bloque un thread ──────────
    # 8 threads : assez pour timers + callbacks pose x2 robots +
    #             callbacks lift + mission_request + robot_status
    from rclpy.executors import MultiThreadedExecutor
    executor = MultiThreadedExecutor(num_threads=8)
    executor.add_node(node)

    # Afficher le résumé toutes les 5s dans le terminal
    def periodic_log():
        while rclpy.ok():
            time.sleep(5)
            print(node.get_status_summary())

    threading.Thread(target=periodic_log, daemon=True).start()

    try:
        executor.spin()
    except KeyboardInterrupt:
        pass
    finally:
        node.get_logger().info("FleetManager arrêt…")
        executor.shutdown()
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()