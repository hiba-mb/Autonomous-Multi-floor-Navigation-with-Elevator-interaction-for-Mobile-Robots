// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "multi_floor_nav/msg/detail/mission_request__rosidl_typesupport_introspection_c.h"
#include "multi_floor_nav/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "multi_floor_nav/msg/detail/mission_request__functions.h"
#include "multi_floor_nav/msg/detail/mission_request__struct.h"


// Include directives for member types
// Member `robot_id`
// Member `mission_id`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  multi_floor_nav__msg__MissionRequest__init(message_memory);
}

void multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_fini_function(void * message_memory)
{
  multi_floor_nav__msg__MissionRequest__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_message_member_array[6] = {
  {
    "robot_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__msg__MissionRequest, robot_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__msg__MissionRequest, x),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__msg__MissionRequest, y),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "yaw",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__msg__MissionRequest, yaw),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "target_floor",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__msg__MissionRequest, target_floor),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "mission_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__msg__MissionRequest, mission_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_message_members = {
  "multi_floor_nav__msg",  // message namespace
  "MissionRequest",  // message name
  6,  // number of fields
  sizeof(multi_floor_nav__msg__MissionRequest),
  false,  // has_any_key_member_
  multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_message_member_array,  // message members
  multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_init_function,  // function to initialize message memory (memory has to be allocated)
  multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_message_type_support_handle = {
  0,
  &multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_message_members,
  get_message_typesupport_handle_function,
  &multi_floor_nav__msg__MissionRequest__get_type_hash,
  &multi_floor_nav__msg__MissionRequest__get_type_description,
  &multi_floor_nav__msg__MissionRequest__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_multi_floor_nav
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, msg, MissionRequest)() {
  if (!multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_message_type_support_handle.typesupport_identifier) {
    multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &multi_floor_nav__msg__MissionRequest__rosidl_typesupport_introspection_c__MissionRequest_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
