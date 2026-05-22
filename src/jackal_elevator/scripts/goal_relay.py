#!/usr/bin/env python3
import argparse
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy
from geometry_msgs.msg import PoseStamped

class GoalRelay(Node):
    def __init__(self, robot_ns: str):
        super().__init__('goal_relay', namespace=robot_ns)
        self.robot_ns = robot_ns

        qos_be = QoSProfile(depth=10)
        qos_be.reliability = ReliabilityPolicy.BEST_EFFORT
        qos_be.durability   = DurabilityPolicy.VOLATILE

        # Topic RViz global  →  ex: /robot1/goal_pose  (lu par elevator_bt_executor)
        self.pub = self.create_publisher(
            PoseStamped,
            f'/{robot_ns}/goal_pose',   # topic absolu dans le namespace du robot
            qos_be
        )

        # RViz publie sur  /robot1/goal_pose  ou  /robot2/goal_pose
        # selon le panel "Navigation2 Goal" configuré avec le bon topic
        self.sub = self.create_subscription(
            PoseStamped,
            f'/{robot_ns}/goal_pose_rviz',   # topic dédié RViz → relay
            self.callback,
            10
        )

        self.get_logger().info(
            f'[{robot_ns}] GoalRelay ready — '
            f'listening on /{robot_ns}/goal_pose_rviz'
        )

    def callback(self, msg: PoseStamped):
        self.get_logger().info(
            f'[{self.robot_ns}] Goal → '
            f'x={msg.pose.position.x:.2f} y={msg.pose.position.y:.2f}'
        )
        msg.header.stamp    = self.get_clock().now().to_msg()
        msg.header.frame_id = 'map'
        self.pub.publish(msg)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--namespace', required=True,
                        choices=['robot1', 'robot2'])
    args, _ = parser.parse_known_args()

    rclpy.init()
    rclpy.spin(GoalRelay(args.namespace))

if __name__ == '__main__':
    main()