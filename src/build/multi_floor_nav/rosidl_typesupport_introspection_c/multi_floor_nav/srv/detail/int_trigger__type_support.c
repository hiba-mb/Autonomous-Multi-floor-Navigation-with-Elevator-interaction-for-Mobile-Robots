// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from multi_floor_nav:srv/IntTrigger.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "multi_floor_nav/srv/detail/int_trigger__rosidl_typesupport_introspection_c.h"
#include "multi_floor_nav/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "multi_floor_nav/srv/detail/int_trigger__functions.h"
#include "multi_floor_nav/srv/detail/int_trigger__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  multi_floor_nav__srv__IntTrigger_Request__init(message_memory);
}

void multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_fini_function(void * message_memory)
{
  multi_floor_nav__srv__IntTrigger_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_member_array[1] = {
  {
    "req_int",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__srv__IntTrigger_Request, req_int),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_members = {
  "multi_floor_nav__srv",  // message namespace
  "IntTrigger_Request",  // message name
  1,  // number of fields
  sizeof(multi_floor_nav__srv__IntTrigger_Request),
  false,  // has_any_key_member_
  multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_member_array,  // message members
  multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_type_support_handle = {
  0,
  &multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_members,
  get_message_typesupport_handle_function,
  &multi_floor_nav__srv__IntTrigger_Request__get_type_hash,
  &multi_floor_nav__srv__IntTrigger_Request__get_type_description,
  &multi_floor_nav__srv__IntTrigger_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_multi_floor_nav
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Request)() {
  if (!multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_type_support_handle.typesupport_identifier) {
    multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "multi_floor_nav/srv/detail/int_trigger__rosidl_typesupport_introspection_c.h"
// already included above
// #include "multi_floor_nav/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "multi_floor_nav/srv/detail/int_trigger__functions.h"
// already included above
// #include "multi_floor_nav/srv/detail/int_trigger__struct.h"


// Include directives for member types
// Member `message`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  multi_floor_nav__srv__IntTrigger_Response__init(message_memory);
}

void multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_fini_function(void * message_memory)
{
  multi_floor_nav__srv__IntTrigger_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_member_array[2] = {
  {
    "success",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__srv__IntTrigger_Response, success),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "message",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__srv__IntTrigger_Response, message),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_members = {
  "multi_floor_nav__srv",  // message namespace
  "IntTrigger_Response",  // message name
  2,  // number of fields
  sizeof(multi_floor_nav__srv__IntTrigger_Response),
  false,  // has_any_key_member_
  multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_member_array,  // message members
  multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_type_support_handle = {
  0,
  &multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_members,
  get_message_typesupport_handle_function,
  &multi_floor_nav__srv__IntTrigger_Response__get_type_hash,
  &multi_floor_nav__srv__IntTrigger_Response__get_type_description,
  &multi_floor_nav__srv__IntTrigger_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_multi_floor_nav
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Response)() {
  if (!multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_type_support_handle.typesupport_identifier) {
    multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "multi_floor_nav/srv/detail/int_trigger__rosidl_typesupport_introspection_c.h"
// already included above
// #include "multi_floor_nav/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "multi_floor_nav/srv/detail/int_trigger__functions.h"
// already included above
// #include "multi_floor_nav/srv/detail/int_trigger__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
#include "multi_floor_nav/srv/int_trigger.h"
// Member `request`
// Member `response`
// already included above
// #include "multi_floor_nav/srv/detail/int_trigger__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  multi_floor_nav__srv__IntTrigger_Event__init(message_memory);
}

void multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_fini_function(void * message_memory)
{
  multi_floor_nav__srv__IntTrigger_Event__fini(message_memory);
}

size_t multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__size_function__IntTrigger_Event__request(
  const void * untyped_member)
{
  const multi_floor_nav__srv__IntTrigger_Request__Sequence * member =
    (const multi_floor_nav__srv__IntTrigger_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_const_function__IntTrigger_Event__request(
  const void * untyped_member, size_t index)
{
  const multi_floor_nav__srv__IntTrigger_Request__Sequence * member =
    (const multi_floor_nav__srv__IntTrigger_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_function__IntTrigger_Event__request(
  void * untyped_member, size_t index)
{
  multi_floor_nav__srv__IntTrigger_Request__Sequence * member =
    (multi_floor_nav__srv__IntTrigger_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__fetch_function__IntTrigger_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const multi_floor_nav__srv__IntTrigger_Request * item =
    ((const multi_floor_nav__srv__IntTrigger_Request *)
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_const_function__IntTrigger_Event__request(untyped_member, index));
  multi_floor_nav__srv__IntTrigger_Request * value =
    (multi_floor_nav__srv__IntTrigger_Request *)(untyped_value);
  *value = *item;
}

void multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__assign_function__IntTrigger_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  multi_floor_nav__srv__IntTrigger_Request * item =
    ((multi_floor_nav__srv__IntTrigger_Request *)
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_function__IntTrigger_Event__request(untyped_member, index));
  const multi_floor_nav__srv__IntTrigger_Request * value =
    (const multi_floor_nav__srv__IntTrigger_Request *)(untyped_value);
  *item = *value;
}

bool multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__resize_function__IntTrigger_Event__request(
  void * untyped_member, size_t size)
{
  multi_floor_nav__srv__IntTrigger_Request__Sequence * member =
    (multi_floor_nav__srv__IntTrigger_Request__Sequence *)(untyped_member);
  multi_floor_nav__srv__IntTrigger_Request__Sequence__fini(member);
  return multi_floor_nav__srv__IntTrigger_Request__Sequence__init(member, size);
}

size_t multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__size_function__IntTrigger_Event__response(
  const void * untyped_member)
{
  const multi_floor_nav__srv__IntTrigger_Response__Sequence * member =
    (const multi_floor_nav__srv__IntTrigger_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_const_function__IntTrigger_Event__response(
  const void * untyped_member, size_t index)
{
  const multi_floor_nav__srv__IntTrigger_Response__Sequence * member =
    (const multi_floor_nav__srv__IntTrigger_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_function__IntTrigger_Event__response(
  void * untyped_member, size_t index)
{
  multi_floor_nav__srv__IntTrigger_Response__Sequence * member =
    (multi_floor_nav__srv__IntTrigger_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__fetch_function__IntTrigger_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const multi_floor_nav__srv__IntTrigger_Response * item =
    ((const multi_floor_nav__srv__IntTrigger_Response *)
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_const_function__IntTrigger_Event__response(untyped_member, index));
  multi_floor_nav__srv__IntTrigger_Response * value =
    (multi_floor_nav__srv__IntTrigger_Response *)(untyped_value);
  *value = *item;
}

void multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__assign_function__IntTrigger_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  multi_floor_nav__srv__IntTrigger_Response * item =
    ((multi_floor_nav__srv__IntTrigger_Response *)
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_function__IntTrigger_Event__response(untyped_member, index));
  const multi_floor_nav__srv__IntTrigger_Response * value =
    (const multi_floor_nav__srv__IntTrigger_Response *)(untyped_value);
  *item = *value;
}

bool multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__resize_function__IntTrigger_Event__response(
  void * untyped_member, size_t size)
{
  multi_floor_nav__srv__IntTrigger_Response__Sequence * member =
    (multi_floor_nav__srv__IntTrigger_Response__Sequence *)(untyped_member);
  multi_floor_nav__srv__IntTrigger_Response__Sequence__fini(member);
  return multi_floor_nav__srv__IntTrigger_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(multi_floor_nav__srv__IntTrigger_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(multi_floor_nav__srv__IntTrigger_Event, request),  // bytes offset in struct
    NULL,  // default value
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__size_function__IntTrigger_Event__request,  // size() function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_const_function__IntTrigger_Event__request,  // get_const(index) function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_function__IntTrigger_Event__request,  // get(index) function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__fetch_function__IntTrigger_Event__request,  // fetch(index, &value) function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__assign_function__IntTrigger_Event__request,  // assign(index, value) function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__resize_function__IntTrigger_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(multi_floor_nav__srv__IntTrigger_Event, response),  // bytes offset in struct
    NULL,  // default value
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__size_function__IntTrigger_Event__response,  // size() function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_const_function__IntTrigger_Event__response,  // get_const(index) function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__get_function__IntTrigger_Event__response,  // get(index) function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__fetch_function__IntTrigger_Event__response,  // fetch(index, &value) function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__assign_function__IntTrigger_Event__response,  // assign(index, value) function pointer
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__resize_function__IntTrigger_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_members = {
  "multi_floor_nav__srv",  // message namespace
  "IntTrigger_Event",  // message name
  3,  // number of fields
  sizeof(multi_floor_nav__srv__IntTrigger_Event),
  false,  // has_any_key_member_
  multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_member_array,  // message members
  multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_type_support_handle = {
  0,
  &multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_members,
  get_message_typesupport_handle_function,
  &multi_floor_nav__srv__IntTrigger_Event__get_type_hash,
  &multi_floor_nav__srv__IntTrigger_Event__get_type_description,
  &multi_floor_nav__srv__IntTrigger_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_multi_floor_nav
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Event)() {
  multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Request)();
  multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Response)();
  if (!multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_type_support_handle.typesupport_identifier) {
    multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "multi_floor_nav/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "multi_floor_nav/srv/detail/int_trigger__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_service_members = {
  "multi_floor_nav__srv",  // service namespace
  "IntTrigger",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_Request_message_type_support_handle,
  NULL,  // response message
  // multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_Response_message_type_support_handle
  NULL  // event_message
  // multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_Response_message_type_support_handle
};


static rosidl_service_type_support_t multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_service_type_support_handle = {
  0,
  &multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_service_members,
  get_service_typesupport_handle_function,
  &multi_floor_nav__srv__IntTrigger_Request__rosidl_typesupport_introspection_c__IntTrigger_Request_message_type_support_handle,
  &multi_floor_nav__srv__IntTrigger_Response__rosidl_typesupport_introspection_c__IntTrigger_Response_message_type_support_handle,
  &multi_floor_nav__srv__IntTrigger_Event__rosidl_typesupport_introspection_c__IntTrigger_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    multi_floor_nav,
    srv,
    IntTrigger
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    multi_floor_nav,
    srv,
    IntTrigger
  ),
  &multi_floor_nav__srv__IntTrigger__get_type_hash,
  &multi_floor_nav__srv__IntTrigger__get_type_description,
  &multi_floor_nav__srv__IntTrigger__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_multi_floor_nav
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger)(void) {
  if (!multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_service_type_support_handle.typesupport_identifier) {
    multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, multi_floor_nav, srv, IntTrigger_Event)()->data;
  }

  return &multi_floor_nav__srv__detail__int_trigger__rosidl_typesupport_introspection_c__IntTrigger_service_type_support_handle;
}
