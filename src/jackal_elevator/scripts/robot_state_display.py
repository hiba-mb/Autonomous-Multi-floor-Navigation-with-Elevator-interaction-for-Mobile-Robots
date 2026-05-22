#!/usr/bin/env python3
"""
Fleet Monitor v3 — avec section Mission AUTO centrale
Changement principal : ajout d'un panneau "🤖 AUTO" qui envoie
robot_id="auto" → FleetManager choisit le meilleur robot.
"""

import tkinter as tk
from tkinter import ttk
import threading
from datetime import datetime
import math
import json

import paho.mqtt.client as mqtt
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy
from std_msgs.msg import String
from geometry_msgs.msg import PoseWithCovarianceStamped
from nav_msgs.msg import Odometry
from sensor_msgs.msg import JointState

BROKER           = "localhost"
PORT             = 1883
TOPIC_STATE      = "elevator/state"
LIFT_JOINT_TOPIC = "/world/default/model/elevator/joint_state"
LIFT_JOINT_NAMES = ["lift", "lift_joint", "elevator_lift", "elevator_joint"]
FLOOR1_THRESHOLD = 1.5
ROBOTS           = ["robot1", "robot2"]

BG    = '#0d1117'
BG2   = '#161b22'
BG3   = '#21262d'
CYAN  = '#00e5ff'
DIM   = '#546e7a'
GREEN = '#69ff47'
AMBER = '#ffb74d'
RED   = '#ff4444'
R1COL = '#00e5ff'
R2COL = '#ff9f43'
AUTO_COL = '#c678dd'   # violet pour le panneau AUTO

STATUS_COLORS = {
    'idle':              GREEN,
    'busy':              AMBER,
    'waiting_elevator':  '#b39ddb',
    'going_to_elevator': '#b39ddb',
    'in_elevator':       '#b39ddb',
    'frozen':            '#ff6b6b',
    'recovery':          '#ffd93d',
    'error':             RED,
}


def _quat_to_yaw(q) -> float:
    siny = 2.0 * (q.w * q.z + q.x * q.y)
    cosy = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
    return math.atan2(siny, cosy)


def _card(parent, label, var, fg=CYAN, width=12):
    cell = tk.Frame(parent, bg=BG2)
    tk.Label(cell, text=label, font=('Consolas', 7), fg=DIM, bg=BG2).pack(pady=(4, 0))
    tk.Label(cell, textvariable=var, font=('Consolas', 13, 'bold'),
             fg=fg, bg=BG2, width=width).pack(pady=(0, 4))
    return cell


def _section(parent, title, color=CYAN):
    outer = tk.Frame(parent, bg=BG3, bd=1, relief=tk.FLAT)
    outer.pack(fill=tk.X, padx=8, pady=2)
    tk.Label(outer, text=f" {title} ",
             font=('Consolas', 8, 'bold'), fg=color, bg=BG3).pack(anchor=tk.W, padx=4, pady=(3, 0))
    inner = tk.Frame(outer, bg=BG2)
    inner.pack(fill=tk.BOTH, expand=True, padx=2, pady=(0, 2))
    return inner


