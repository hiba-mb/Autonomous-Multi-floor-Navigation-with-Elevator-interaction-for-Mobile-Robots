#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <nav2_msgs/action/navigate_to_pose.hpp>
#include "nav_msgs/msg/odometry.hpp"
#include <cmath>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <std_msgs/msg/int32.hpp>
#include "std_msgs/msg/string.hpp"
#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/loggers/bt_cout_logger.h>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <thread>
#include <chrono>
#include <cstdio>
#include <string>
#include "multi_floor_nav/bt_nodes/smart_spin_action.hpp"
#include "multi_floor_nav/bt_nodes/cancel_navigation.hpp"
#include "multi_floor_nav/bt_nodes/PublishFloorStatus.hpp"
#include "multi_floor_nav/bt_nodes/elevator_door_nodes.hpp"
#include "multi_floor_nav/bt_nodes/elevator_move_nodes.hpp"
#include "multi_floor_nav/bt_nodes/elevator_mqtt_nodes.hpp"
#include "multi_floor_nav/srv/int_trigger.hpp"

#include <nav2_msgs/srv/clear_entire_costmap.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2/time.h>
#include <std_msgs/msg/int32.hpp>

using NavigateToPose = nav2_msgs::action::NavigateToPose;
using GoalHandleNav  = rclcpp_action::ClientGoalHandle<NavigateToPose>;
using ServerGoalHandle = rclcpp_action::ServerGoalHandle<NavigateToPose>;

// ─────────────────────────────────────────────────────────────────────────────
// Helper Gazebo Z — paramétré par nom de modèle
// ─────────────────────────────────────────────────────────────────────────────
double getRobotZ(const std::string & model_name)
{
  std::string cmd = "gz model -m " + model_name +
                    " -p 2>/dev/null | grep -A 1 'XYZ' | tail -1";
  FILE * pipe = popen(cmd.c_str(), "r");
  if (!pipe) return 0.0;
  char buf[256]; std::string res;
  while (fgets(buf, sizeof(buf), pipe)) res += buf;
  pclose(pipe);
  double x, y, z;
  if (sscanf(res.c_str(), " [%lf %lf %lf]", &x, &y, &z) == 3) return z;
  return 0.0;
}

int getCurrentFloor(const std::string & model_name)
{
  return (getRobotZ(model_name) > 1.5) ? 1 : 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// Nœud Wait
// ─────────────────────────────────────────────────────────────────────────────
class Wait : public BT::SyncActionNode
{
public:
  Wait(const std::string & name, const BT::NodeConfig & config)
  : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return { BT::InputPort<double>("wait_duration", 1.0, "Durée") };
  }

  BT::NodeStatus tick() override {
    double d; getInput("wait_duration", d);
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(d * 1000)));
    return BT::NodeStatus::SUCCESS;
  }
};

// ─────────────────────────────────────────────────────────────────────────────
// Nœud SetGoal
// ─────────────────────────────────────────────────────────────────────────────
class SetGoal : public BT::SyncActionNode
{
public:
  SetGoal(const std::string & name, const BT::NodeConfig & config,
          rclcpp::Node::SharedPtr node,
          geometry_msgs::msg::PoseStamped & final_goal,
          std::atomic<int> & pending_goal_floor,
          const std::string & model_name)            // ← AJOUT
  : BT::SyncActionNode(name, config), node_(node),
    final_goal_(final_goal), pending_goal_floor_(pending_goal_floor),
    model_name_(model_name) {}                       // ← AJOUT

  static BT::PortsList providedPorts() {
    return {
      BT::OutputPort<int>("current_floor"),
      BT::OutputPort<int>("goal_floor"),
    };
  }

  BT::NodeStatus tick() override {
    int cur  = getCurrentFloor(model_name_);          // ← AJOUT model_name_
    int goal = pending_goal_floor_.load();
    setOutput("current_floor", cur);
    setOutput("goal_floor",    goal);
    RCLCPP_INFO(node_->get_logger(),
      "[SetGoal] model=%s current=%d goal=%d dest=(%.2f,%.2f)",
      model_name_.c_str(), cur, goal,
      final_goal_.pose.position.x, final_goal_.pose.position.y);
    return BT::NodeStatus::SUCCESS;
  }

private:
  rclcpp::Node::SharedPtr node_;
  geometry_msgs::msg::PoseStamped & final_goal_;
  std::atomic<int> & pending_goal_floor_;
  std::string model_name_;                           // ← AJOUT
};

// ─────────────────────────────────────────────────────────────────────────────
// Nœud SameFloor
// ─────────────────────────────────────────────────────────────────────────────
class SameFloor : public BT::ConditionNode
{
public:
  SameFloor(const std::string & name, const BT::NodeConfig & config,
            rclcpp::Node::SharedPtr node)
  : BT::ConditionNode(name, config), node_(node) {}

  static BT::PortsList providedPorts() {
    return {
      BT::InputPort<int>("current_floor", "Étage actuel"),
      BT::InputPort<int>("goal_floor",    "Étage cible"),
    };
  }

