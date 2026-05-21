// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice
#ifndef MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "multi_floor_nav/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "multi_floor_nav/msg/detail/mission_request__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_multi_floor_nav
bool cdr_serialize_multi_floor_nav__msg__MissionRequest(
  const multi_floor_nav__msg__MissionRequest * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_multi_floor_nav
bool cdr_deserialize_multi_floor_nav__msg__MissionRequest(
  eprosima::fastcdr::Cdr &,
  multi_floor_nav__msg__MissionRequest * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_multi_floor_nav
size_t get_serialized_size_multi_floor_nav__msg__MissionRequest(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_multi_floor_nav
size_t max_serialized_size_multi_floor_nav__msg__MissionRequest(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_multi_floor_nav
bool cdr_serialize_key_multi_floor_nav__msg__MissionRequest(
  const multi_floor_nav__msg__MissionRequest * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_multi_floor_nav
size_t get_serialized_size_key_multi_floor_nav__msg__MissionRequest(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_multi_floor_nav
size_t max_serialized_size_key_multi_floor_nav__msg__MissionRequest(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_multi_floor_nav
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, multi_floor_nav, msg, MissionRequest)();

#ifdef __cplusplus
}
#endif

#endif  // MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