# ─────────────────────────────────────────────────────────────────────────────
#  Panneau robot individuel (robot1 / robot2)
# ─────────────────────────────────────────────────────────────────────────────
class RobotPanel:
    def __init__(self, parent, robot_id: str, color: str, log_fn):
        self.robot_id = robot_id
        self.log      = log_fn
        self.color    = color

        self._x = self._y = self._yaw = 0.0
        self._floor    = "0"
        self._status   = "idle"
        self._mission  = None
        self._priority = 5
        self._pose_src  = "---"
        self._last_pose_t = None
        self._lock = threading.Lock()

        self._build(parent)

    def _build(self, parent):
        outer = tk.Frame(parent, bg=self.color, bd=2)
        outer.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=4, pady=4)
        inner = tk.Frame(outer, bg=BG)
        inner.pack(fill=tk.BOTH, expand=True, padx=2, pady=2)

        hdr = tk.Frame(inner, bg=BG)
        hdr.pack(fill=tk.X, padx=6, pady=(6, 2))
        tk.Label(hdr, text=f"⬡ {self.robot_id.upper()}",
                 font=('Consolas', 13, 'bold'), fg=self.color, bg=BG).pack(side=tk.LEFT)
        self._vstatus = tk.StringVar(value="idle")
        self._status_lbl = tk.Label(hdr, textvariable=self._vstatus,
                                    font=('Consolas', 9, 'bold'), fg=GREEN, bg=BG)
        self._status_lbl.pack(side=tk.RIGHT)
        tk.Frame(inner, height=1, bg=self.color).pack(fill=tk.X, padx=6)

        sec = _section(inner, "📍 POSITION", self.color)
        row = tk.Frame(sec, bg=BG2)
        row.pack(fill=tk.X, padx=6, pady=6)
        self._vx   = tk.StringVar(value="+0.000")
        self._vy   = tk.StringVar(value="+0.000")
        self._vyaw = tk.StringVar(value="+0.0°")
        for i, (lbl, var) in enumerate([("X (m)", self._vx), ("Y (m)", self._vy), ("Yaw", self._vyaw)]):
            c = _card(row, lbl, var, fg=self.color, width=8)
            c.grid(row=0, column=i, padx=3, sticky='nsew')
            row.columnconfigure(i, weight=1)

        info = tk.Frame(sec, bg=BG2)
        info.pack(fill=tk.X, padx=6, pady=(0, 4))
        tk.Label(info, text="Src:", font=('Consolas', 8), fg=DIM, bg=BG2).pack(side=tk.LEFT)
        self._vsrc_lbl = tk.Label(info, text="---", font=('Consolas', 8, 'bold'), fg=AMBER, bg=BG2)
        self._vsrc_lbl.pack(side=tk.LEFT, padx=(2, 0))
        tk.Label(info, text="  Âge:", font=('Consolas', 8), fg=DIM, bg=BG2).pack(side=tk.LEFT, padx=(8, 2))
        self._pose_age = tk.Label(info, text="⏱ ---", font=('Consolas', 8, 'bold'), fg=GREEN, bg=BG2)
        self._pose_age.pack(side=tk.LEFT)

        sec2 = _section(inner, "📊 ÉTAT", self.color)
        row2 = tk.Frame(sec2, bg=BG2)
        row2.pack(fill=tk.X, padx=6, pady=6)
        self._vfloor   = tk.StringVar(value="F0")
        self._vmission = tk.StringVar(value="---")
        for i, (lbl, var, fg) in enumerate([("Étage", self._vfloor, GREEN), ("Mission #", self._vmission, AMBER)]):
            c = _card(row2, lbl, var, fg=fg, width=10)
            c.grid(row=0, column=i, padx=3, sticky='nsew')
            row2.columnconfigure(i, weight=1)

        self._vprio = tk.StringVar(value="Prio: —")
        tk.Label(row2, textvariable=self._vprio,
                 font=('Consolas', 7), fg=DIM, bg=BG2).grid(row=1, column=0, columnspan=2, pady=(0, 2))

        # ── Formulaire mission explicite (robot imposé) ───────────────────────
        mf_outer = _section(inner, f"🎯 MISSION → {self.robot_id.upper()} (imposé)", self.color)
        mf = tk.Frame(mf_outer, bg=BG2)
        mf.pack(fill=tk.X, padx=6, pady=6)

        tk.Label(mf, text="Étage:", font=('Consolas', 8), fg=DIM, bg=BG2).grid(row=0, column=0, sticky='w', padx=2)
        self._tf = tk.StringVar(value="0")
        ttk.Combobox(mf, textvariable=self._tf, values=["0", "1"], width=4,
                     state='readonly').grid(row=0, column=1, padx=2)

        tk.Label(mf, text="Prio:", font=('Consolas', 8), fg=DIM, bg=BG2).grid(row=0, column=2, sticky='w', padx=(8, 1))
        self._prio_var = tk.StringVar(value="5")
        ttk.Combobox(mf, textvariable=self._prio_var,
                     values=[str(i) for i in range(10)], width=3,
                     state='readonly').grid(row=0, column=3, padx=1)

        fields = [("X:", "2.0"), ("Y:", "0.5"), ("Yaw°:", "0.0")]
        self._entries = {}
        for col, (lbl, default) in enumerate(fields, start=4):
            tk.Label(mf, text=lbl, font=('Consolas', 8), fg=DIM, bg=BG2).grid(
                row=0, column=col * 2, sticky='w', padx=(4, 1))
            e = tk.Entry(mf, width=6, font=('Consolas', 9),
                         bg=BG3, fg='#e6edf3', insertbackground='white', relief=tk.FLAT)
            e.insert(0, default)
            e.grid(row=0, column=col * 2 + 1, padx=1)
            self._entries[lbl] = e

        self._send_btn = tk.Button(
            mf_outer,
            text=f"▶ FORCER → {self.robot_id.upper()}",
            font=('Consolas', 9, 'bold'),
            bg=self.color, fg=BG,
            relief=tk.FLAT, cursor='hand2', height=1)
        self._send_btn.pack(fill=tk.X, padx=6, pady=(2, 6))

    def bind_send(self, callback):
        self._send_btn.config(command=callback)

    def get_mission_params(self) -> dict | None:
        try:
            return {
                'robot_id':     self.robot_id,   # toujours explicite ici
                'target_floor': int(self._tf.get()),
                'x':            float(self._entries["X:"].get()),
                'y':            float(self._entries["Y:"].get()),
                'yaw':          math.radians(float(self._entries["Yaw°:"].get())),
                'priority':     int(self._prio_var.get()),
            }
        except ValueError as e:
            self.log(f"❌ [{self.robot_id}] Valeur invalide: {e}")
            return None

    def update_pose(self, x, y, yaw, src, t):
        with self._lock:
            self._x = x; self._y = y; self._yaw = yaw
            self._pose_src = src; self._last_pose_t = t

    def update_fleet_status(self, status: dict):
        with self._lock:
            if 'status' in status:
                self._status  = status.get('status', 'idle')
            if 'mission' in status:
                self._mission = status.get('mission')
            if 'priority' in status:
                self._priority = status.get('priority', 5)
            if 'floor' in status:
                self._floor = str(status.get('floor', 0))

    def refresh(self):
        with self._lock:
            x, y, yaw       = self._x, self._y, self._yaw
            src, t           = self._pose_src, self._last_pose_t
            status, floor    = self._status, self._floor
            mission, priority = self._mission, self._priority

        self._vx.set(f"{x:+.3f}")
        self._vy.set(f"{y:+.3f}")
        self._vyaw.set(f"{math.degrees(yaw):+.1f}°")
        self._vsrc_lbl.config(text=src)
        self._vfloor.set(f"F{floor}")
        self._vmission.set(str(mission) if mission else "---")
        self._vprio.set(f"Prio: {priority}")

        col = STATUS_COLORS.get(status, DIM)
        self._vstatus.set(f"● {status.upper()}")
        self._status_lbl.config(fg=col)

        if t is None:
            self._pose_age.config(text="⏱ ---", fg=DIM)
        else:
            ms = (datetime.now() - t).total_seconds() * 1000
            c2 = GREEN if ms < 200 else (AMBER if ms < 1000 else RED)
            self._pose_age.config(text=f"⏱ {ms:.0f}ms", fg=c2)