  BT::NodeStatus tick() override {
    int cur, goal;
    getInput("current_floor", cur);
    getInput("goal_floor",    goal);
    bool same = (cur == goal);
    RCLCPP_INFO(node_->get_logger(),
      "[SameFloor] current=%d goal=%d → %s",
      cur, goal, same ? "SUCCESS" : "FAILURE (ascenseur)");
    return same ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
  }

private:
  rclcpp::Node::SharedPtr node_;
};

// ─────────────────────────────────────────────────────────────────────────────
// Nœud IsAtFloor
// ─────────────────────────────────────────────────────────────────────────────
class IsAtFloor : public BT::ConditionNode
{
public:
  IsAtFloor(const std::string & name, const BT::NodeConfig & config,
            rclcpp::Node::SharedPtr node,
            const std::string & model_name)          // ← AJOUT
  : BT::ConditionNode(name, config), node_(node),
    model_name_(model_name) {}                       // ← AJOUT

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<int>("floor",         0,  "Étage attendu"),
        BT::InputPort<int>("current_floor", 0,  "Étage courant (blackboard)")
    };
}

BT::NodeStatus tick() override {
    int expected, current;
    getInput("floor",         expected);
    getInput("current_floor", current);   // ← from blackboard, set by SetGoal
    RCLCPP_INFO(node_->get_logger(),
        "[IsAtFloor] model=%s actuel=%d attendu=%d",
        model_name_.c_str(), current, expected);
    return (current == expected) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
}

private:
  rclcpp::Node::SharedPtr node_;
  std::string model_name_;                           // ← AJOUT
};

// ─────────────────────────────────────────────────────────────────────────────
// Nœud NavigateToGoal
// ─────────────────────────────────────────────────────────────────────────────
class NavigateToGoal : public BT::StatefulActionNode
{
public:
  NavigateToGoal(const std::string & name, const BT::NodeConfig & config,
                 rclcpp::Node::SharedPtr node,
                 geometry_msgs::msg::PoseStamped & final_goal)
  : BT::StatefulActionNode(name, config), node_(node),
    final_goal_(final_goal), done_(false), succeeded_(false) {}

  static BT::PortsList providedPorts() { return {}; }

  BT::NodeStatus onStart() override {
    done_ = false; succeeded_ = false;
    action_client_ = rclcpp_action::create_client<NavigateToPose>(
      node_, "navigate_to_pose_internal");
    if (!action_client_->wait_for_action_server(std::chrono::seconds(5))) {
      RCLCPP_ERROR(node_->get_logger(), "[NavigateToGoal] Nav2 indisponible");
      return BT::NodeStatus::FAILURE;
    }
    NavigateToPose::Goal goal;
    goal.pose                 = final_goal_;
    goal.pose.header.frame_id = "map";
    goal.pose.header.stamp    = node_->now();
    auto opts = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
    opts.result_callback = [this](const GoalHandleNav::WrappedResult & r) {
      succeeded_ = (r.code == rclcpp_action::ResultCode::SUCCEEDED);
      done_ = true;
    };
    action_client_->async_send_goal(goal, opts);
    RCLCPP_INFO(node_->get_logger(), "[NavigateToGoal] → (%.2f, %.2f)",
      final_goal_.pose.position.x, final_goal_.pose.position.y);
    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus onRunning() override {
    return done_ ? (succeeded_ ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE)
                 : BT::NodeStatus::RUNNING;
  }

  void onHalted() override {
    if (action_client_) action_client_->async_cancel_all_goals();
  }

private:
  rclcpp::Node::SharedPtr node_;
  geometry_msgs::msg::PoseStamped & final_goal_;
  rclcpp_action::Client<NavigateToPose>::SharedPtr action_client_;
  std::atomic<bool> done_, succeeded_;
};

// ─────────────────────────────────────────────────────────────────────────────
// Nœud ClearCostmap — namespaced
// ─────────────────────────────────────────────────────────────────────────────
class ClearCostmap : public BT::SyncActionNode
{
public:
  ClearCostmap(const std::string & name, const BT::NodeConfig & config,
               rclcpp::Node::SharedPtr node,
               const std::string & robot_ns)         // ← AJOUT
  : BT::SyncActionNode(name, config), node_(node),
    robot_ns_(robot_ns) {}                           // ← AJOUT

  static BT::PortsList providedPorts() { return {}; }

