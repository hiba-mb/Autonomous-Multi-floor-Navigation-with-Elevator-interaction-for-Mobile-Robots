#pragma once

#include <string>
#include <memory>
#include <atomic>
#include "behaviortree_cpp/action_node.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <nlohmann/json.hpp>

class PublishFloorStatus : public BT::SyncActionNode
{
public:
    PublishFloorStatus(
        const std::string& name,
        const BT::NodeConfig& config,
        rclcpp::Node::SharedPtr node,
        const std::string& robot_namespace,
        std::atomic<bool>& flag)
        : BT::SyncActionNode(name, config),
          node_(node),
          robot_namespace_(robot_namespace),
          cancel_flag_(flag)
    {
        std::string topic = "/" + robot_namespace_ + "/robot_status";
        pub_ = node_->create_publisher<std_msgs::msg::String>(topic, 10);
        RCLCPP_INFO(node_->get_logger(),
            "[PublishFloorStatus] Publisher created on %s", topic.c_str());
    }
    
    static BT::PortsList providedPorts()
    {
        return {
            BT::InputPort<int>("floor", "Current robot floor (0 or 1)"),
            BT::InputPort<std::string>("state", "Robot state: busy / idle / in_elevator"),
        };
    }
    
    BT::NodeStatus tick() override
    {
        auto floor_res = getInput<int>("floor");
        auto state_res = getInput<std::string>("state");
        
        if (!floor_res || !state_res) {
            RCLCPP_ERROR(node_->get_logger(),
                "[PublishFloorStatus] Missing port: floor=%s state=%s",
                floor_res ? "ok" : "MISSING",
                state_res ? "ok" : "MISSING");
            return BT::NodeStatus::FAILURE;
        }
        
        int floor = floor_res.value();
        std::string state = state_res.value();
        
        nlohmann::json j;
        j["state"] = state;
        j["floor"] = floor;
        std::string payload = j.dump();
        
        std_msgs::msg::String msg;
        msg.data = payload;
        pub_->publish(msg);
        
        RCLCPP_INFO(node_->get_logger(),
            "[PublishFloorStatus] ✅ Published to /%s/robot_status: %s",
            robot_namespace_.c_str(), payload.c_str());
        
        return BT::NodeStatus::SUCCESS;
    }
    
private:
    rclcpp::Node::SharedPtr node_;
    std::string robot_namespace_;
    std::atomic<bool>& cancel_flag_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
};