# ─────────────────────────────────────────────────────────────────────────────
#  Panneau AUTO — envoie robot_id="auto" → FleetManager décide
# ─────────────────────────────────────────────────────────────────────────────
class AutoMissionPanel:
    """
    Panneau central indépendant.
    robot_id = "auto"  →  FleetManager choisit le meilleur robot.
    """
    def __init__(self, parent, log_fn):
        self.log   = log_fn
        self._lock = threading.Lock()
        self._build(parent)

    def _build(self, parent):
        outer = tk.Frame(parent, bg=AUTO_COL, bd=2)
        outer.pack(fill=tk.X, padx=4, pady=(0, 6))
        inner = tk.Frame(outer, bg=BG)
        inner.pack(fill=tk.BOTH, expand=True, padx=2, pady=2)

        # En-tête
        hdr = tk.Frame(inner, bg=BG)
        hdr.pack(fill=tk.X, padx=6, pady=(6, 2))
        tk.Label(hdr, text="🤖 MISSION AUTOMATIQUE",
                 font=('Consolas', 12, 'bold'), fg=AUTO_COL, bg=BG).pack(side=tk.LEFT)
        self._vinfo = tk.StringVar(value="FleetManager choisit le robot")
        tk.Label(hdr, textvariable=self._vinfo,
                 font=('Consolas', 8), fg=DIM, bg=BG).pack(side=tk.RIGHT)
        tk.Frame(inner, height=1, bg=AUTO_COL).pack(fill=tk.X, padx=6)

        # Formulaire compact sur une ligne
        mf = tk.Frame(inner, bg=BG2)
        mf.pack(fill=tk.X, padx=8, pady=10)

        # Étage cible
        tk.Label(mf, text="Étage:", font=('Consolas', 9), fg=DIM, bg=BG2).grid(
            row=0, column=0, sticky='w', padx=(4, 2))
        self._tf = tk.StringVar(value="0")
        ttk.Combobox(mf, textvariable=self._tf, values=["0", "1"], width=5,
                     state='readonly').grid(row=0, column=1, padx=2)

        # Coordonnées X Y
        tk.Label(mf, text="X:", font=('Consolas', 9), fg=DIM, bg=BG2).grid(
            row=0, column=2, sticky='w', padx=(14, 2))
        self._ex = tk.Entry(mf, width=7, font=('Consolas', 9),
                            bg=BG3, fg='#e6edf3', insertbackground='white', relief=tk.FLAT)
        self._ex.insert(0, "2.0")
        self._ex.grid(row=0, column=3, padx=2)

        tk.Label(mf, text="Y:", font=('Consolas', 9), fg=DIM, bg=BG2).grid(
            row=0, column=4, sticky='w', padx=(8, 2))
        self._ey = tk.Entry(mf, width=7, font=('Consolas', 9),
                            bg=BG3, fg='#e6edf3', insertbackground='white', relief=tk.FLAT)
        self._ey.insert(0, "0.5")
        self._ey.grid(row=0, column=5, padx=2)

        tk.Label(mf, text="Yaw°:", font=('Consolas', 9), fg=DIM, bg=BG2).grid(
            row=0, column=6, sticky='w', padx=(8, 2))
        self._eyaw = tk.Entry(mf, width=6, font=('Consolas', 9),
                              bg=BG3, fg='#e6edf3', insertbackground='white', relief=tk.FLAT)
        self._eyaw.insert(0, "0.0")
        self._eyaw.grid(row=0, column=7, padx=2)

        # Priorité
        tk.Label(mf, text="Prio:", font=('Consolas', 9), fg=DIM, bg=BG2).grid(
            row=0, column=8, sticky='w', padx=(14, 2))
        self._prio = tk.StringVar(value="5")
        ttk.Combobox(mf, textvariable=self._prio,
                     values=[str(i) for i in range(10)], width=3,
                     state='readonly').grid(row=0, column=9, padx=2)

        # Ligne 2 : logique de sélection + bouton
        logic_frame = tk.Frame(inner, bg=BG)
        logic_frame.pack(fill=tk.X, padx=8, pady=(0, 6))

        tk.Label(logic_frame, text="Logique de sélection:",
                 font=('Consolas', 8), fg=DIM, bg=BG).pack(side=tk.LEFT, padx=(0, 6))

        self._logic = tk.StringVar(value="auto")
        logics = [
            ("Auto (plus proche + étage)", "auto"),
            ("Même étage d'abord",         "prefer_floor"),
            ("Robot 1 prioritaire",        "prefer_r1"),
            ("Robot 2 prioritaire",        "prefer_r2"),
        ]
        for text, val in logics:
            tk.Radiobutton(logic_frame, text=text, variable=self._logic, value=val,
                           font=('Consolas', 8), fg=DIM, bg=BG,
                           selectcolor=BG3, activebackground=BG,
                           activeforeground=AUTO_COL).pack(side=tk.LEFT, padx=4)

        # Bouton principal AUTO
        self._btn = tk.Button(
            inner,
            text="🤖 ENVOYER AUTO → FleetManager choisit",
            font=('Consolas', 10, 'bold'),
            bg=AUTO_COL, fg=BG,
            relief=tk.FLAT, cursor='hand2', height=1,
            activebackground='#a855f7', activeforeground=BG)
        self._btn.pack(fill=tk.X, padx=8, pady=(4, 8))

    def bind_send(self, callback):
        self._btn.config(command=callback)

    def get_mission_params(self) -> dict | None:
        try:
            logic    = self._logic.get()
            # robot_id encodé selon la logique choisie
            # fleet_manager.py interprète "auto", "prefer_floor", etc.
            # (on peut aussi tout envoyer comme "auto" et laisser le FM décider)
            robot_id = {
                "auto":         "auto",
                "prefer_floor": "auto",   # le FM donne déjà un bonus étage
                "prefer_r1":    "robot1", # forcer robot1 si dispo, sinon file
                "prefer_r2":    "robot2",
            }.get(logic, "auto")

            return {
                'robot_id':     robot_id,
                'target_floor': int(self._tf.get()),
                'x':            float(self._ex.get()),
                'y':            float(self._ey.get()),
                'yaw':          math.radians(float(self._eyaw.get())),
                'priority':     int(self._prio.get()),
                'logic':        logic,   # info supplémentaire pour les logs
            }
        except ValueError as e:
            self.log(f"❌ [AUTO] Valeur invalide: {e}")
            return None

    def set_info(self, text: str):
        self._vinfo.set(text)