  BT::NodeStatus tick() override {
    // ← Node temporaire isolé, zéro conflit avec l'executor principal
    auto tmp_node = rclcpp::Node::make_shared("clear_costmap_tmp_" + robot_ns_);
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(tmp_node);

    auto cg = tmp_node->create_client<nav2_msgs::srv::ClearEntireCostmap>(
        "/" + robot_ns_ + "/global_costmap/clear_entirely_global_costmap");
    auto cl = tmp_node->create_client<nav2_msgs::srv::ClearEntireCostmap>(
        "/" + robot_ns_ + "/local_costmap/clear_entirely_local_costmap");

    if (cg->wait_for_service(std::chrono::seconds(2))) {
        auto f = cg->async_send_request(
            std::make_shared<nav2_msgs::srv::ClearEntireCostmap::Request>());
        executor.spin_until_future_complete(f, std::chrono::seconds(2));
    }
    if (cl->wait_for_service(std::chrono::seconds(2))) {
        auto f = cl->async_send_request(
            std::make_shared<nav2_msgs::srv::ClearEntireCostmap::Request>());
        executor.spin_until_future_complete(f, std::chrono::seconds(2));
    }

    RCLCPP_INFO(node_->get_logger(), "[ClearCostmap] Effacé (%s)", robot_ns_.c_str());
    return BT::NodeStatus::SUCCESS;
}

private:
  rclcpp::Node::SharedPtr node_;
  std::string robot_ns_;                             // ← AJOUT
};

// ─────────────────────────────────────────────────────────────────────────────
// Nœud NavigateToElevatorEntry
// ─────────────────────────────────────────────────────────────────────────────
class NavigateToElevatorEntry : public BT::StatefulActionNode
{
public:
  NavigateToElevatorEntry(const std::string & name, const BT::NodeConfig & config,
                          rclcpp::Node::SharedPtr node)
    : BT::StatefulActionNode(name, config), node_(node),
      done_(false), succeeded_(false)
  {
    tf_buffer_   = std::make_shared<tf2_ros::Buffer>(node_->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
  }

  static BT::PortsList providedPorts() {
    return {
      BT::InputPort<double>("x",   3.0,     "X"),
      BT::InputPort<double>("y",  -0.5,     "Y"),
      BT::InputPort<double>("yaw", 3.14159, "Yaw"),
    };
  }

  double getCurrentYaw()
  {
    std::string robot_ns;
    config().blackboard->get("robot_namespace", robot_ns);
    std::string base_frame = robot_ns.empty() ? "base_link" : robot_ns + "/base_link";

    try {
      auto tf = tf_buffer_->lookupTransform("map", base_frame, tf2::TimePointZero);
      tf2::Quaternion q(
        tf.transform.rotation.x, tf.transform.rotation.y,
        tf.transform.rotation.z, tf.transform.rotation.w);
      double roll, pitch, yaw;
      tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);
      return yaw;
    } catch (const tf2::TransformException & e) {
      RCLCPP_WARN(node_->get_logger(),
        "[NavToElevator] TF '%s' indispo: %s — yaw=0 par défaut",
        base_frame.c_str(), e.what());
      return 0.0;
    }
  }

  BT::NodeStatus onStart() override
  {
    done_ = false; succeeded_ = false;

    double x, y, yaw;
    getInput("x", x);
    getInput("y", y);
    getInput("yaw", yaw);

    double current_yaw   = getCurrentYaw();
    double error         = std::atan2(std::sin(yaw - current_yaw),
                                      std::cos(yaw - current_yaw));
    double corrected_yaw = current_yaw + error;

    RCLCPP_INFO(node_->get_logger(),
      "[NavToElevator] target=(%.2f,%.2f) "
      "yaw_actuel=%.2f yaw_cible=%.2f yaw_corrigé=%.2f",
      x, y, current_yaw, yaw, corrected_yaw);

    action_client_ = rclcpp_action::create_client<NavigateToPose>(
      node_, "navigate_to_pose_internal");

    if (!action_client_->wait_for_action_server(std::chrono::seconds(5))) {
      RCLCPP_ERROR(node_->get_logger(), "[NavToElevator] serveur indisponible");
      return BT::NodeStatus::FAILURE;
    }

    NavigateToPose::Goal goal;
    goal.pose.header.frame_id = "map";
    goal.pose.header.stamp    = node_->now();
    goal.pose.pose.position.x = x;
    goal.pose.pose.position.y = y;

    tf2::Quaternion q;
    q.setRPY(0, 0, corrected_yaw);
    goal.pose.pose.orientation = tf2::toMsg(q);

    auto opts = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
    opts.result_callback = [this](const GoalHandleNav::WrappedResult & r) {
      succeeded_ = (r.code == rclcpp_action::ResultCode::SUCCEEDED);
      done_ = true;
    };

    action_client_->async_send_goal(goal, opts);
    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus onRunning() override {
    return done_ ? (succeeded_ ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE)
                 : BT::NodeStatus::RUNNING;
  }

  void onHalted() override {
    if (action_client_) action_client_->async_cancel_all_goals();
  }

private:
  rclcpp::Node::SharedPtr                          node_;
  rclcpp_action::Client<NavigateToPose>::SharedPtr action_client_;
  std::atomic<bool>                                done_, succeeded_;
  std::shared_ptr<tf2_ros::Buffer>                 tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener>      tf_listener_;
};

// ─────────────────────────────────────────────────────────────────────────────
// Nœud ChangeMap
// ─────────────────────────────────────────────────────────────────────────────
class ChangeMap : public BT::SyncActionNode
{
public:
  ChangeMap(const std::string & name, const BT::NodeConfig & config,
            rclcpp::Node::SharedPtr node)
  : BT::SyncActionNode(name, config), node_(node)
  {
    ns_ = node_->get_namespace();
    if (!ns_.empty() && ns_.back() == '/') ns_.pop_back();

    // ✅ Plus de cbg ni executor_ dans le constructeur
    std::string svc = "/change_map";
    RCLCPP_INFO(node_->get_logger(), "[ChangeMap] service cible: '%s'", svc.c_str());

    // Client sans cbg spécifique — sera recréé dans tick()
    floor_pub_ = node_->create_publisher<std_msgs::msg::Int32>(
      ns_ + "/current_floor", rclcpp::QoS(1).transient_local());

    active_floor_pub_ = node_->create_publisher<std_msgs::msg::Int32>(
      "/active_floor", rclcpp::QoS(1).transient_local());

    std_msgs::msg::Int32 init_msg;
    init_msg.data = 0;
    floor_pub_->publish(init_msg);
  }

