#pragma once

#include <behaviortree_cpp/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <nav2_msgs/action/navigate_to_pose.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <string>
#include <cmath>
#include <std_msgs/msg/bool.hpp>  // ← AJOUTER CETTE LIGNE
using NavigateToPose = nav2_msgs::action::NavigateToPose;
using GoalHandleNav  = rclcpp_action::ClientGoalHandle<NavigateToPose>;

// ─────────────────────────────────────────────────────────────────────────────
// EnterElevator : cmd_vel direct (pas de carte dans la cabine)
// ─────────────────────────────────────────────────────────────────────────────
class EnterElevator : public BT::StatefulActionNode
{
public:
  EnterElevator(const std::string & name, const BT::NodeConfig & config,
                rclcpp::Node::SharedPtr node)
  : BT::StatefulActionNode(name, config), node_(node),
    distance_travelled_(0.0), started_(false) {}

  static BT::PortsList providedPorts() {
    return {
      BT::InputPort<std::string>("cmd_vel_topic", "/cmd_vel", "Topic cmd_vel"),
      BT::InputPort<double>("distance", 3.0,  "Distance (m)"),
      BT::InputPort<double>("speed",    0.25, "Vitesse (m/s)"),
    };
  }

  BT::NodeStatus onStart() override {
    std::string topic;
    getInput("cmd_vel_topic", topic);
    getInput("distance", target_distance_);
    getInput("speed",    speed_);

    cmd_vel_pub_ = node_->create_publisher<geometry_msgs::msg::Twist>(topic, 10);
    odom_sub_ = node_->create_subscription<nav_msgs::msg::Odometry>(
      "odometry/filtered", 10,
      [this](const nav_msgs::msg::Odometry::SharedPtr msg) {
        if (!started_) {
          start_pos_ = msg->pose.pose.position;
          started_ = true;
        } else {
          auto & p = msg->pose.pose.position;
          double dx = p.x - start_pos_.x;
          double dy = p.y - start_pos_.y;
          distance_travelled_ = std::sqrt(dx*dx + dy*dy);
        }
      });

    distance_travelled_ = 0.0;
    started_ = false;
    RCLCPP_INFO(node_->get_logger(),
      "[EnterElevator] Entrée ascenseur — distance=%.2f speed=%.2f",
      target_distance_, speed_);
    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus onRunning() override {
    if (distance_travelled_ >= target_distance_) {
      geometry_msgs::msg::Twist stop;
      cmd_vel_pub_->publish(stop);
      RCLCPP_INFO(node_->get_logger(),
        "[EnterElevator] Distance atteinte: %.2f m", distance_travelled_);
      return BT::NodeStatus::SUCCESS;
    }
    geometry_msgs::msg::Twist cmd;
    cmd.linear.x = speed_;
    cmd_vel_pub_->publish(cmd);
    return BT::NodeStatus::RUNNING;
  }

  void onHalted() override {
    geometry_msgs::msg::Twist stop;
    if (cmd_vel_pub_) cmd_vel_pub_->publish(stop);
  }

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
  geometry_msgs::msg::Point start_pos_;
  double distance_travelled_, target_distance_, speed_;
  bool started_;
};

// ─────────────────────────────────────────────────────────────────────────────
// ExitElevator : Nav2 vers point de sortie
// FIX 1 : goal_response_callback ajouté → détecte rejet immédiat de Nav2
// FIX 2 : valeurs par défaut neutrales (0.0) — le XML doit toujours fournir les vraies valeurs
// FIX 3 : timeout de sécurité → évite boucle infinie si Nav2 ne répond plus
// ─────────────────────────────────────────────────────────────────────────────Exit
// ─────────────────────────────────────────────────────────────────────────────
// ExitElevator : cmd_vel direct pour sortir de l'ascenseur (recul)
// ─────────────────────────────────────────────────────────────────────────────
class ExitElevator : public BT::StatefulActionNode
{
public:
  ExitElevator(const std::string & name, const BT::NodeConfig & config,
               rclcpp::Node::SharedPtr node)
  : BT::StatefulActionNode(name, config), node_(node),
    distance_travelled_(0.0), started_(false) {}

  static BT::PortsList providedPorts() {
    return {
      BT::InputPort<double>("distance", 3.0,  "Distance de recul (m)"),
      BT::InputPort<double>("speed",    -0.25, "Vitesse de recul (m/s, négatif)"),
    };
  }

  BT::NodeStatus onStart() override
{
  getInput("distance", target_distance_);
  getInput("speed", speed_);

  // namespace du robot
  std::string ns = node_->get_namespace();
  cmd_vel_pub_ = node_->create_publisher<geometry_msgs::msg::Twist>(
    ns + "/cmd_vel", 10);
  exit_done_pub_ = node_->create_publisher<std_msgs::msg::Bool>(
    "/elevator/exit_done", 1);

  auto qos = rclcpp::QoS(rclcpp::KeepLast(10))
    .reliability(rclcpp::ReliabilityPolicy::BestEffort)
    .durability(rclcpp::DurabilityPolicy::Volatile);

  odom_sub_ = node_->create_subscription<nav_msgs::msg::Odometry>(
    ns + "/odometry/filtered", qos,
    [this](const nav_msgs::msg::Odometry::SharedPtr msg) {
      if (!started_) {
        start_pos_ = msg->pose.pose.position;
        started_ = true;
      } else {
        auto & p = msg->pose.pose.position;
        double dx = p.x - start_pos_.x;
        double dy = p.y - start_pos_.y;
        distance_travelled_ = std::sqrt(dx*dx + dy*dy);
      }
    });

  distance_travelled_ = 0.0;
  started_ = false;
  start_time_ = node_->now();

  return BT::NodeStatus::RUNNING;
  // ← ne pas publier ici, onRunning s'en charge
}

BT::NodeStatus onRunning() override
{
  // republier à chaque tick pour contrer Nav2
  geometry_msgs::msg::Twist twist;
  twist.linear.x = speed_;
  cmd_vel_pub_->publish(twist);

  double elapsed = (node_->now() - start_time_).seconds();
  double time_based_dist = std::abs(elapsed * speed_);
  double effective_dist = started_ ? std::abs(distance_travelled_) : time_based_dist;

  if (effective_dist >= std::abs(target_distance_)) {
    geometry_msgs::msg::Twist stop;
    cmd_vel_pub_->publish(stop);

    std_msgs::msg::Bool done;
    done.data = true;
    exit_done_pub_->publish(done);

    RCLCPP_INFO(node_->get_logger(),
      "[ExitElevator] Recul terminé: %.2f m (odom=%s)",
      effective_dist, started_ ? "oui" : "fallback timer");
    return BT::NodeStatus::SUCCESS;
  }

  return BT::NodeStatus::RUNNING;
}

  void onHalted() override {
    geometry_msgs::msg::Twist stop;
    if (cmd_vel_pub_) cmd_vel_pub_->publish(stop);
    
    std_msgs::msg::Bool done;
    done.data = true;
    if (exit_done_pub_) exit_done_pub_->publish(done);  // réactiver même en cas d'interruption
    
    RCLCPP_WARN(node_->get_logger(), "[ExitElevator] Halted — arrêt d'urgence");
  }

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr exit_done_pub_;
  geometry_msgs::msg::Point start_pos_;
  double distance_travelled_, target_distance_, speed_;
  bool started_;
  rclcpp::Time start_time_;
};