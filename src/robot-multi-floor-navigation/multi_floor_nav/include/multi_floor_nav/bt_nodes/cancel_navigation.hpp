// cancel_navigation.hpp
#pragma once
#include <behaviortree_cpp/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>

class CancelNavigation : public BT::SyncActionNode
{
public:
  CancelNavigation(const std::string & name, const BT::NodeConfig & config,
                   rclcpp::Node::SharedPtr node)
  : BT::SyncActionNode(name, config), node_(node)
  {
    cmd_pub_ = node_->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 1);
  }

  static BT::PortsList providedPorts() { return {}; }

  BT::NodeStatus tick() override
  {
    // Publier 5x stop pour écraser toute commande résiduelle du contrôleur
    geometry_msgs::msg::Twist stop;
    for (int i = 0; i < 5; ++i) {
      cmd_pub_->publish(stop);
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    RCLCPP_INFO(node_->get_logger(), "[CancelNavigation] cmd_vel stoppé");
    return BT::NodeStatus::SUCCESS;
  }

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
};