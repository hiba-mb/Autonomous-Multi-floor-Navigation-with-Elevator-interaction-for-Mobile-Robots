#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovarianceStamped, TransformStamped
from tf2_ros import TransformBroadcaster
import math

class ForceSync(Node):
    def __init__(self):
        super().__init__('force_sync')
        
        # QoS
        qos = QoSProfile(depth=10, reliability=ReliabilityPolicy.BEST_EFFORT)
        
        # Publishers
        self.pose_pub = self.create_publisher(PoseWithCovarianceStamped, '/initialpose', 10)
        self.tf_broadcaster = TransformBroadcaster(self)
        
        # Subscribe to Gazebo odometry
        self.odom_sub = self.create_subscription(
            Odometry, 
            '/odometry/filtered',
            self.odom_callback, 
            qos
        )
        
        # Timer pour forcer la synchro en continu
        self.create_timer(0.1, self.force_sync)  # 10 fois par seconde
        
        self.current_pose = None
        self.get_logger().info("✅ Force Sync activé")
    
    def odom_callback(self, msg):
        self.current_pose = msg
    
    def force_sync(self):
        if self.current_pose is None:
            return
        
        # 1. Publier sur /initialpose
        pose_msg = PoseWithCovarianceStamped()
        pose_msg.header = self.current_pose.header
        pose_msg.header.frame_id = 'map'
        pose_msg.pose.pose = self.current_pose.pose.pose
        pose_msg.pose.covariance = [0.001] * 36  # Confiance totale
        self.pose_pub.publish(pose_msg)
        
        # 2. Publier TF map→odom
        t = TransformStamped()
        t.header = self.current_pose.header
        t.header.frame_id = 'map'
        t.child_frame_id = 'odom'
        t.transform.translation.x = self.current_pose.pose.pose.position.x
        t.transform.translation.y = self.current_pose.pose.pose.position.y
        t.transform.translation.z = self.current_pose.pose.pose.position.z
        t.transform.rotation = self.current_pose.pose.pose.orientation
        self.tf_broadcaster.sendTransform(t)

def main():
    rclpy.init()
    node = ForceSync()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()