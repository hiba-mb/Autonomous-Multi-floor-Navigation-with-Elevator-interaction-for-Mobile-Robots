// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from multi_floor_nav:msg/MissionRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "multi_floor_nav/msg/mission_request.hpp"


#ifndef MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__STRUCT_HPP_
#define MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__multi_floor_nav__msg__MissionRequest __attribute__((deprecated))
#else
# define DEPRECATED__multi_floor_nav__msg__MissionRequest __declspec(deprecated)
#endif

namespace multi_floor_nav
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct MissionRequest_
{
  using Type = MissionRequest_<ContainerAllocator>;

  explicit MissionRequest_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->robot_id = "";
      this->x = 0.0;
      this->y = 0.0;
      this->yaw = 0.0;
      this->target_floor = 0l;
      this->mission_id = "";
    }
  }

  explicit MissionRequest_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : robot_id(_alloc),
    mission_id(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->robot_id = "";
      this->x = 0.0;
      this->y = 0.0;
      this->yaw = 0.0;
      this->target_floor = 0l;
      this->mission_id = "";
    }
  }

  // field types and members
  using _robot_id_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _robot_id_type robot_id;
  using _x_type =
    double;
  _x_type x;
  using _y_type =
    double;
  _y_type y;
  using _yaw_type =
    double;
  _yaw_type yaw;
  using _target_floor_type =
    int32_t;
  _target_floor_type target_floor;
  using _mission_id_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _mission_id_type mission_id;

  // setters for named parameter idiom
  Type & set__robot_id(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->robot_id = _arg;
    return *this;
  }
  Type & set__x(
    const double & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const double & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__yaw(
    const double & _arg)
  {
    this->yaw = _arg;
    return *this;
  }
  Type & set__target_floor(
    const int32_t & _arg)
  {
    this->target_floor = _arg;
    return *this;
  }
  Type & set__mission_id(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->mission_id = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    multi_floor_nav::msg::MissionRequest_<ContainerAllocator> *;
  using ConstRawPtr =
    const multi_floor_nav::msg::MissionRequest_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<multi_floor_nav::msg::MissionRequest_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<multi_floor_nav::msg::MissionRequest_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      multi_floor_nav::msg::MissionRequest_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<multi_floor_nav::msg::MissionRequest_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      multi_floor_nav::msg::MissionRequest_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<multi_floor_nav::msg::MissionRequest_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<multi_floor_nav::msg::MissionRequest_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<multi_floor_nav::msg::MissionRequest_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__multi_floor_nav__msg__MissionRequest
    std::shared_ptr<multi_floor_nav::msg::MissionRequest_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__multi_floor_nav__msg__MissionRequest
    std::shared_ptr<multi_floor_nav::msg::MissionRequest_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MissionRequest_ & other) const
  {
    if (this->robot_id != other.robot_id) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->yaw != other.yaw) {
      return false;
    }
    if (this->target_floor != other.target_floor) {
      return false;
    }
    if (this->mission_id != other.mission_id) {
      return false;
    }
    return true;
  }
  bool operator!=(const MissionRequest_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MissionRequest_

// alias to use template instance with default allocator
using MissionRequest =
  multi_floor_nav::msg::MissionRequest_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace multi_floor_nav

#endif  // MULTI_FLOOR_NAV__MSG__DETAIL__MISSION_REQUEST__STRUCT_HPP_
