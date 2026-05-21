// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice
#include "multi_floor_nav/msg/detail/mission_request__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `robot_id`
// Member `mission_id`
#include "rosidl_runtime_c/string_functions.h"

bool
multi_floor_nav__msg__MissionRequest__init(multi_floor_nav__msg__MissionRequest * msg)
{
  if (!msg) {
    return false;
  }
  // robot_id
  if (!rosidl_runtime_c__String__init(&msg->robot_id)) {
    multi_floor_nav__msg__MissionRequest__fini(msg);
    return false;
  }
  // x
  // y
  // yaw
  // target_floor
  // mission_id
  if (!rosidl_runtime_c__String__init(&msg->mission_id)) {
    multi_floor_nav__msg__MissionRequest__fini(msg);
    return false;
  }
  return true;
}

void
multi_floor_nav__msg__MissionRequest__fini(multi_floor_nav__msg__MissionRequest * msg)
{
  if (!msg) {
    return;
  }
  // robot_id
  rosidl_runtime_c__String__fini(&msg->robot_id);
  // x
  // y
  // yaw
  // target_floor
  // mission_id
  rosidl_runtime_c__String__fini(&msg->mission_id);
}

bool
multi_floor_nav__msg__MissionRequest__are_equal(const multi_floor_nav__msg__MissionRequest * lhs, const multi_floor_nav__msg__MissionRequest * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // robot_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->robot_id), &(rhs->robot_id)))
  {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // y
  if (lhs->y != rhs->y) {
    return false;
  }
  // yaw
  if (lhs->yaw != rhs->yaw) {
    return false;
  }
  // target_floor
  if (lhs->target_floor != rhs->target_floor) {
    return false;
  }
  // mission_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->mission_id), &(rhs->mission_id)))
  {
    return false;
  }
  return true;
}

bool
multi_floor_nav__msg__MissionRequest__copy(
  const multi_floor_nav__msg__MissionRequest * input,
  multi_floor_nav__msg__MissionRequest * output)
{
  if (!input || !output) {
    return false;
  }
  // robot_id
  if (!rosidl_runtime_c__String__copy(
      &(input->robot_id), &(output->robot_id)))
  {
    return false;
  }
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  // yaw
  output->yaw = input->yaw;
  // target_floor
  output->target_floor = input->target_floor;
  // mission_id
  if (!rosidl_runtime_c__String__copy(
      &(input->mission_id), &(output->mission_id)))
  {
    return false;
  }
  return true;
}

multi_floor_nav__msg__MissionRequest *
multi_floor_nav__msg__MissionRequest__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  multi_floor_nav__msg__MissionRequest * msg = (multi_floor_nav__msg__MissionRequest *)allocator.allocate(sizeof(multi_floor_nav__msg__MissionRequest), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(multi_floor_nav__msg__MissionRequest));
  bool success = multi_floor_nav__msg__MissionRequest__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
multi_floor_nav__msg__MissionRequest__destroy(multi_floor_nav__msg__MissionRequest * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    multi_floor_nav__msg__MissionRequest__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
multi_floor_nav__msg__MissionRequest__Sequence__init(multi_floor_nav__msg__MissionRequest__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  multi_floor_nav__msg__MissionRequest * data = NULL;

  if (size) {
    data = (multi_floor_nav__msg__MissionRequest *)allocator.zero_allocate(size, sizeof(multi_floor_nav__msg__MissionRequest), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = multi_floor_nav__msg__MissionRequest__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        multi_floor_nav__msg__MissionRequest__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
multi_floor_nav__msg__MissionRequest__Sequence__fini(multi_floor_nav__msg__MissionRequest__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      multi_floor_nav__msg__MissionRequest__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

multi_floor_nav__msg__MissionRequest__Sequence *
multi_floor_nav__msg__MissionRequest__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  multi_floor_nav__msg__MissionRequest__Sequence * array = (multi_floor_nav__msg__MissionRequest__Sequence *)allocator.allocate(sizeof(multi_floor_nav__msg__MissionRequest__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = multi_floor_nav__msg__MissionRequest__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
multi_floor_nav__msg__MissionRequest__Sequence__destroy(multi_floor_nav__msg__MissionRequest__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    multi_floor_nav__msg__MissionRequest__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
multi_floor_nav__msg__MissionRequest__Sequence__are_equal(const multi_floor_nav__msg__MissionRequest__Sequence * lhs, const multi_floor_nav__msg__MissionRequest__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!multi_floor_nav__msg__MissionRequest__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
multi_floor_nav__msg__MissionRequest__Sequence__copy(
  const multi_floor_nav__msg__MissionRequest__Sequence * input,
  multi_floor_nav__msg__MissionRequest__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(multi_floor_nav__msg__MissionRequest);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    multi_floor_nav__msg__MissionRequest * data =
      (multi_floor_nav__msg__MissionRequest *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!multi_floor_nav__msg__MissionRequest__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          multi_floor_nav__msg__MissionRequest__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!multi_floor_nav__msg__MissionRequest__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