# ─────────────────────────────────────────────────────────────────────────────
#  Nœud ROS2
# ─────────────────────────────────────────────────────────────────────────────
class FleetMonitorNode(Node):
    def __init__(self, panels: dict, auto_panel: AutoMissionPanel, log_fn):
        super().__init__('fleet_monitor')
        self._panels     = panels        # {robot_id: RobotPanel}
        self._auto_panel = auto_panel
        self._log        = log_fn
        self._lock       = threading.Lock()

        self._lift_pos    = 0.0
        self._lift_floor  = "0"
        self._lift_moving = False
        self._lift_queue  = []
        self._last_lift_t = None

        odom_qos = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=10
        )

        for robot in ROBOTS:
            self.create_subscription(
                PoseWithCovarianceStamped, f'/{robot}/amcl_pose',
                lambda msg, r=robot: self._cb_amcl(msg, r), 10)
            self.create_subscription(
                Odometry, f'/{robot}/odometry/filtered',
                lambda msg, r=robot: self._cb_odom(msg, r), odom_qos)
            self.create_subscription(
                String, f'/{robot}/robot_status',
                lambda msg, r=robot: self._cb_robot_status(msg, r), 10)
            self.create_subscription(
                String, f'/{robot}/current_floor',
                lambda msg, r=robot: self._cb_robot_floor(msg, r), 10)

        self.create_subscription(
            JointState, LIFT_JOINT_TOPIC, self._cb_lift, odom_qos)

        # Subscriber fleet/status → met à jour TOUS les robots d'un coup
        self.create_subscription(
            String, '/fleet/status', self._cb_fleet_status, 10)

        # Publisher missions → FleetManager
        self._mission_pub = self.create_publisher(
            String, '/fleet/mission_request', 10)

        self._log("🟢 FleetMonitorNode démarré")

    # ── Callbacks ──────────────────────────────────────────────────────────────

    def _cb_amcl(self, msg, robot):
        x   = msg.pose.pose.position.x
        y   = msg.pose.pose.position.y
        yaw = _quat_to_yaw(msg.pose.pose.orientation)
        self._panels[robot].update_pose(x, y, yaw, 'AMCL', datetime.now())

    def _cb_odom(self, msg, robot):
        p = self._panels[robot]
        with p._lock:
            if p._pose_src == 'AMCL' and p._last_pose_t:
                if (datetime.now() - p._last_pose_t).total_seconds() < 2.0:
                    return
        x   = msg.pose.pose.position.x
        y   = msg.pose.pose.position.y
        yaw = _quat_to_yaw(msg.pose.pose.orientation)
        p.update_pose(x, y, yaw, 'Odom', datetime.now())

    def _cb_robot_status(self, msg, robot):
        try:
            data = json.loads(msg.data)
            self._panels[robot].update_fleet_status({
                'status':   data.get('state', 'idle'),
                'mission':  data.get('mission_id'),
            })
        except Exception:
            pass

    def _cb_robot_floor(self, msg, robot):
        try:
            data = json.loads(msg.data)
            self._panels[robot].update_fleet_status({'floor': data.get('floor', 0)})
        except Exception:
            pass

    def _cb_fleet_status(self, msg):
        """
        Met à jour robots + ascenseur depuis /fleet/status (publié par FleetManager).
        C'est la source principale de vérité pour le dashboard.
        """
        try:
            data   = json.loads(msg.data)
            robots = data.get('robots', {})
            for rid, info in robots.items():
                if rid in self._panels:
                    self._panels[rid].update_fleet_status({
                        'status':   info.get('state', 'idle'),
                        'floor':    info.get('floor', 0),
                        'mission':  info.get('mission'),
                    })

            # Ascenseur — file d'attente
            elev = data.get('elevator', {})
            with self._lock:
                self._lift_queue  = elev.get('waiting', [])
                self._lift_moving = elev.get('moving', False)
                ef = elev.get('floor', 0)
                self._lift_floor  = str(ef)
                self._lift_pos    = elev.get('pos', 0.0)
                self._last_lift_t = datetime.now()

            # Info dans le panneau AUTO
            free = [r for r, i in data.get('robots', {}).items()
                    if i.get('state') == 'idle']
            q    = data.get('queue', [])
            self._auto_panel.set_info(
                f"Libres: {', '.join(free) if free else 'aucun'}  |  "
                f"File: {len(q)} mission(s)")
        except Exception as e:
            self.get_logger().debug(f"_cb_fleet_status: {e}")

    def _cb_lift(self, msg):
        pos = None
        for name, p in zip(msg.name, msg.position):
            if name in LIFT_JOINT_NAMES:
                pos = p; break
        if pos is None and msg.position:
            pos = msg.position[0]
        if pos is None:
            return
        with self._lock:
            old               = self._lift_pos
            self._lift_pos    = pos
            self._lift_floor  = "1" if pos > FLOOR1_THRESHOLD else "0"
            self._lift_moving = abs(pos - old) > 0.01
            self._last_lift_t = datetime.now()

    # ── Publication ────────────────────────────────────────────────────────────

    def send_mission(self, params: dict):
        """Publie la mission sur /fleet/mission_request."""
        if params is None:
            return False
        msg      = String()
        msg.data = json.dumps(params)
        self._mission_pub.publish(msg)
        logic = params.get('logic', '')
        self._log(
            f"📤 Mission → FleetManager  "
            f"robot={params['robot_id']}  "
            f"floor={params['target_floor']}  "
            f"x={params['x']:.2f} y={params['y']:.2f}  "
            f"prio={params['priority']}"
            + (f"  logique={logic}" if logic and logic != 'auto' else "")
        )
        return True

    def get_lift_state(self) -> dict:
        with self._lock:
            return {
                'pos':    self._lift_pos,
                'floor':  self._lift_floor,
                'moving': self._lift_moving,
                'queue':  list(self._lift_queue),
                't':      self._last_lift_t,
            }


