#ifndef MULTI_FLOOR_NAV__SMART_SPIN_ACTION_HPP_
#define MULTI_FLOOR_NAV__SMART_SPIN_ACTION_HPP_

#include <cmath>
#include <string>
#include <memory>

#include "nav2_behavior_tree/bt_action_node.hpp"
#include "nav2_msgs/action/spin.hpp"
#include "nav_msgs/msg/occupancy_grid.hpp"
#include "rclcpp/rclcpp.hpp"

class SmartSpinAction : public nav2_behavior_tree::BtActionNode<nav2_msgs::action::Spin>
{
public:
  SmartSpinAction(
  const std::string & xml_tag_name,
  const BT::NodeConfiguration & conf,
  rclcpp::Node::SharedPtr node)
: BtActionNode<nav2_msgs::action::Spin>(xml_tag_name, "spin", conf),
  node_(node)
{}

  void on_tick() override
  {
    double spin_dist = 1.57;
    getInput("spin_dist", spin_dist);

    nav_msgs::msg::OccupancyGrid costmap_msg;
    bool received = false;

    auto sub = node_->create_subscription<nav_msgs::msg::OccupancyGrid>(
      "local_costmap/costmap", rclcpp::QoS(1).transient_local(),
      [&](const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
        costmap_msg = *msg;
        received = true;
      });

    // Attendre max 1 seconde
    auto start = node_->now();
    while (!received && (node_->now() - start).seconds() < 1.0) {
      rclcpp::spin_some(node_);
    }

    if (!received) {
      RCLCPP_WARN(node_->get_logger(),
        "SmartSpin: costmap non reçue, spin gauche par défaut");
      goal_.target_yaw = spin_dist;
      return;
    }

    float left_cost  = computeSectorCost(costmap_msg, true);
    float right_cost = computeSectorCost(costmap_msg, false);

    goal_.target_yaw = (left_cost <= right_cost) ? +spin_dist : -spin_dist;

    RCLCPP_INFO(node_->get_logger(),
      "SmartSpin: left_cost=%.1f right_cost=%.1f → %s",
      left_cost, right_cost,
      (goal_.target_yaw > 0) ? "gauche (+)" : "droite (-)");
  }

  static BT::PortsList providedPorts()
  {
    return providedBasicPorts({
      BT::InputPort<double>("spin_dist", 1.57, "angle de spin en radians")
    });
  }

private:
  rclcpp::Node::SharedPtr node_;

  float computeSectorCost(const nav_msgs::msg::OccupancyGrid & map, bool left_side)
  {
    int width  = map.info.width;
    int height = map.info.height;
    int cx = width / 2;
    int cy = height / 2;
    float cost_sum = 0.0f;
    int count = 0;

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int dx = x - cx;
        int dy = y - cy;
        float dist  = std::sqrt(float(dx * dx + dy * dy));
        float angle = std::atan2(float(dy), float(dx));

        // Limiter à ~1 mètre autour du robot
        if (dist > (1.0f / map.info.resolution)) continue;

        // Gauche = angle [0, π], Droite = angle [-π, 0]
        bool is_left = (angle > 0.0f);
        if (is_left != left_side) continue;

        int8_t cell = map.data[y * width + x];
        if (cell > 0) cost_sum += static_cast<float>(cell);
        count++;
      }
    }

    return (count > 0) ? (cost_sum / count) : 0.0f;
  }
};

#endif  // MULTI_FLOOR_NAV__SMART_SPIN_ACTION_HPP_