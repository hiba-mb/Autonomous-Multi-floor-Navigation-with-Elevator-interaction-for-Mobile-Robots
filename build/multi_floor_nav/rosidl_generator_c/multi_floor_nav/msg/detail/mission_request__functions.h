// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "multi_floor_nav/msg/mission_request.h"


#ifndef MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__FUNCTIONS_H_
#define MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/action_type_support_struct.h"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_runtime_c/type_description/type_description__struct.h"
#include "rosidl_runtime_c/type_description/type_source__struct.h"
#include "rosidl_runtime_c/type_hash.h"
#include "rosidl_runtime_c/visibility_control.h"
#include "multi_floor_nav/msg/rosidl_generator_c__visibility_control.h"

#include "multi_floor_nav/msg/detail/mission_request__struct.h"

/// Initialize msg/MissionRequest message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * multi_floor_nav__msg__MissionRequest
 * )) before or use
 * multi_floor_nav__msg__MissionRequest__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
bool
multi_floor_nav__msg__MissionRequest__init(multi_floor_nav__msg__MissionRequest * msg);

/// Finalize msg/MissionRequest message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
void
multi_floor_nav__msg__MissionRequest__fini(multi_floor_nav__msg__MissionRequest * msg);

/// Create msg/MissionRequest message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * multi_floor_nav__msg__MissionRequest__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
multi_floor_nav__msg__MissionRequest *
multi_floor_nav__msg__MissionRequest__create(void);

/// Destroy msg/MissionRequest message.
/**
 * It calls
 * multi_floor_nav__msg__MissionRequest__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
void
multi_floor_nav__msg__MissionRequest__destroy(multi_floor_nav__msg__MissionRequest * msg);

/// Check for msg/MissionRequest message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
bool
multi_floor_nav__msg__MissionRequest__are_equal(const multi_floor_nav__msg__MissionRequest * lhs, const multi_floor_nav__msg__MissionRequest * rhs);

/// Copy a msg/MissionRequest message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
bool
multi_floor_nav__msg__MissionRequest__copy(
  const multi_floor_nav__msg__MissionRequest * input,
  multi_floor_nav__msg__MissionRequest * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_type_hash_t *
multi_floor_nav__msg__MissionRequest__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_runtime_c__type_description__TypeDescription *
multi_floor_nav__msg__MissionRequest__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_runtime_c__type_description__TypeSource *
multi_floor_nav__msg__MissionRequest__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_runtime_c__type_description__TypeSource__Sequence *
multi_floor_nav__msg__MissionRequest__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of msg/MissionRequest messages.
/**
 * It allocates the memory for the number of elements and calls
 * multi_floor_nav__msg__MissionRequest__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
bool
multi_floor_nav__msg__MissionRequest__Sequence__init(multi_floor_nav__msg__MissionRequest__Sequence * array, size_t size);

/// Finalize array of msg/MissionRequest messages.
/**
 * It calls
 * multi_floor_nav__msg__MissionRequest__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
void
multi_floor_nav__msg__MissionRequest__Sequence__fini(multi_floor_nav__msg__MissionRequest__Sequence * array);

/// Create array of msg/MissionRequest messages.
/**
 * It allocates the memory for the array and calls
 * multi_floor_nav__msg__MissionRequest__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
multi_floor_nav__msg__MissionRequest__Sequence *
multi_floor_nav__msg__MissionRequest__Sequence__create(size_t size);

/// Destroy array of msg/MissionRequest messages.
/**
 * It calls
 * multi_floor_nav__msg__MissionRequest__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
void
multi_floor_nav__msg__MissionRequest__Sequence__destroy(multi_floor_nav__msg__MissionRequest__Sequence * array);

/// Check for msg/MissionRequest message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
bool
multi_floor_nav__msg__MissionRequest__Sequence__are_equal(const multi_floor_nav__msg__MissionRequest__Sequence * lhs, const multi_floor_nav__msg__MissionRequest__Sequence * rhs);

/// Copy an array of msg/MissionRequest messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
bool
multi_floor_nav__msg__MissionRequest__Sequence__copy(
  const multi_floor_nav__msg__MissionRequest__Sequence * input,
  multi_floor_nav__msg__MissionRequest__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__FUNCTIONS_H_
