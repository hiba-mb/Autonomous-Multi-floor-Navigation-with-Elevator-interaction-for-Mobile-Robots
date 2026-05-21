// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice

#include "multi_floor_nav/msg/detail/mission_request__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_type_hash_t *
multi_floor_nav__msg__MissionRequest__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xee, 0xba, 0x05, 0x4b, 0xd6, 0xa0, 0xb9, 0xf3,
      0x33, 0x56, 0x4e, 0x98, 0x96, 0x47, 0x07, 0x1e,
      0x83, 0x81, 0xdb, 0xbe, 0xa9, 0x83, 0xad, 0x48,
      0x58, 0x8b, 0x25, 0x47, 0xe1, 0x3f, 0xf2, 0x87,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char multi_floor_nav__msg__MissionRequest__TYPE_NAME[] = "multi_floor_nav/msg/MissionRequest";

// Define type names, field names, and default values
static char multi_floor_nav__msg__MissionRequest__FIELD_NAME__robot_id[] = "robot_id";
static char multi_floor_nav__msg__MissionRequest__FIELD_NAME__x[] = "x";
static char multi_floor_nav__msg__MissionRequest__FIELD_NAME__y[] = "y";
static char multi_floor_nav__msg__MissionRequest__FIELD_NAME__yaw[] = "yaw";
static char multi_floor_nav__msg__MissionRequest__FIELD_NAME__target_floor[] = "target_floor";
static char multi_floor_nav__msg__MissionRequest__FIELD_NAME__mission_id[] = "mission_id";

static rosidl_runtime_c__type_description__Field multi_floor_nav__msg__MissionRequest__FIELDS[] = {
  {
    {multi_floor_nav__msg__MissionRequest__FIELD_NAME__robot_id, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__msg__MissionRequest__FIELD_NAME__x, 1, 1},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__msg__MissionRequest__FIELD_NAME__y, 1, 1},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__msg__MissionRequest__FIELD_NAME__yaw, 3, 3},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__msg__MissionRequest__FIELD_NAME__target_floor, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__msg__MissionRequest__FIELD_NAME__mission_id, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
multi_floor_nav__msg__MissionRequest__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {multi_floor_nav__msg__MissionRequest__TYPE_NAME, 34, 34},
      {multi_floor_nav__msg__MissionRequest__FIELDS, 6, 6},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# MissionRequest.msg\n"
  "string robot_id        # 'robot1' / 'robot2' / 'auto'\n"
  "float64 x\n"
  "float64 y\n"
  "float64 yaw\n"
  "int32 target_floor\n"
  "string mission_id      # UUID unique";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
multi_floor_nav__msg__MissionRequest__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {multi_floor_nav__msg__MissionRequest__TYPE_NAME, 34, 34},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 163, 163},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
multi_floor_nav__msg__MissionRequest__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *multi_floor_nav__msg__MissionRequest__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
