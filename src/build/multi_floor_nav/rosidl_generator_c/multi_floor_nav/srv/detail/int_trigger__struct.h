// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from multi_floor_nav:srv/IntTrigger.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "multi_floor_nav/srv/int_trigger.h"


#ifndef MULTI_FLOOR_NAV__SRV__DETAIL__INT_TRIGGER__STRUCT_H_
#define MULTI_FLOOR_NAV__SRV__DETAIL__INT_TRIGGER__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/IntTrigger in the package multi_floor_nav.
typedef struct multi_floor_nav__srv__IntTrigger_Request
{
  int32_t req_int;
} multi_floor_nav__srv__IntTrigger_Request;

// Struct for a sequence of multi_floor_nav__srv__IntTrigger_Request.
typedef struct multi_floor_nav__srv__IntTrigger_Request__Sequence
{
  multi_floor_nav__srv__IntTrigger_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} multi_floor_nav__srv__IntTrigger_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/IntTrigger in the package multi_floor_nav.
typedef struct multi_floor_nav__srv__IntTrigger_Response
{
  bool success;
  rosidl_runtime_c__String message;
} multi_floor_nav__srv__IntTrigger_Response;

// Struct for a sequence of multi_floor_nav__srv__IntTrigger_Response.
typedef struct multi_floor_nav__srv__IntTrigger_Response__Sequence
{
  multi_floor_nav__srv__IntTrigger_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} multi_floor_nav__srv__IntTrigger_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  multi_floor_nav__srv__IntTrigger_Event__request__MAX_SIZE = 1
};
// response
enum
{
  multi_floor_nav__srv__IntTrigger_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/IntTrigger in the package multi_floor_nav.
typedef struct multi_floor_nav__srv__IntTrigger_Event
{
  service_msgs__msg__ServiceEventInfo info;
  multi_floor_nav__srv__IntTrigger_Request__Sequence request;
  multi_floor_nav__srv__IntTrigger_Response__Sequence response;
} multi_floor_nav__srv__IntTrigger_Event;

// Struct for a sequence of multi_floor_nav__srv__IntTrigger_Event.
typedef struct multi_floor_nav__srv__IntTrigger_Event__Sequence
{
  multi_floor_nav__srv__IntTrigger_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} multi_floor_nav__srv__IntTrigger_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MULTI_FLOOR_NAV__SRV__DETAIL__INT_TRIGGER__STRUCT_H_