  static BT::PortsList providedPorts() {
    return { BT::InputPort<int>("floor", 1, "Étage") };
  }

  BT::NodeStatus tick() override
{
    int floor;
    getInput("floor", floor);

    // ✅ Nom unique + namespace hérité du node principal
    std::string tmp_name = "change_map_tmp_" + std::to_string(floor) 
                           + "_" + std::to_string(std::rand());

    auto options = rclcpp::NodeOptions();
    options.arguments({
        "--ros-args",
        "-r", "__ns:=" + ns_,   // ← injecter le namespace de robot1/robot2
        "-r", "__node:=" + tmp_name
    });

    auto tmp_node = rclcpp::Node::make_shared(tmp_name, options);
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(tmp_node);

    // Maintenant tmp_node est dans /robot2/ → trouve /robot2/change_map
    auto client = tmp_node->create_client<multi_floor_nav::srv::IntTrigger>(
        "change_map");   // relatif → /robot2/change_map ✓

    if (!client->wait_for_service(std::chrono::seconds(5))) {
        RCLCPP_ERROR(node_->get_logger(), 
            "[ChangeMap] service indisponible: %s/change_map", ns_.c_str());
        return BT::NodeStatus::FAILURE;
    }

    auto req = std::make_shared<multi_floor_nav::srv::IntTrigger::Request>();
    req->req_int = floor;
    auto future = client->async_send_request(req);

    if (executor.spin_until_future_complete(future, std::chrono::seconds(5))
        != rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR(node_->get_logger(), 
            "[ChangeMap] timeout floor=%d", floor);
        return BT::NodeStatus::FAILURE;
    }

    if (!future.get()->success) {
        RCLCPP_ERROR(node_->get_logger(), 
            "[ChangeMap] refusé floor=%d", floor);
        return BT::NodeStatus::FAILURE;
    }

    // Attendre la carte sur /robot2/map
    bool map_received = false;
    auto map_sub = tmp_node->create_subscription<nav_msgs::msg::OccupancyGrid>(
        "map",   // relatif → /robot2/map ✓
        rclcpp::QoS(1).transient_local(),
        [&](const nav_msgs::msg::OccupancyGrid::SharedPtr) {
            map_received = true;
        });

    auto deadline = tmp_node->now() + rclcpp::Duration::from_seconds(10.0);
    while (!map_received && tmp_node->now() < deadline && rclcpp::ok()) {
        executor.spin_some(std::chrono::milliseconds(100));
    }

    if (!map_received) {
        RCLCPP_ERROR(node_->get_logger(), 
            "[ChangeMap] carte jamais reçue floor=%d", floor);
        return BT::NodeStatus::FAILURE;
    }

    config().blackboard->set("current_floor", floor);
    // Après map_received = true, avant return SUCCESS
    // Clear costmap global
    auto clear_global = tmp_node->create_client<nav2_msgs::srv::ClearEntireCostmap>(
        "global_costmap/clear_entirely_global_costmap");
    auto clear_local = tmp_node->create_client<nav2_msgs::srv::ClearEntireCostmap>(
        "local_costmap/clear_entirely_local_costmap");

    if (clear_global->wait_for_service(std::chrono::seconds(2))) {
        auto f = clear_global->async_send_request(
            std::make_shared<nav2_msgs::srv::ClearEntireCostmap::Request>());
        executor.spin_until_future_complete(f, std::chrono::seconds(2));
    }
    if (clear_local->wait_for_service(std::chrono::seconds(2))) {
        auto f = clear_local->async_send_request(
            std::make_shared<nav2_msgs::srv::ClearEntireCostmap::Request>());
        executor.spin_until_future_complete(f, std::chrono::seconds(2));
    }

RCLCPP_INFO(node_->get_logger(), "[ChangeMap] costmaps effacés ✓");
    std_msgs::msg::Int32 msg;
    msg.data = floor;
    floor_pub_->publish(msg);
    active_floor_pub_->publish(msg);

    RCLCPP_INFO(node_->get_logger(), 
        "[ChangeMap] carte %d prête (%s/map)", floor, ns_.c_str());
    return BT::NodeStatus::SUCCESS;
}

private:
  rclcpp::Node::SharedPtr node_;
  std::string ns_;
  // ✅ Plus de cbg_client_, cbg_sub_, executor_, client_ membres
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr floor_pub_;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr active_floor_pub_;
};
// ─────────────────────────────────────────────────────────────────────────────
// Nœud Relocalize — namespaced
// ─────────────────────────────────────────────────────────────────────────────
class Relocalize : public BT::SyncActionNode
{
public:
  Relocalize(const std::string & name, const BT::NodeConfig & config,
             rclcpp::Node::SharedPtr node,
             const std::string & robot_ns)           // ← AJOUT
  : BT::SyncActionNode(name, config), node_(node),
    robot_ns_(robot_ns)                              // ← AJOUT
  {
    using PoseWithCov = geometry_msgs::msg::PoseWithCovarianceStamped;
    // ← Topic avec namespace : /robot1/initialpose ou /robot2/initialpose
    initialpose_pub_ = node_->create_publisher<PoseWithCov>(
      "/" + robot_ns_ + "/initialpose", 10);
  }

