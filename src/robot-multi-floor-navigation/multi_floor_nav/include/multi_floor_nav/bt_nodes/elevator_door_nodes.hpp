#pragma once

#include <behaviortree_cpp/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <string>

// ─────────────────────────────────────────────
// OpenElevatorDoor : publie position=1.0 sur /elevator/door_joint/cmd
// ─────────────────────────────────────────────
class OpenElevatorDoor : public BT::SyncActionNode
{
public:
  OpenElevatorDoor(const std::string & name, const BT::NodeConfig & config,
                   rclcpp::Node::SharedPtr node)
  : BT::SyncActionNode(name, config), node_(node)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<std::string>("topic", "/elevator/door_joint/cmd", "Topic Gazebo porte"),
      BT::InputPort<double>("position", 1.0, "Position ouverte (0.0 - 1.0)")
    };
  }

  BT::NodeStatus tick() override
  {
    std::string topic;
    double position;
    getInput("topic", topic);
    getInput("position", position);

    auto pub = node_->create_publisher<std_msgs::msg::Float64>(topic, 1);
    std_msgs::msg::Float64 msg;
    msg.data = position;

    // Publier quelques fois pour s'assurer que Gazebo reçoit
    for (int i = 0; i < 3; ++i) {
      pub->publish(msg);
      rclcpp::sleep_for(std::chrono::milliseconds(100));
    }

    RCLCPP_INFO(node_->get_logger(), "[OpenElevatorDoor] Porte ouverte (%.1f)", position);
    return BT::NodeStatus::SUCCESS;
  }

private:
  rclcpp::Node::SharedPtr node_;
};

// ─────────────────────────────────────────────
// CloseElevatorDoor : publie position=0.0 sur /elevator/door_joint/cmd
// ─────────────────────────────────────────────
class CloseElevatorDoor : public BT::SyncActionNode
{
public:
  CloseElevatorDoor(const std::string & name, const BT::NodeConfig & config,
                    rclcpp::Node::SharedPtr node)
  : BT::SyncActionNode(name, config), node_(node)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<std::string>("topic", "/elevator/door_joint/cmd", "Topic Gazebo porte"),
      BT::InputPort<double>("position", 0.0, "Position fermée")
    };
  }

  BT::NodeStatus tick() override
  {
    std::string topic;
    double position;
    getInput("topic", topic);
    getInput("position", position);

    auto pub = node_->create_publisher<std_msgs::msg::Float64>(topic, 1);
    std_msgs::msg::Float64 msg;
    msg.data = position;

    for (int i = 0; i < 3; ++i) {
      pub->publish(msg);
      rclcpp::sleep_for(std::chrono::milliseconds(100));
    }

    RCLCPP_INFO(node_->get_logger(), "[CloseElevatorDoor] Porte fermée (%.1f)", position);
    return BT::NodeStatus::SUCCESS;
  }

private:
  rclcpp::Node::SharedPtr node_;
};
