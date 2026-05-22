#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy

class OdomCovarianceFixer(Node):
    def __init__(self):
        super().__init__('odom_covariance_fixer')

        # Définir les QoS correctement
        qos_profile = QoSProfile(
            depth=10,
            reliability=ReliabilityPolicy.BEST_EFFORT,
            durability=DurabilityPolicy.VOLATILE
        )

        # Topics relatifs (namespace résolu automatiquement)
        self.pub = self.create_publisher(Odometry, 'odometry/filtered', qos_profile)
        self.sub = self.create_subscription(Odometry, 'odometry/raw', self.callback, qos_profile)
        self.get_logger().info(f'OdomCovarianceFixer ready in namespace: {self.get_namespace()}')

    def callback(self, msg):
        # Corriger l'orientation si nécessaire
        if msg.pose.pose.orientation.w < 0:
            msg.pose.pose.orientation.x *= -1
            msg.pose.pose.orientation.y *= -1
            msg.pose.pose.orientation.z *= -1
            msg.pose.pose.orientation.w *= -1
        
        # Fixer les covariances
        msg.pose.covariance[0]  = 0.1
        msg.pose.covariance[7]  = 0.1
        msg.pose.covariance[35] = 0.2
        msg.twist.covariance[0]  = 0.1
        msg.twist.covariance[7]  = 0.1
        msg.twist.covariance[35] = 0.2
        
        self.pub.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = OdomCovarianceFixer()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()