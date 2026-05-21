// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice

#ifndef MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include <cstddef>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "multi_floor_nav/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "multi_floor_nav/msg/detail/mission_request__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace multi_floor_nav
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_multi_floor_nav
cdr_serialize(
  const multi_floor_nav::msg::MissionRequest & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_multi_floor_nav
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  multi_floor_nav::msg::MissionRequest & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_multi_floor_nav
get_serialized_size(
  const multi_floor_nav::msg::MissionRequest & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_multi_floor_nav
max_serialized_size_MissionRequest(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_multi_floor_nav
cdr_serialize_key(
  const multi_floor_nav::msg::MissionRequest & ros_message,
  eprosima::fastcdr::Cdr &);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_multi_floor_nav
get_serialized_size_key(
  const multi_floor_nav::msg::MissionRequest & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_multi_floor_nav
max_serialized_size_key_MissionRequest(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace multi_floor_nav

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_multi_floor_nav
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, multi_floor_nav, msg, MissionRequest)();

#ifdef __cplusplus
}
#endif

#endif  // MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
