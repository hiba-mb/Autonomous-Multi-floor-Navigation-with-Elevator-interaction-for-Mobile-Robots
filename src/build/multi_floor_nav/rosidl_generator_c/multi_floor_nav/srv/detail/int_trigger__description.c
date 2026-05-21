// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from multi_floor_nav:srv/IntTrigger.idl
// generated code does not contain a copyright notice

#include "multi_floor_nav/srv/detail/int_trigger__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_type_hash_t *
multi_floor_nav__srv__IntTrigger__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xba, 0x37, 0xf5, 0x3b, 0x96, 0x96, 0x69, 0x96,
      0x03, 0x88, 0x48, 0x7d, 0xf9, 0xcc, 0x19, 0x44,
      0xe7, 0xf0, 0xfa, 0x8d, 0xc1, 0x7a, 0xe7, 0xfd,
      0x48, 0x38, 0x06, 0xd2, 0xca, 0x8c, 0xfd, 0x59,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_type_hash_t *
multi_floor_nav__srv__IntTrigger_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x0c, 0xf0, 0x9f, 0x78, 0x7a, 0x07, 0x5e, 0xe6,
      0xa0, 0xf8, 0x44, 0x5c, 0xe7, 0x4d, 0xbd, 0x77,
      0xf1, 0xa7, 0x34, 0x33, 0x41, 0xd7, 0x29, 0xee,
      0xdb, 0x5c, 0x6b, 0x7b, 0x72, 0xdf, 0x95, 0xb4,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_type_hash_t *
multi_floor_nav__srv__IntTrigger_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x8a, 0x86, 0xcd, 0x7d, 0x0f, 0xde, 0x60, 0xb5,
      0x1d, 0x25, 0xc7, 0xdf, 0x0e, 0x98, 0x65, 0xad,
      0x2d, 0x85, 0x04, 0xcd, 0x85, 0x83, 0xfd, 0x3c,
      0x05, 0x00, 0xc3, 0x5e, 0x47, 0xe8, 0xb2, 0x17,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_multi_floor_nav
const rosidl_type_hash_t *
multi_floor_nav__srv__IntTrigger_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xe8, 0x1b, 0xd3, 0xce, 0x8e, 0x7b, 0xf2, 0x63,
      0x45, 0xdd, 0x4f, 0x3a, 0x8e, 0x35, 0x6b, 0x7c,
      0xfc, 0xb7, 0x66, 0x33, 0xf1, 0x66, 0xb9, 0x8a,
      0xc3, 0xbe, 0xb8, 0x2d, 0x05, 0x0f, 0xa5, 0x2e,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "service_msgs/msg/detail/service_event_info__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char multi_floor_nav__srv__IntTrigger__TYPE_NAME[] = "multi_floor_nav/srv/IntTrigger";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char multi_floor_nav__srv__IntTrigger_Event__TYPE_NAME[] = "multi_floor_nav/srv/IntTrigger_Event";
