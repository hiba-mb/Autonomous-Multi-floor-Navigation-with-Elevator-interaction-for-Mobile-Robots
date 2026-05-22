#!/usr/bin/env python3
import math, json, time, threading, subprocess
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy, DurabilityPolicy
import paho.mqtt.client as mqtt
from std_msgs.msg import Bool, String
from geometry_msgs.msg import PoseWithCovarianceStamped
from nav_msgs.msg import Odometry
from sensor_msgs.msg import JointState

BROKER = "localhost"
PORT   = 1883

TOPIC_CALL    = "elevator/call"
TOPIC_STATUS  = "elevator/status"
TOPIC_STATE   = "elevator/state"
TOPIC_RPI_CMD = "rpi/elevator/call"
TOPIC_RPI_FB  = "rpi/elevator/status"
TOPIC_REQUEST = "elevator/request"
TOPIC_RELEASE = "elevator/release"
TOPIC_GRANT   = "elevator/grant"    # bridge → BT
ROBOTS           = ["robot1", "robot2"]
FLOOR_HEIGHTS    = {"0": 0.0, "1": 3.09}
FLOOR1_THRESHOLD = 1.5
MQTT_HZ          = 1

def _quat_to_yaw(q):
    siny = 2.0*(q.w*q.z + q.x*q.y)
    cosy = 1.0 - 2.0*(q.y*q.y + q.z*q.z)
    return math.atan2(siny, cosy)