  static BT::PortsList providedPorts() {
    return {
      BT::InputPort<double>("x",   0.0,  "X initial"),
      BT::InputPort<double>("y",  -0.5,  "Y initial"),
      BT::InputPort<double>("yaw", 0.0,  "Yaw initial"),
    };
  }

  BT::NodeStatus tick() override {
    double x, y, yaw;
    getInput("x", x); getInput("y", y); getInput("yaw", yaw);
    RCLCPP_INFO(node_->get_logger(),
      "[Relocalize] ns=%s pose=(%.2f, %.2f) yaw=%.2f",
      robot_ns_.c_str(), x, y, yaw);

    std::thread([]() {
      system("pkill -f 'static_transform_publisher' 2>/dev/null || true");
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
      system("ros2 run tf2_ros static_transform_publisher "
             "--x 0.0 --y 0.0 --z 0.0 "
             "--roll 0 --pitch 0 --yaw 0 "
             "--frame-id map --child-frame-id odom &");
    }).detach();

    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    int wait = 0;
    while (initialpose_pub_->get_subscription_count() == 0
           && rclcpp::ok() && wait < 20) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      wait++;
    }

    tf2::Quaternion q; q.setRPY(0, 0, yaw);
    geometry_msgs::msg::PoseWithCovarianceStamped pose;
    pose.header.frame_id       = "map";
    pose.header.stamp          = node_->now();
    pose.pose.pose.position.x  = x;
    pose.pose.pose.position.y  = y;
    pose.pose.pose.position.z  = 0.0;
    pose.pose.pose.orientation = tf2::toMsg(q);
    pose.pose.covariance[0]    = 0.25;
    pose.pose.covariance[7]    = 0.25;
    pose.pose.covariance[35]   = 0.07;
    initialpose_pub_->publish(pose);

    RCLCPP_INFO(node_->get_logger(),
      "[Relocalize] pose envoyée sur /%s/initialpose: (%.2f, %.2f) yaw=%.2f",
      robot_ns_.c_str(), x, y, yaw);
    return BT::NodeStatus::SUCCESS;
  }

private:
  rclcpp::Node::SharedPtr node_;
  std::string robot_ns_;                             // ← AJOUT
  rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr initialpose_pub_;
};

// ─────────────────────────────────────────────────────────────────────────────
// Nœud WaitElevatorReady
// ─────────────────────────────────────────────────────────────────────────────
class WaitElevatorReady : public BT::StatefulActionNode
{
public:
  WaitElevatorReady(const std::string & name, const BT::NodeConfig & config,
                    rclcpp::Node::SharedPtr node)
    : BT::StatefulActionNode(name, config), node_(node), ready_(false) {}

  static BT::PortsList providedPorts() {
    return {
      BT::InputPort<std::string>("robot_id"),
      BT::InputPort<std::string>("topic"),
      BT::InputPort<std::string>("expected_floor"),
      BT::InputPort<double>("timeout")
    };
  }

