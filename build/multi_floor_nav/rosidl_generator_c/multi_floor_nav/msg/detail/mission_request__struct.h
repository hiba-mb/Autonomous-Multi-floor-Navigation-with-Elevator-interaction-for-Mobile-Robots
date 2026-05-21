// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "multi_floor_nav/msg/mission_request.h"


#ifndef MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__STRUCT_H_
#define MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'robot_id'
// Member 'mission_id'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/MissionRequest in the package multi_floor_nav.
/**
  * MissionRequest.msg
 */
typedef struct multi_floor_nav__msg__MissionRequest
{
  /// 'robot1' / 'robot2' / 'auto'
  rosidl_runtime_c__String robot_id;
  double x;
  double y;
  double yaw;
  int32_t target_floor;
  /// UUID unique
  rosidl_runtime_c__String mission_id;
} multi_floor_nav__msg__MissionRequest;

// Struct for a sequence of multi_floor_nav__msg__MissionRequest.
typedef struct multi_floor_nav__msg__MissionRequest__Sequence
{
  multi_floor_nav__msg__MissionRequest * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} multi_floor_nav__msg__MissionRequest__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__STRUCT_H_