class ElevatorMQTTBridge(Node):
    def __init__(self):
        super().__init__('elevator_mqtt_bridge')
        self.lift_pos        = 0.0
        self.is_moving       = False
        self.current_floor   = "0"
        self._lock           = threading.Lock()
        self.robot_poses     = {}

        # Verrou ascenseur
        self._elevator_owner = None
        self._waiting_queue  = []
        self._verrou_lock    = threading.Lock()

        odom_qos = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST, depth=10)

        for robot in ROBOTS:
            self.create_subscription(
                PoseWithCovarianceStamped, f'/{robot}/amcl_pose',
                lambda msg, r=robot: self._cb_amcl(msg, r), 10)
            self.create_subscription(
                Odometry, f'/{robot}/odometry/filtered',
                lambda msg, r=robot: self._cb_odom(msg, r), odom_qos)

        self.create_subscription(
            JointState, '/world/default/model/elevator/joint_state',
            self._cb_lift, odom_qos)
        self.create_subscription(
            Bool, '/elevator/exit_done', self._cb_exit_done, 10)

        ready_qos = QoSProfile(depth=1, durability=DurabilityPolicy.TRANSIENT_LOCAL)
        self.ready_pubs = {
            robot: self.create_publisher(String, f'/{robot}/elevator/ready', ready_qos)
            for robot in ROBOTS}

        self.create_timer(1.0/MQTT_HZ, self._publish_state)

        self.mqtt_client = mqtt.Client(
            client_id="elevator-bridge-ubuntu",
            clean_session=True
        )
        self.mqtt_client.on_connect = self._mqtt_on_connect
        self.mqtt_client.on_message = self._mqtt_on_message
        self.mqtt_client.connect(BROKER, PORT, 60)
        self.mqtt_client.loop_start()
        self.get_logger().info(f'Bridge prêt — robots: {ROBOTS}')

    def _cb_amcl(self, msg, robot):
        with self._lock:
            self.robot_poses[robot] = {
                'x': msg.pose.pose.position.x,
                'y': msg.pose.pose.position.y,
                'yaw': _quat_to_yaw(msg.pose.pose.orientation)}

    def _cb_odom(self, msg, robot):
        with self._lock:
            if robot not in self.robot_poses:
                self.robot_poses[robot] = {
                    'x': msg.pose.pose.position.x,
                    'y': msg.pose.pose.position.y,
                    'yaw': _quat_to_yaw(msg.pose.pose.orientation)}

    def _cb_lift(self, msg):
        pos = None
        for name, p in zip(msg.name, msg.position):
            if name == "lift":
                pos = p
                break
        if pos is None and msg.position:
            pos = msg.position[0]
        if pos is None:
            return
        with self._lock:
            self.lift_pos      = pos
            self.current_floor = "1" if pos > FLOOR1_THRESHOLD else "0"

    def _cb_exit_done(self, msg):
        if msg.data:
            self.get_logger().info('Sortie ascenseur confirmée')

    def _publish_state(self):
        with self._lock:
            state = {
                'robots': self.robot_poses,
                'elevator': {
                    'lift_height':   round(self.lift_pos, 4),
                    'is_moving':     self.is_moving,
                    'current_floor': self.current_floor},
                'timestamp': time.time()}
        self.mqtt_client.publish(TOPIC_STATE, json.dumps(state))

    def _gz_send_lift(self, height):
        subprocess.Popen(
            f'gz topic -t /model/elevator/joint/lift/0/cmd_pos '
            f'-m gz.msgs.Double -p "data: {height}"',
            shell=True)

    def _gz_pause(self, pause):
        subprocess.run([
            'gz', 'service', '-s', '/world/default/control',
            '--reqtype', 'gz.msgs.WorldControl',
            '--reptype', 'gz.msgs.Boolean',
            '--timeout', '2000', '--req',
            f'pause: {"true" if pause else "false"}'], capture_output=True)

    def _wait_lift_arrived(self, target, timeout=25.0):
        start = time.time()
        while time.time() - start < timeout:
            with self._lock:
                pos = self.lift_pos
            if abs(pos - target) < 0.15:
                self.get_logger().info(f'Lift @ {pos:.3f}m ✓')
                return True
            time.sleep(0.05)
        self.get_logger().warn(f'Timeout lift — pos={self.lift_pos:.3f}m cible={target}m')
        return False

    def _handle_floor_request(self, requested_floor, robot_id, source):
        self.is_moving = True
        with self._lock:
            current = self.current_floor

        if current != requested_floor:
            self._gz_send_lift(FLOOR_HEIGHTS[requested_floor])
            self._wait_lift_arrived(FLOOR_HEIGHTS[requested_floor])
            self._gz_pause(True)
            time.sleep(0.3)
            self._gz_pause(False)
            time.sleep(0.3)
        
        # Notifier seulement après arrivée
        self._notify_robot(robot_id, requested_floor)
        self.mqtt_client.publish(TOPIC_STATUS, f"arrived:{requested_floor}", retain=False)
        self.mqtt_client.publish(TOPIC_RPI_FB, f"arrived:{requested_floor}:{robot_id}")
        self.is_moving = False

    def _notify_robot(self, robot_id, floor):
        if robot_id not in self.ready_pubs:
            self.get_logger().error(f'Robot inconnu: {robot_id}')
            return
        msg = String()
        msg.data = floor
        self.ready_pubs[robot_id].publish(msg)
        self.get_logger().info(f'[{robot_id}] → /{robot_id}/elevator/ready = {floor}')

    def _grant_elevator(self, robot_id, floor):
        self._elevator_owner = robot_id
        self.mqtt_client.publish(
            f"elevator/grant/{robot_id}",
            json.dumps({"floor": floor, "robot_id": robot_id}),
            retain=True  # ← le broker garde le message pour les nouveaux subscribers
        )
        self.get_logger().info(f'[GRANT] verrou → {robot_id} étage {floor}')

    def _mqtt_on_connect(self, client, userdata, flags, rc):
        self.get_logger().info(f'MQTT connecté (rc={rc})')
        client.subscribe(TOPIC_CALL)
        client.subscribe(TOPIC_RPI_CMD)
        client.subscribe(TOPIC_RELEASE)
        client.subscribe(TOPIC_REQUEST)

    def _mqtt_on_message(self, client, userdata, msg):
        payload = msg.payload.decode().strip()
        if not payload:
            return
        try:
            data     = json.loads(payload)
            floor    = str(data.get('floor', '0'))
            robot_id = str(data.get('robot_id', ROBOTS[0]))
        except Exception:
            floor    = payload
            robot_id = ROBOTS[0]

        # Release verrou
        if msg.topic == TOPIC_RELEASE:
            with self._verrou_lock:
                self.get_logger().info(f'[RELEASE] verrou libéré par {robot_id}')
                # Effacer le retain
                self.mqtt_client.publish(f"elevator/grant/{robot_id}", "", retain=True)
                self._elevator_owner = None
                if self._waiting_queue:
                    next_req = self._waiting_queue.pop(0)
                    self._grant_elevator(next_req['robot_id'], next_req['floor'])
            return

        if floor not in FLOOR_HEIGHTS:
            self.get_logger().error(f'Étage inconnu: {floor}')
            return

        # BT demande verrou
        if msg.topic == TOPIC_CALL:
            self.get_logger().info(f'[BT|{robot_id}] étage {floor}')
            with self._verrou_lock:
                if self._elevator_owner is None:
                    self._grant_elevator(robot_id, floor)
                    self.mqtt_client.publish(
                        TOPIC_REQUEST,
                        json.dumps({"floor": floor, "robot_id": robot_id}))
                elif self._elevator_owner == robot_id:
                    self.mqtt_client.publish(
                        TOPIC_REQUEST,
                        json.dumps({"floor": floor, "robot_id": robot_id}))
                else:
                    self._waiting_queue.append({"floor": floor, "robot_id": robot_id})
                    self.get_logger().info(
                        f'[WAIT] {robot_id} en file — verrou tenu par {self._elevator_owner}')

        # RPi commande ascenseur
        elif msg.topic in (TOPIC_RPI_CMD, TOPIC_REQUEST):
            self.get_logger().info(f'[RPi|{robot_id}] étage {floor}')
            threading.Thread(
                target=self._handle_floor_request,
                args=(floor, robot_id, 'rpi'),
                daemon=True
            ).start()

def main(args=None):
    rclpy.init(args=args)
    node = ElevatorMQTTBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()