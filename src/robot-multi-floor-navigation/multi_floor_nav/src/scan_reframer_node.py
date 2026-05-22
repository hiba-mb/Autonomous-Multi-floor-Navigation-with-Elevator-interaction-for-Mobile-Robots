#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import LaserScan
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy

class ScanReframer(Node):
    def __init__(self):
        super().__init__('scan_reframer')

        ns = self.get_namespace().strip('/')  # ← déplacé ici, avant utilisation
        frame = f'{ns}/front_laser' if ns else 'front_laser'
        self.frame_id = frame

        qos_sub = QoSProfile(
            depth=10,
            reliability=ReliabilityPolicy.BEST_EFFORT,
            durability=DurabilityPolicy.VOLATILE
        )
        qos_pub = QoSProfile(
            depth=10,
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.VOLATILE
        )

        self.pub = self.create_publisher(LaserScan, 'front/scan_fixed', qos_pub)
        self.sub = self.create_subscription(LaserScan, 'front/scan', self.callback, qos_sub)
        self.get_logger().info(f'ScanReframer ready: front/scan -> front/scan_fixed [frame_id={frame}]')

    def callback(self, msg):
        msg.header.frame_id = self.frame_id
        self.pub.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = ScanReframer()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()