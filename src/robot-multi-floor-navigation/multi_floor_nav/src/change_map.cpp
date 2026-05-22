#include "multi_floor_nav/change_map.hpp"

ChangeMap::ChangeMap() : Node("change_map_node")
{
    // Récupérer le namespace proprement
    ns_ = this->get_namespace();
    if (!ns_.empty() && ns_[0] == '/') ns_ = ns_.substr(1);  // "robot2"

    map_pub_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>(
        "map", rclcpp::QoS(1).transient_local());

    map_level_0_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
        "/map_level_0", rclcpp::QoS(1).transient_local(),
        std::bind(&ChangeMap::mapLevelZeroCallback, this, std::placeholders::_1));

    map_level_1_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
        "/map_level_1", rclcpp::QoS(1).transient_local(),
        std::bind(&ChangeMap::mapLevelOneCallback, this, std::placeholders::_1));

    change_map_server_ = this->create_service<multi_floor_nav::srv::IntTrigger>(
        "change_map",
        std::bind(&ChangeMap::changeMapCallback, this, 
                  std::placeholders::_1, std::placeholders::_2));

    RCLCPP_INFO(this->get_logger(), "ChangeMap node started, namespace: %s", ns_.c_str());
}

void ChangeMap::mapLevelZeroCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
{
    map_level_0_ = *msg;
    map_level_0_.header.frame_id = ns_ + "/map";  // ✅ fixer dès la réception
}

void ChangeMap::mapLevelOneCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
{
    map_level_1_ = *msg;
    map_level_1_.header.frame_id = ns_ + "/map";  // ✅ fixer dès la réception
}

void ChangeMap::changeMapCallback(
    const std::shared_ptr<multi_floor_nav::srv::IntTrigger::Request> req,
    std::shared_ptr<multi_floor_nav::srv::IntTrigger::Response> res)
{
    if (req->req_int == 0) {
        map_ = map_level_0_;
    } else {
        map_ = map_level_1_;
    }

    // frame_id déjà correct depuis les callbacks
    // juste mettre à jour le timestamp
    map_.header.stamp = this->now();

    map_pub_->publish(map_);
    res->success = true;
    res->message = "Successfully loaded map for level: " + std::to_string(req->req_int);
    RCLCPP_INFO(this->get_logger(), "%s", res->message.c_str());
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ChangeMap>());
    rclcpp::shutdown();
    return 0;
}