#ifndef CHANGE_MAP_HPP
#define CHANGE_MAP_HPP

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/occupancy_grid.hpp"
#include "multi_floor_nav/srv/int_trigger.hpp"

class ChangeMap : public rclcpp::Node
{
public:
    ChangeMap();

private:
    std::string ns_;   // ← ajouter
    rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr map_pub_;
    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_level_0_sub_;
    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_level_1_sub_;
    rclcpp::Service<multi_floor_nav::srv::IntTrigger>::SharedPtr change_map_server_;
    nav_msgs::msg::OccupancyGrid map_level_0_, map_level_1_, map_;
    rclcpp::TimerBase::SharedPtr init_timer_;  // ✅ ajouter
    void mapLevelZeroCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
    void mapLevelOneCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
    void changeMapCallback(
        const std::shared_ptr<multi_floor_nav::srv::IntTrigger::Request> req,
        std::shared_ptr<multi_floor_nav::srv::IntTrigger::Response> res);
};

#endif
