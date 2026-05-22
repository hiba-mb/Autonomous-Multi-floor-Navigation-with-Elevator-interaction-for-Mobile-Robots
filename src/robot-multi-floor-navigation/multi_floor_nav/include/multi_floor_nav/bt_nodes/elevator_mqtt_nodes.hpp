#pragma once

#include <behaviortree_cpp/action_node.h>
#include <rclcpp/rclcpp.hpp>
#include <mqtt/async_client.h>
#include <string>
#include <chrono>
#include <atomic>

// ─────────────────────────────────────────────
// CallElevatorMQTT : publie "floor=X" sur MQTT
// pour commander la montée de l'ascenseur Gazebo
// ─────────────────────────────────────────────
class CallElevatorMQTT : public BT::SyncActionNode
{
public:
  CallElevatorMQTT(const std::string & name, const BT::NodeConfig & config,
                   rclcpp::Node::SharedPtr node)
  : BT::SyncActionNode(name, config), node_(node)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<std::string>("broker", "localhost", "Adresse du broker MQTT"),
      BT::InputPort<int>("port", 1883, "Port MQTT"),
      BT::InputPort<std::string>("topic", "elevator/request", "Topic MQTT de commande"),
      BT::InputPort<std::string>("floor", "1", "Étage cible"),
      BT::InputPort<std::string>("robot_id", "auto", "Robot demandeur")  // ← AJOUTER CETTE LIGNE
    };
  }

  BT::NodeStatus tick() override
  {
    std::string broker, topic, floor, robot_id;
    int port;
    getInput("broker", broker);
    getInput("port", port);
    getInput("topic", topic);
    getInput("floor", floor);
    getInput("robot_id", robot_id);  // ← LIRE LE PORT

    // Construire le payload avec robot_id
    std::string payload = "{\"floor\": \"" + floor + "\", \"robot_id\": \"" + robot_id + "\"}";
    
    std::string server_uri = "tcp://" + broker + ":" + std::to_string(port);

    try {
      std::string client_id = "bt_caller_" + robot_id + "_" + std::to_string(
      std::chrono::steady_clock::now().time_since_epoch().count() % 100000);
      mqtt::async_client client(server_uri, client_id);
      auto conn_opts = mqtt::connect_options_builder()
        .clean_session(true)
        .finalize();

      client.connect(conn_opts)->wait();
      client.publish(topic, payload.c_str(), payload.size(), 1, false)->wait();
      client.disconnect()->wait();

      RCLCPP_INFO(node_->get_logger(),
        "[CallElevatorMQTT] Publié '%s' sur '%s' (robot_id=%s)", 
        payload.c_str(), topic.c_str(), robot_id.c_str());
      return BT::NodeStatus::SUCCESS;
    } catch (const mqtt::exception & e) {
      RCLCPP_ERROR(node_->get_logger(),
        "[CallElevatorMQTT] Erreur MQTT: %s", e.what());
      return BT::NodeStatus::FAILURE;
    }
  }

private:
  rclcpp::Node::SharedPtr node_;
};

class WaitElevatorGrant : public BT::StatefulActionNode
{
public:
  WaitElevatorGrant(const std::string & name, const BT::NodeConfig & config,
                    rclcpp::Node::SharedPtr node)
    : BT::StatefulActionNode(name, config), node_(node), granted_(false) {}

  static BT::PortsList providedPorts() {
    return {
      BT::InputPort<std::string>("robot_id"),
      BT::InputPort<double>("timeout", 120.0, "Timeout en secondes")
    };
  }