# ─────────────────────────────────────────────────────────────────────────────
#  Application principale
# ─────────────────────────────────────────────────────────────────────────────
class FleetMonitorApp:

    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Fleet Monitor v3 — Mission AUTO")
        self.root.configure(bg=BG)
        self.root.geometry("1200x900")

        self._build_ui()

        rclpy.init()
        self._node = FleetMonitorNode(self._panels, self._auto_panel, self._log)

        # Bind boutons
        for robot_id in self._panels:
            panel = self._panels[robot_id]
            panel.bind_send(lambda r=robot_id: self._on_send_explicit(r))
        self._auto_panel.bind_send(self._on_send_auto)

        # MQTT
        self._mqtt = mqtt.Client()
        self._mqtt.on_connect = lambda c, u, f, rc: (
            c.subscribe(TOPIC_STATE),
            self.root.after(0, lambda: self._log(f"✅ MQTT connecté (rc={rc})")))
        self._mqtt.on_message = self._mqtt_msg
        try:
            self._mqtt.connect(BROKER, PORT, 60)
            threading.Thread(target=self._mqtt.loop_forever, daemon=True).start()
        except Exception as e:
            self._log(f"⚠️ MQTT non disponible: {e}")

        threading.Thread(target=rclpy.spin, args=(self._node,), daemon=True).start()
        self.root.after(100, self._refresh_loop)

    # ── Construction UI ────────────────────────────────────────────────────────

    def _build_ui(self):
        # En-tête
        hdr = tk.Frame(self.root, bg=BG)
        hdr.pack(fill=tk.X, padx=10, pady=(8, 2))
        tk.Label(hdr, text="⬡  FLEET MONITOR v3",
                 font=('Consolas', 16, 'bold'), fg=CYAN, bg=BG).pack(side=tk.LEFT)
        tk.Label(hdr, text=" ⚡ FleetManager ",
                 font=('Consolas', 8, 'bold'), fg=BG, bg=GREEN,
                 padx=4, pady=2).pack(side=tk.LEFT, padx=12)
        self._vtime = tk.StringVar(value="")
        tk.Label(hdr, textvariable=self._vtime,
                 font=('Consolas', 9), fg=DIM, bg=BG).pack(side=tk.RIGHT)
        tk.Frame(self.root, height=1, bg=CYAN).pack(fill=tk.X, padx=10)

        # ── Section robots (côte à côte) ──────────────────────────────────────
        robots_frame = tk.Frame(self.root, bg=BG)
        robots_frame.pack(fill=tk.BOTH, expand=False, padx=4, pady=4)

        self._panels = {}
        colors = {'robot1': R1COL, 'robot2': R2COL}
        for robot_id in ROBOTS:
            panel = RobotPanel(robots_frame, robot_id, colors[robot_id], self._log)
            self._panels[robot_id] = panel

        # ── Section AUTO (pleine largeur, bien visible) ───────────────────────
        auto_frame = tk.Frame(self.root, bg=BG)
        auto_frame.pack(fill=tk.X, padx=4, pady=0)

        # Titre séparateur
        sep_frame = tk.Frame(auto_frame, bg=BG)
        sep_frame.pack(fill=tk.X, padx=8, pady=(4, 2))
        tk.Frame(sep_frame, height=1, bg=AUTO_COL).pack(fill=tk.X, side=tk.LEFT, expand=True)
        tk.Label(sep_frame, text="  SÉLECTION AUTOMATIQUE  ",
                 font=('Consolas', 8, 'bold'), fg=AUTO_COL, bg=BG).pack(side=tk.LEFT)
        tk.Frame(sep_frame, height=1, bg=AUTO_COL).pack(fill=tk.X, side=tk.LEFT, expand=True)

        self._auto_panel = AutoMissionPanel(auto_frame, self._log)

        # ── Ascenseur ─────────────────────────────────────────────────────────
        elev_outer = tk.Frame(self.root, bg=BG3)
        elev_outer.pack(fill=tk.X, padx=10, pady=2)
        tk.Label(elev_outer, text=" 🛗 ASCENSEUR ",
                 font=('Consolas', 8, 'bold'), fg='#b39ddb', bg=BG3).pack(
                     anchor=tk.W, padx=4, pady=(3, 0))
        elev_inner = tk.Frame(elev_outer, bg=BG2)
        elev_inner.pack(fill=tk.X, padx=2, pady=(0, 2))
        erow = tk.Frame(elev_inner, bg=BG2)
        erow.pack(fill=tk.X, padx=8, pady=6)

        self._vlift    = tk.StringVar(value="0.0000 m")
        self._vefloor  = tk.StringVar(value="F0")
        self._vemoving = tk.StringVar(value="🟢 Disponible")
        self._vequeue  = tk.StringVar(value="—")

        for i, (lbl, var, fg) in enumerate([
                ("Hauteur",    self._vlift,    '#b39ddb'),
                ("Étage",      self._vefloor,  GREEN),
                ("Statut",     self._vemoving, AMBER),
                ("En attente", self._vequeue,  DIM)]):
            c = _card(erow, lbl, var, fg=fg, width=14)
            c.grid(row=0, column=i, padx=4, sticky='nsew')
            erow.columnconfigure(i, weight=1)

        # ── Journal ───────────────────────────────────────────────────────────
        log_outer = tk.Frame(self.root, bg=BG3)
        log_outer.pack(fill=tk.BOTH, expand=True, padx=10, pady=2)
        tk.Label(log_outer, text=" 📝 JOURNAL ",
                 font=('Consolas', 8, 'bold'), fg=CYAN, bg=BG3).pack(
                     anchor=tk.W, padx=4, pady=(3, 0))
        log_inner = tk.Frame(log_outer, bg=BG2)
        log_inner.pack(fill=tk.BOTH, expand=True, padx=2, pady=(0, 2))
        self._log_txt = tk.Text(log_inner, bg=BG2, fg='#8b949e',
                                font=('Consolas', 8), wrap=tk.WORD,
                                relief=tk.FLAT, height=7)
        sb = tk.Scrollbar(log_inner, command=self._log_txt.yview,
                          bg=BG3, troughcolor=BG2)
        self._log_txt.configure(yscrollcommand=sb.set)
        self._log_txt.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(8, 0), pady=4)
        sb.pack(side=tk.RIGHT, fill=tk.Y, pady=4)

        self._status_bar = tk.Label(
            self.root, text="🟢 Prêt",
            bd=0, anchor=tk.W, bg=BG2, fg=CYAN,
            font=('Consolas', 9), padx=12, pady=4)
        self._status_bar.pack(side=tk.BOTTOM, fill=tk.X)

    # ── Callbacks boutons ─────────────────────────────────────────────────────

    def _on_send_auto(self):
        """Bouton AUTO — robot_id='auto' → FleetManager choisit."""
        params = self._auto_panel.get_mission_params()
        if params and self._node.send_mission(params):
            self._status_bar.config(
                text=f"🤖 AUTO → FleetManager  floor={params['target_floor']}  "
                     f"x={params['x']:.2f} y={params['y']:.2f}  prio={params['priority']}")

    def _on_send_explicit(self, robot_id: str):
        """Bouton FORCER → robot imposé."""
        params = self._panels[robot_id].get_mission_params()
        if params and self._node.send_mission(params):
            self._status_bar.config(
                text=f"▶ FORCÉ → {robot_id}  floor={params['target_floor']}  "
                     f"x={params['x']:.2f} y={params['y']:.2f}  prio={params['priority']}")

    # ── MQTT ──────────────────────────────────────────────────────────────────

    def _mqtt_msg(self, client, userdata, msg):
        try:
            data  = json.loads(msg.payload.decode())
            elev  = data.get('elevator', {})
            queue = elev.get('queue', [])
            self.root.after(0, lambda: self._vequeue.set(
                ', '.join(q.get('robot_id', '?') for q in queue) if queue else '—'))
        except Exception:
            pass

    # ── Refresh loop ──────────────────────────────────────────────────────────

    def _refresh_loop(self):
        self._vtime.set(datetime.now().strftime("%H:%M:%S"))
        for panel in self._panels.values():
            panel.refresh()

        lift = self._node.get_lift_state()
        self._vlift.set(f"{lift['pos']:.4f} m")
        self._vefloor.set(f"F{lift['floor']}")
        self._vemoving.set("🟡 En mouvement…" if lift['moving'] else "🟢 Disponible")
        if lift['queue']:
            self._vequeue.set(', '.join(r.get('robot_id', '?') for r in lift['queue']))

        self.root.after(100, self._refresh_loop)

    # ── Log ───────────────────────────────────────────────────────────────────

    def _log(self, msg: str):
        ts = datetime.now().strftime("%H:%M:%S")
        if hasattr(self, '_log_txt'):
            self._log_txt.insert(tk.END, f"[{ts}] {msg}\n")
            self._log_txt.see(tk.END)
            lines = int(self._log_txt.index('end-1c').split('.')[0])
            if lines > 300:
                self._log_txt.delete('1.0', '50.0')

    # ── Run ───────────────────────────────────────────────────────────────────

    def run(self):
        self._log("🟢 Fleet Monitor v3 démarré")
        self._log("🤖 Panneau AUTO disponible — FleetManager sélectionne le robot")
        self._log("📡 En attente des topics ROS2…")
        try:
            self.root.mainloop()
        finally:
            self._node.destroy_node()
            rclpy.shutdown()


def main():
    app = FleetMonitorApp()
    app.run()


if __name__ == '__main__':
    main()