// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "multi_floor_nav/msg/mission_request.hpp"


#ifndef MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__TRAITS_HPP_
#define MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "multi_floor_nav/msg/detail/mission_request__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace multi_floor_nav
{

namespace msg
{

inline void to_flow_style_yaml(
  const MissionRequest & msg,
  std::ostream & out)
{
  out << "{";
  // member: robot_id
  {
    out << "robot_id: ";
    rosidl_generator_traits::value_to_yaml(msg.robot_id, out);
    out << ", ";
  }

  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << ", ";
  }

  // member: yaw
  {
    out << "yaw: ";
    rosidl_generator_traits::value_to_yaml(msg.yaw, out);
    out << ", ";
  }

  // member: target_floor
  {
    out << "target_floor: ";
    rosidl_generator_traits::value_to_yaml(msg.target_floor, out);
    out << ", ";
  }

  // member: mission_id
  {
    out << "mission_id: ";
    rosidl_generator_traits::value_to_yaml(msg.mission_id, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MissionRequest & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: robot_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "robot_id: ";
    rosidl_generator_traits::value_to_yaml(msg.robot_id, out);
    out << "\n";
  }

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: yaw
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "yaw: ";
    rosidl_generator_traits::value_to_yaml(msg.yaw, out);
    out << "\n";
  }

  // member: target_floor
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target_floor: ";
    rosidl_generator_traits::value_to_yaml(msg.target_floor, out);
    out << "\n";
  }

  // member: mission_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "mission_id: ";
    rosidl_generator_traits::value_to_yaml(msg.mission_id, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MissionRequest & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace multi_floor_nav

namespace rosidl_generator_traits
{

[[deprecated("use multi_floor_nav::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const multi_floor_nav::msg::MissionRequest & msg,
  std::ostream & out, size_t indentation = 0)
{
  multi_floor_nav::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use multi_floor_nav::msg::to_yaml() instead")]]
inline std::string to_yaml(const multi_floor_nav::msg::MissionRequest & msg)
{
  return multi_floor_nav::msg::to_yaml(msg);
}

template<>
inline const char * data_type<multi_floor_nav::msg::MissionRequest>()
{
  return "multi_floor_nav::msg::MissionRequest";
}

template<>
inline const char * name<multi_floor_nav::msg::MissionRequest>()
{
  return "multi_floor_nav/msg/MissionRequest";
}

template<>
struct has_fixed_size<multi_floor_nav::msg::MissionRequest>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<multi_floor_nav::msg::MissionRequest>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<multi_floor_nav::msg::MissionRequest>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__TRAITS_HPP_