static char multi_floor_nav__srv__IntTrigger_Request__TYPE_NAME[] = "multi_floor_nav/srv/IntTrigger_Request";
static char multi_floor_nav__srv__IntTrigger_Response__TYPE_NAME[] = "multi_floor_nav/srv/IntTrigger_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char multi_floor_nav__srv__IntTrigger__FIELD_NAME__request_message[] = "request_message";
static char multi_floor_nav__srv__IntTrigger__FIELD_NAME__response_message[] = "response_message";
static char multi_floor_nav__srv__IntTrigger__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field multi_floor_nav__srv__IntTrigger__FIELDS[] = {
  {
    {multi_floor_nav__srv__IntTrigger__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {multi_floor_nav__srv__IntTrigger_Request__TYPE_NAME, 38, 38},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {multi_floor_nav__srv__IntTrigger_Response__TYPE_NAME, 39, 39},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {multi_floor_nav__srv__IntTrigger_Event__TYPE_NAME, 36, 36},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription multi_floor_nav__srv__IntTrigger__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger_Event__TYPE_NAME, 36, 36},
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger_Request__TYPE_NAME, 38, 38},
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger_Response__TYPE_NAME, 39, 39},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
multi_floor_nav__srv__IntTrigger__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {multi_floor_nav__srv__IntTrigger__TYPE_NAME, 30, 30},
      {multi_floor_nav__srv__IntTrigger__FIELDS, 3, 3},
    },
    {multi_floor_nav__srv__IntTrigger__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = multi_floor_nav__srv__IntTrigger_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = multi_floor_nav__srv__IntTrigger_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = multi_floor_nav__srv__IntTrigger_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char multi_floor_nav__srv__IntTrigger_Request__FIELD_NAME__req_int[] = "req_int";

static rosidl_runtime_c__type_description__Field multi_floor_nav__srv__IntTrigger_Request__FIELDS[] = {
  {
    {multi_floor_nav__srv__IntTrigger_Request__FIELD_NAME__req_int, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
multi_floor_nav__srv__IntTrigger_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {multi_floor_nav__srv__IntTrigger_Request__TYPE_NAME, 38, 38},
      {multi_floor_nav__srv__IntTrigger_Request__FIELDS, 1, 1},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char multi_floor_nav__srv__IntTrigger_Response__FIELD_NAME__success[] = "success";
static char multi_floor_nav__srv__IntTrigger_Response__FIELD_NAME__message[] = "message";

static rosidl_runtime_c__type_description__Field multi_floor_nav__srv__IntTrigger_Response__FIELDS[] = {
  {
    {multi_floor_nav__srv__IntTrigger_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger_Response__FIELD_NAME__message, 7, 7},
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
multi_floor_nav__srv__IntTrigger_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {multi_floor_nav__srv__IntTrigger_Response__TYPE_NAME, 39, 39},
      {multi_floor_nav__srv__IntTrigger_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char multi_floor_nav__srv__IntTrigger_Event__FIELD_NAME__info[] = "info";
static char multi_floor_nav__srv__IntTrigger_Event__FIELD_NAME__request[] = "request";
static char multi_floor_nav__srv__IntTrigger_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field multi_floor_nav__srv__IntTrigger_Event__FIELDS[] = {
  {
    {multi_floor_nav__srv__IntTrigger_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {multi_floor_nav__srv__IntTrigger_Request__TYPE_NAME, 38, 38},
    },
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {multi_floor_nav__srv__IntTrigger_Response__TYPE_NAME, 39, 39},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription multi_floor_nav__srv__IntTrigger_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger_Request__TYPE_NAME, 38, 38},
    {NULL, 0, 0},
  },
  {
    {multi_floor_nav__srv__IntTrigger_Response__TYPE_NAME, 39, 39},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
multi_floor_nav__srv__IntTrigger_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {multi_floor_nav__srv__IntTrigger_Event__TYPE_NAME, 36, 36},
      {multi_floor_nav__srv__IntTrigger_Event__FIELDS, 3, 3},
    },
    {multi_floor_nav__srv__IntTrigger_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = multi_floor_nav__srv__IntTrigger_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = multi_floor_nav__srv__IntTrigger_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "int32 req_int\n"
  "---\n"
  "bool success\n"
  "string message";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
multi_floor_nav__srv__IntTrigger__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {multi_floor_nav__srv__IntTrigger__TYPE_NAME, 30, 30},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 46, 46},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
multi_floor_nav__srv__IntTrigger_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {multi_floor_nav__srv__IntTrigger_Request__TYPE_NAME, 38, 38},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
multi_floor_nav__srv__IntTrigger_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {multi_floor_nav__srv__IntTrigger_Response__TYPE_NAME, 39, 39},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
multi_floor_nav__srv__IntTrigger_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {multi_floor_nav__srv__IntTrigger_Event__TYPE_NAME, 36, 36},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
multi_floor_nav__srv__IntTrigger__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *multi_floor_nav__srv__IntTrigger__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *multi_floor_nav__srv__IntTrigger_Event__get_individual_type_description_source(NULL);
    sources[3] = *multi_floor_nav__srv__IntTrigger_Request__get_individual_type_description_source(NULL);
    sources[4] = *multi_floor_nav__srv__IntTrigger_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
multi_floor_nav__srv__IntTrigger_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *multi_floor_nav__srv__IntTrigger_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
multi_floor_nav__srv__IntTrigger_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *multi_floor_nav__srv__IntTrigger_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
multi_floor_nav__srv__IntTrigger_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *multi_floor_nav__srv__IntTrigger_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *multi_floor_nav__srv__IntTrigger_Request__get_individual_type_description_source(NULL);
    sources[3] = *multi_floor_nav__srv__IntTrigger_Response__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