  BT::NodeStatus onStart() override {
    granted_ = false;
    std::string robot_id;
    getInput("robot_id", robot_id);

    topic_ = "elevator/grant/" + robot_id;
    start_time_ = std::chrono::steady_clock::now();

    std::string broker = "localhost";
    int port = 1883;
    std::string client_id = "bt_grant_" + robot_id + "_" +
      std::to_string(std::chrono::steady_clock::now().time_since_epoch().count() % 100000);

    try {
      client_ = std::make_shared<mqtt::async_client>("tcp://" + broker + ":" + std::to_string(port), client_id);
      client_->set_message_callback([this](mqtt::const_message_ptr msg) {
        granted_ = true;
        RCLCPP_INFO(node_->get_logger(), "[WaitElevatorGrant] ✅ Grant reçu sur %s", topic_.c_str());
      });
      auto conn_opts = mqtt::connect_options_builder().clean_session(true).finalize();
      client_->connect(conn_opts)->wait();
      client_->subscribe(topic_, 1)->wait();
      RCLCPP_INFO(node_->get_logger(), "[WaitElevatorGrant] Attente grant sur %s", topic_.c_str());
    } catch (const mqtt::exception & e) {
      RCLCPP_ERROR(node_->get_logger(), "[WaitElevatorGrant] Erreur MQTT: %s", e.what());
      return BT::NodeStatus::FAILURE;
    }
    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus onRunning() override {
    if (granted_) {
      if (client_) { try { client_->disconnect()->wait(); } catch (...) {} }
      return BT::NodeStatus::SUCCESS;
    }
    double timeout = 120.0;
    getInput("timeout", timeout);
    auto elapsed = std::chrono::duration<double>(
      std::chrono::steady_clock::now() - start_time_).count();
    if (elapsed > timeout) {
      RCLCPP_WARN(node_->get_logger(), "[WaitElevatorGrant] Timeout %.1fs", timeout);
      if (client_) { try { client_->disconnect()->wait(); } catch (...) {} }
      return BT::NodeStatus::FAILURE;
    }
    return BT::NodeStatus::RUNNING;
  }

  void onHalted() override {
    if (client_) { try { client_->disconnect()->wait(); } catch (...) {} }
    granted_ = false;
  }

private:
  rclcpp::Node::SharedPtr node_;
  std::shared_ptr<mqtt::async_client> client_;
  std::string topic_;
  std::chrono::steady_clock::time_point start_time_;
  std::atomic<bool> granted_;
};

// ─────────────────────────────────────────────
// WaitElevatorMQTT : attend le statut de l'ascenseur
// (ex: "arrived:1") ou utilise un délai fixe
// ─────────────────────────────────────────────
class WaitElevatorMQTT : public BT::StatefulActionNode
{
public:
  WaitElevatorMQTT(const std::string & name, const BT::NodeConfig & config,
                   rclcpp::Node::SharedPtr node)
  : BT::StatefulActionNode(name, config), node_(node), arrived_(false)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<std::string>("broker", "localhost", "Adresse du broker MQTT"),
      BT::InputPort<int>("port", 1883, "Port MQTT"),
      BT::InputPort<std::string>("topic", "elevator/status", "Topic MQTT de statut"),
      BT::InputPort<std::string>("expected_floor", "1", "Étage attendu"),
      BT::InputPort<double>("timeout", 15.0, "Timeout en secondes")
    };
  }

  BT::NodeStatus onStart() override
  {
    std::string broker, topic, expected_floor;
    int port;
    getInput("broker", broker);
    getInput("port", port);
    getInput("topic", topic);
    getInput("expected_floor", expected_floor);
    getInput("timeout", timeout_);

    expected_floor_ = expected_floor;
    arrived_ = false;
    start_time_ = node_->now();

    std::string server_uri = "tcp://" + broker + ":" + std::to_string(port);

    try {
      client_ = std::make_shared<mqtt::async_client>(server_uri, "elevator_bt_waiter");
      auto conn_opts = mqtt::connect_options_builder()
        .clean_session(true)
        .finalize();

      client_->set_message_callback([this](mqtt::const_message_ptr msg) {
        std::string payload = msg->get_payload_str();
        RCLCPP_INFO(node_->get_logger(),
          "[WaitElevatorMQTT] Reçu: %s", payload.c_str());
        if (payload == "arrived:" + expected_floor_) {
          arrived_ = true;
        }
      });

      client_->connect(conn_opts)->wait();
      client_->subscribe(topic, 1)->wait();
      std::string status_request = "{\"action\": \"get_status\"}";
      client_->publish("elevator/status/request", status_request.c_str(), 
                     status_request.size(), 1, false)->wait();
      RCLCPP_INFO(node_->get_logger(),
        "[WaitElevatorMQTT] En attente d'arrivée à l'étage %s", expected_floor_.c_str());
    } catch (const mqtt::exception & e) {
      RCLCPP_WARN(node_->get_logger(),
        "[WaitElevatorMQTT] MQTT non disponible, utilisation du timeout fixe: %s", e.what());
    }

    return BT::NodeStatus::RUNNING;
  }

  BT::NodeStatus onRunning() override
  {
    // Vérifier si confirmation MQTT reçue
    if (arrived_) {
      cleanup();
      RCLCPP_INFO(node_->get_logger(), "[WaitElevatorMQTT] Ascenseur arrivé !");
      return BT::NodeStatus::SUCCESS;
    }

    // Vérifier timeout
    double elapsed = (node_->now() - start_time_).seconds();
    if (elapsed >= timeout_) {
      cleanup();
      RCLCPP_WARN(node_->get_logger(),
        "[WaitElevatorMQTT] Timeout (%.1fs) — on suppose que l'ascenseur est arrivé", timeout_);
      return BT::NodeStatus::SUCCESS; // On continue malgré le timeout
    }

    return BT::NodeStatus::RUNNING;
  }

  void onHalted() override { cleanup(); }

private:
  void cleanup()
  {
    if (client_ && client_->is_connected()) {
      try { client_->disconnect()->wait(); } catch (...) {}
    }
  }

  rclcpp::Node::SharedPtr node_;
  std::shared_ptr<mqtt::async_client> client_;
  std::string expected_floor_;
  double timeout_;
  rclcpp::Time start_time_;
  std::atomic<bool> arrived_;
};
