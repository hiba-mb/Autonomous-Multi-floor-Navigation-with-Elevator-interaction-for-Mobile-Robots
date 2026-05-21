// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "multi_floor_nav/msg/mission_request.hpp"


#ifndef MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__BUILDER_HPP_
#define MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "multi_floor_nav/msg/detail/mission_request__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace multi_floor_nav
{

namespace msg
{

namespace builder
{

class Init_MissionRequest_mission_id
{
public:
  explicit Init_MissionRequest_mission_id(::multi_floor_nav::msg::MissionRequest & msg)
  : msg_(msg)
  {}
  ::multi_floor_nav::msg::MissionRequest mission_id(::multi_floor_nav::msg::MissionRequest::_mission_id_type arg)
  {
    msg_.mission_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::multi_floor_nav::msg::MissionRequest msg_;
};

class Init_MissionRequest_target_floor
{
public:
  explicit Init_MissionRequest_target_floor(::multi_floor_nav::msg::MissionRequest & msg)
  : msg_(msg)
  {}
  Init_MissionRequest_mission_id target_floor(::multi_floor_nav::msg::MissionRequest::_target_floor_type arg)
  {
    msg_.target_floor = std::move(arg);
    return Init_MissionRequest_mission_id(msg_);
  }

private:
  ::multi_floor_nav::msg::MissionRequest msg_;
};

class Init_MissionRequest_yaw
{
public:
  explicit Init_MissionRequest_yaw(::multi_floor_nav::msg::MissionRequest & msg)
  : msg_(msg)
  {}
  Init_MissionRequest_target_floor yaw(::multi_floor_nav::msg::MissionRequest::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_MissionRequest_target_floor(msg_);
  }

private:
  ::multi_floor_nav::msg::MissionRequest msg_;
};

class Init_MissionRequest_y
{
public:
  explicit Init_MissionRequest_y(::multi_floor_nav::msg::MissionRequest & msg)
  : msg_(msg)
  {}
  Init_MissionRequest_yaw y(::multi_floor_nav::msg::MissionRequest::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_MissionRequest_yaw(msg_);
  }

private:
  ::multi_floor_nav::msg::MissionRequest msg_;
};

class Init_MissionRequest_x
{
public:
  explicit Init_MissionRequest_x(::multi_floor_nav::msg::MissionRequest & msg)
  : msg_(msg)
  {}
  Init_MissionRequest_y x(::multi_floor_nav::msg::MissionRequest::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_MissionRequest_y(msg_);
  }

private:
  ::multi_floor_nav::msg::MissionRequest msg_;
};

class Init_MissionRequest_robot_id
{
public:
  Init_MissionRequest_robot_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MissionRequest_x robot_id(::multi_floor_nav::msg::MissionRequest::_robot_id_type arg)
  {
    msg_.robot_id = std::move(arg);
    return Init_MissionRequest_x(msg_);
  }

private:
  ::multi_floor_nav::msg::MissionRequest msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::multi_floor_nav::msg::MissionRequest>()
{
  return multi_floor_nav::msg::builder::Init_MissionRequest_robot_id();
}

}  // namespace multi_floor_nav

#endif  // MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__BUILDER_HPP_