  BT::NodeStatus onStart() override {
    ready_ = false;

    std::string robot_id, expected_floor;
    getInput("robot_id",       robot_id);
    getInput("expected_floor", expected_floor);
    expected_floor_ = expected_floor;

    if (robot_id.empty() || robot_id.find('{') != std::string::npos) {
      RCLCPP_ERROR(node_->get_logger(),
        "[WaitElevatorReady] ❌ robot_id non résolu : '%s'", robot_id.c_str());
      return BT::NodeStatus::FAILURE;
    }

    topic_ = "/" + robot_id + "/elevator/ready";

    RCLCPP_INFO(node_->get_logger(),
      "[WaitElevatorReady] onStart | topic=%s floor=%s",
      topic_.c_str(), expected_floor_.c_str());

    // ✅ Utiliser directement node_ — pas de node séparé
    auto qos = rclcpp::QoS(1)
    .reliability(rclcpp::ReliabilityPolicy::Reliable)
    .durability(rclcpp::DurabilityPolicy::TransientLocal);

    sub_ = node_->create_subscription<std_msgs::msg::String>(
        topic_,
        qos,
        [this](std_msgs::msg::String::SharedPtr msg) {
          auto elapsed = std::chrono::duration<double>(
              std::chrono::steady_clock::now() - start_time_).count();
          
          if (msg->data == expected_floor_) {
              ready_ = true;
          }
      });

    RCLCPP_INFO(node_->get_logger(),
      "[WaitElevatorReady] subscriber créé sur %s", topic_.c_str());

    start_time_ = std::chrono::steady_clock::now();
    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus onRunning() override {
    if (ready_) {
      RCLCPP_INFO(node_->get_logger(),
        "[WaitElevatorReady] ✅ SUCCESS étage=%s", expected_floor_.c_str());
      return BT::NodeStatus::SUCCESS;
    }

    double timeout = 30.0;
    getInput("timeout", timeout);
    auto elapsed = std::chrono::steady_clock::now() - start_time_;
    if (std::chrono::duration<double>(elapsed).count() > timeout) {
      RCLCPP_WARN(node_->get_logger(),
        "[WaitElevatorReady] ⏱ Timeout après %.1fs sur %s",
        timeout, topic_.c_str());
      return BT::NodeStatus::FAILURE;
    }
    return BT::NodeStatus::RUNNING;
  }

  void onHalted() override {
    sub_.reset();
    ready_ = false;
  }

private:
  rclcpp::Node::SharedPtr                                node_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
  std::string                                            expected_floor_;
  std::string                                            topic_;
  std::chrono::steady_clock::time_point                  start_time_;
  std::atomic<bool>                                      ready_;
};
// ─────────────────────────────────────────────────────────────────────────────
// ElevatorBTExecutor — multi-robot
// ─────────────────────────────────────────────────────────────────────────────
class ElevatorBTExecutor : public rclcpp::Node
{
public:
  ElevatorBTExecutor()
  : rclcpp::Node("elevator_bt_executor"),
    bt_running_(false), pending_goal_floor_(0)
  {
    this->declare_parameter("bt_xml_path", "");
    this->declare_parameter("robot_namespace", "robot1");

    robot_ns_   = this->get_parameter("robot_namespace").as_string();
    model_name_ = (robot_ns_ == "robot2") ? "jackal2" : "jackal";
        odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
        "/" + robot_ns_ + "/odometry/filtered", 10,
        [this](const nav_msgs::msg::Odometry::SharedPtr msg) {
            double vx = msg->twist.twist.linear.x;
            double vy = msg->twist.twist.linear.y;
            double vw = msg->twist.twist.angular.z;
            moving_flag_ = (std::sqrt(vx*vx + vy*vy) > 0.01 ||
                            std::abs(vw) > 0.01);
        });
    RCLCPP_INFO(this->get_logger(),
        "[BT] Subscriber odom → /%s/odometry/filtered", robot_ns_.c_str());

    RCLCPP_INFO(this->get_logger(),
      "[BT] Démarrage — namespace=%s model_gazebo=%s",
      robot_ns_.c_str(), model_name_.c_str());

    // Subscription à l'étage cible avec namespace
    floor_sub_ = this->create_subscription<std_msgs::msg::Int32>(
  "/" + robot_ns_ + "/goal_floor", 10,
  [this](const std_msgs::msg::Int32::SharedPtr msg) {
    pending_goal_floor_ = msg->data;  // ← stocker dans le membre
    if (blackboard_) {
      blackboard_->set("goal_floor", msg->data);
    }
    RCLCPP_INFO(this->get_logger(), "Étage cible → %d", msg->data);
  });

// 2. subscriber goal pose
floor_goal_sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
  "/" + robot_ns_ + "/goal_pose", 10,
  [this](const geometry_msgs::msg::PoseStamped::SharedPtr msg) {
    if (blackboard_) {
      blackboard_->set("goal_pose", *msg);
    }
    if (tree_.rootNode()) {
      tree_.haltTree();
    }
    RCLCPP_INFO(this->get_logger(), "Nouveau goal pose reçu");
  });

