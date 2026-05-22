#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import LaserScan

class FixScanFrame(Node):
    def __init__(self):
        super().__init__('fix_scan_frame')
        self.sub = self.create_subscription(
            LaserScan, '/front/scan_raw', self.callback, 10)
        self.pub = self.create_publisher(LaserScan, '/front/scan', 10)
        self.get_logger().info('fix_scan_frame started')

    def callback(self, msg):
        msg.header.frame_id = 'front_laser'
        self.pub.publish(msg)

def main():
    rclpy.init()
    rclpy.spin(FixScanFrame())

if __name__ == '__main__':
    main()
