// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from multi_floor_nav:srv/IntTrigger.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "multi_floor_nav/srv/int_trigger.hpp"


#ifndef MULTI_FLOOR_NAV__SRV__DETAIL__INT_TRIGGER__BUILDER_HPP_
#define MULTI_FLOOR_NAV__SRV__DETAIL__INT_TRIGGER__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "multi_floor_nav/srv/detail/int_trigger__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace multi_floor_nav
{

namespace srv
{

namespace builder
{

class Init_IntTrigger_Request_req_int
{
public:
  Init_IntTrigger_Request_req_int()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::multi_floor_nav::srv::IntTrigger_Request req_int(::multi_floor_nav::srv::IntTrigger_Request::_req_int_type arg)
  {
    msg_.req_int = std::move(arg);
    return std::move(msg_);
  }

private:
  ::multi_floor_nav::srv::IntTrigger_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::multi_floor_nav::srv::IntTrigger_Request>()
{
  return multi_floor_nav::srv::builder::Init_IntTrigger_Request_req_int();
}

}  // namespace multi_floor_nav


namespace multi_floor_nav
{

namespace srv
{

namespace builder
{

class Init_IntTrigger_Response_message
{
public:
  explicit Init_IntTrigger_Response_message(::multi_floor_nav::srv::IntTrigger_Response & msg)
  : msg_(msg)
  {}
  ::multi_floor_nav::srv::IntTrigger_Response message(::multi_floor_nav::srv::IntTrigger_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::multi_floor_nav::srv::IntTrigger_Response msg_;
};

class Init_IntTrigger_Response_success
{
public:
  Init_IntTrigger_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_IntTrigger_Response_message success(::multi_floor_nav::srv::IntTrigger_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_IntTrigger_Response_message(msg_);
  }

private:
  ::multi_floor_nav::srv::IntTrigger_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::multi_floor_nav::srv::IntTrigger_Response>()
{
  return multi_floor_nav::srv::builder::Init_IntTrigger_Response_success();
}

}  // namespace multi_floor_nav


namespace multi_floor_nav
{

namespace srv
{

namespace builder
{

class Init_IntTrigger_Event_response
{
public:
  explicit Init_IntTrigger_Event_response(::multi_floor_nav::srv::IntTrigger_Event & msg)
  : msg_(msg)
  {}
  ::multi_floor_nav::srv::IntTrigger_Event response(::multi_floor_nav::srv::IntTrigger_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::multi_floor_nav::srv::IntTrigger_Event msg_;
};

class Init_IntTrigger_Event_request
{
public:
  explicit Init_IntTrigger_Event_request(::multi_floor_nav::srv::IntTrigger_Event & msg)
  : msg_(msg)
  {}
  Init_IntTrigger_Event_response request(::multi_floor_nav::srv::IntTrigger_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_IntTrigger_Event_response(msg_);
  }

private:
  ::multi_floor_nav::srv::IntTrigger_Event msg_;
};

class Init_IntTrigger_Event_info
{
public:
  Init_IntTrigger_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_IntTrigger_Event_request info(::multi_floor_nav::srv::IntTrigger_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_IntTrigger_Event_request(msg_);
  }

private:
  ::multi_floor_nav::srv::IntTrigger_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::multi_floor_nav::srv::IntTrigger_Event>()
{
  return multi_floor_nav::srv::builder::Init_IntTrigger_Event_info();
}

}  // namespace multi_floor_nav

#endif  // MULTI_FLOOR_NAV__SRV__DETAIL__INT_TRIGGER__BUILDER_HPP_