    // Action server avec file d'attente pour replanning
    action_server_ = rclcpp_action::create_server<NavigateToPose>(
      this,
      "navigate_to_pose",
      [this](const rclcpp_action::GoalUUID &,
             std::shared_ptr<const NavigateToPose::Goal> goal)
        -> rclcpp_action::GoalResponse
      {
        geometry_msgs::msg::PoseStamped new_goal = goal->pose;
        new_goal.header.frame_id = "map";
        new_goal.header.stamp = this->now();

        if (bt_running_) {
          // Mettre en file d'attente au lieu de rejeter
          std::lock_guard<std::mutex> lk(queue_mutex_);
          goal_queue_.push(new_goal);
          RCLCPP_WARN(this->get_logger(),
            "[BT] En cours — goal mis en file (%zu en attente)",
            goal_queue_.size());
          return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }

        final_goal_ = new_goal;
        RCLCPP_INFO(this->get_logger(),
          "[BT] Goal intercepté: (%.2f, %.2f) étage_cible=%d",
          final_goal_.pose.position.x,
          final_goal_.pose.position.y,
          pending_goal_floor_.load());
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
      },
      [this](std::shared_ptr<ServerGoalHandle>)
        -> rclcpp_action::CancelResponse
      {
        return rclcpp_action::CancelResponse::ACCEPT;
      },
      [this](std::shared_ptr<ServerGoalHandle> goal_handle)
      {
        current_server_goal_handle_ = goal_handle;
        auto feedback = std::make_shared<NavigateToPose::Feedback>();
        goal_handle->publish_feedback(feedback);
        std::thread([this]() { run_behavior_tree(); }).detach();
      });

    RCLCPP_INFO(this->get_logger(),
      "[BT] Prêt — namespace=%s\n"
      "  1) ros2 topic pub --once %s std_msgs/msg/Int32 '{data: 1}'\n"
      "  2) Envoyer goal via action %s/navigate_to_pose",
      robot_ns_.c_str(), ("/" + robot_ns_ + "/goal_pose").c_str(), robot_ns_.c_str());
  }

private:
  rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr floor_goal_sub_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr floor_sub_;
  BT::Blackboard::Ptr blackboard_;
  BT::Tree tree_;

  class IsRobot : public BT::ConditionNode
{
public:
  IsRobot(const std::string & name, const BT::NodeConfig & config,
          rclcpp::Node::SharedPtr node)
  : BT::ConditionNode(name, config), node_(node) {}

  static BT::PortsList providedPorts() {
    return {
      BT::InputPort<std::string>("robot_id", "", "robot1 ou robot2"),
    };
  }

  BT::NodeStatus tick() override
{
  std::string expected;
  getInput("robot_id", expected);

  // ✅ robot_namespace et non robot_ns
  std::string actual;
  config().blackboard->get("robot_namespace", actual);

  // Nettoyer le "/" si besoin
  if (!actual.empty() && actual[0] == '/') {
    actual = actual.substr(1);
  }

  bool match = (actual == expected);
  RCLCPP_INFO(node_->get_logger(),
    "[IsRobot] blackboard='%s' expected='%s' → %s",
    actual.c_str(), expected.c_str(), match ? "SUCCESS" : "FAILURE");

  return match ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
}

private:
  rclcpp::Node::SharedPtr node_;
};
  void run_behavior_tree()
  {
    bt_running_ = true;
    auto node_ptr = this->shared_from_this();

    std::string bt_xml_path = this->get_parameter("bt_xml_path").as_string();
    if (bt_xml_path.empty()) {
      bt_xml_path = ament_index_cpp::get_package_share_directory("multi_floor_nav")
                    + "/behavior_trees/elevator_navigation.xml";
                    
    }

    BT::BehaviorTreeFactory factory;

    factory.registerNodeType<Wait>("Wait");

    factory.registerBuilder<SetGoal>("SetGoal",
      [node_ptr, this](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<SetGoal>(n, c, node_ptr,
          final_goal_, pending_goal_floor_, model_name_);
      });

    factory.registerBuilder<SameFloor>("SameFloor",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<SameFloor>(n, c, node_ptr);
      });

    factory.registerBuilder<IsAtFloor>("IsAtFloor",
      [node_ptr, this](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<IsAtFloor>(n, c, node_ptr, model_name_);
      });

    factory.registerBuilder<NavigateToGoal>("NavigateToGoal",
      [node_ptr, this](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<NavigateToGoal>(n, c, node_ptr, final_goal_);
      });

    factory.registerBuilder<ClearCostmap>("ClearCostmap",
      [node_ptr, this](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<ClearCostmap>(n, c, node_ptr, robot_ns_);
      });

