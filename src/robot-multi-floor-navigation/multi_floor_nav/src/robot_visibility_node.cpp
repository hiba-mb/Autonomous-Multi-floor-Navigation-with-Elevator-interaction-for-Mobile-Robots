#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/string.hpp>
#include <map>
#include <string>

class RobotVisibilityNode : public rclcpp::Node
{
public:
  RobotVisibilityNode() : rclcpp::Node("robot_visibility_node")
  {
    this->declare_parameter<std::vector<std::string>>("robots", {"robot1", "robot2"});
    robots_ = this->get_parameter("robots").as_string_array();

    active_floor_sub_ = this->create_subscription<std_msgs::msg::Int32>(
      "/active_floor", rclcpp::QoS(1).transient_local(),
      [this](const std_msgs::msg::Int32::SharedPtr msg) {
        active_floor_ = msg->data;
        update_visibility();
      });

    for (const auto & robot : robots_) {
      robot_floors_[robot] = 0;

      auto floor_sub = this->create_subscription<std_msgs::msg::Int32>(
        "/" + robot + "/current_floor",
        rclcpp::QoS(1).transient_local(),
        [this, robot](const std_msgs::msg::Int32::SharedPtr msg) {
          robot_floors_[robot] = msg->data;
          update_visibility();
        });
      floor_subs_.push_back(floor_sub);

      auto desc_pub = this->create_publisher<std_msgs::msg::String>(
        "/" + robot + "/robot_description_filtered",
        rclcpp::QoS(1).transient_local());
      description_pubs_[robot] = desc_pub;

      auto desc_sub = this->create_subscription<std_msgs::msg::String>(
        "/" + robot + "/robot_description",
        rclcpp::QoS(1).transient_local(),
        [this, robot](const std_msgs::msg::String::SharedPtr msg) {
          robot_descriptions_[robot] = msg->data;
          update_visibility();
        });
      desc_subs_.push_back(desc_sub);
    }
  }

private:
  void update_visibility()
  {
    for (const auto & robot : robots_) {
      if (!description_pubs_.count(robot)) continue;

      bool visible = (robot_floors_[robot] == active_floor_);

      std_msgs::msg::String msg;
      msg.data = (visible && robot_descriptions_.count(robot))
                 ? robot_descriptions_[robot] : "";
      description_pubs_[robot]->publish(msg);

      RCLCPP_INFO(this->get_logger(), "Robot %s floor=%d active=%d → %s",
        robot.c_str(), robot_floors_[robot], active_floor_,
        visible ? "VISIBLE" : "MASQUÉ");
    }
  }

  int active_floor_ = 0;
  std::vector<std::string> robots_;
  std::map<std::string, int> robot_floors_;
  std::map<std::string, std::string> robot_descriptions_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr active_floor_sub_;
  std::vector<rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr> floor_subs_;
  std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> desc_subs_;
  std::map<std::string, rclcpp::Publisher<std_msgs::msg::String>::SharedPtr> description_pubs_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RobotVisibilityNode>());
  rclcpp::shutdown();
  return 0;
}