    factory.registerBuilder<NavigateToElevatorEntry>("NavigateToElevatorEntry",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<NavigateToElevatorEntry>(n, c, node_ptr);
      });

    factory.registerBuilder<OpenElevatorDoor>("OpenElevatorDoor",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<OpenElevatorDoor>(n, c, node_ptr);
      });

    factory.registerBuilder<CloseElevatorDoor>("CloseElevatorDoor",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<CloseElevatorDoor>(n, c, node_ptr);
      });

    factory.registerBuilder<EnterElevator>("EnterElevator",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<EnterElevator>(n, c, node_ptr);
      });

    factory.registerBuilder<ExitElevator>("ExitElevator",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<ExitElevator>(n, c, node_ptr);
      });

    factory.registerBuilder<CallElevatorMQTT>("CallElevatorMQTT",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<CallElevatorMQTT>(n, c, node_ptr);
      });

    factory.registerBuilder<WaitElevatorMQTT>("WaitElevatorMQTT",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<WaitElevatorMQTT>(n, c, node_ptr);
      });

    factory.registerBuilder<WaitElevatorReady>("WaitElevatorReady",
    [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<WaitElevatorReady>(n, c, node_ptr);  // ← add node_ptr
    });
    factory.registerBuilder<WaitElevatorGrant>("WaitElevatorGrant",
    [node_ptr](const std::string & n, const BT::NodeConfig & c) {
      return std::make_unique<WaitElevatorGrant>(n, c, node_ptr);
    });

    factory.registerBuilder<ChangeMap>("ChangeMap",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<ChangeMap>(n, c, node_ptr);
      });

    factory.registerBuilder<Relocalize>("Relocalize",
      [node_ptr, this](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<Relocalize>(n, c, node_ptr, robot_ns_);
      });

    factory.registerBuilder<CancelNavigation>("CancelNavigation",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<CancelNavigation>(n, c, node_ptr);
      });

    factory.registerBuilder<SmartSpinAction>("SmartSpin",
      [node_ptr](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<SmartSpinAction>(n, c, node_ptr);
      });
    factory.registerBuilder<PublishFloorStatus>("PublishFloorStatus",
     [this](const std::string & n, const BT::NodeConfig & c) {
        return std::make_unique<PublishFloorStatus>(
            n, c, shared_from_this(), robot_ns_, moving_flag_);
     });
     factory.registerBuilder<IsRobot>("IsRobot",
  [node_ptr](const std::string & n, const BT::NodeConfig & c) {
    return std::make_unique<IsRobot>(n, c, node_ptr);
  });

    BT::Blackboard::Ptr bb = BT::Blackboard::create();
    int real_floor = getCurrentFloor(model_name_);
    bb->set("current_floor", real_floor);
    bb->set("goal_floor", pending_goal_floor_.load());
    bb->set("robot_namespace", robot_ns_);
    bb->set("door_cmd_topic", "/elevator/door_joint/cmd");

    bb->set("goal_floor", pending_goal_floor_.load());
    blackboard_ = bb;  // ← stocker la référence pour le subscriber
    RCLCPP_INFO(this->get_logger(),
      "[BT] Démarrage — goal_floor=%d", pending_goal_floor_.load());
    auto tree = factory.createTreeFromFile(bt_xml_path, bb);
    BT::StdCoutLogger logger(tree);

    RCLCPP_INFO(this->get_logger(),
      "[BT] Démarré — model=%s current=%d goal=%d dest=(%.2f,%.2f)",
      model_name_.c_str(),
      getCurrentFloor(model_name_),
      pending_goal_floor_.load(),
      final_goal_.pose.position.x, final_goal_.pose.position.y);

    BT::NodeStatus status = BT::NodeStatus::RUNNING;
    while (status == BT::NodeStatus::RUNNING && rclcpp::ok()) {
      status = tree.tickOnce();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Vérifier s'il y a un goal en file d'attente
    geometry_msgs::msg::PoseStamped next_goal;
    bool has_next = false;
    {
      std::lock_guard<std::mutex> lk(queue_mutex_);
      if (!goal_queue_.empty()) {
        next_goal = goal_queue_.front();
        goal_queue_.pop();
        has_next = true;
      }
    }

    if (has_next) {
      final_goal_ = next_goal;
      RCLCPP_INFO(this->get_logger(),
        "[BT] Goal suivant dans la file: (%.2f, %.2f)",
        final_goal_.pose.position.x, final_goal_.pose.position.y);
      bt_running_ = false;  // Permet au prochain goal de démarrer
    } else if (current_server_goal_handle_) {
      auto result = std::make_shared<NavigateToPose::Result>();
      if (status == BT::NodeStatus::SUCCESS) {
        current_server_goal_handle_->succeed(result);
        RCLCPP_INFO(this->get_logger(), "[BT] ✅ Mission accomplie !");
      } else {
        current_server_goal_handle_->abort(result);
        RCLCPP_ERROR(this->get_logger(), "[BT] ❌ Mission échouée !");
      }
      current_server_goal_handle_.reset();
    }

    bt_running_ = false;
  }
  std::atomic<bool> moving_flag_{false};
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;

  // ── Membres ─────────────────────────────────────────────────────────────────

  rclcpp_action::Server<NavigateToPose>::SharedPtr      action_server_;
  std::shared_ptr<ServerGoalHandle>                     current_server_goal_handle_;
  geometry_msgs::msg::PoseStamped                       final_goal_;
  std::atomic<bool>                                     bt_running_;
  std::atomic<int>                                      pending_goal_floor_;
  std::string                                           robot_ns_;
  std::string                                           model_name_;
  std::queue<geometry_msgs::msg::PoseStamped>           goal_queue_;
  std::mutex                                            queue_mutex_;
};
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ElevatorBTExecutor>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}