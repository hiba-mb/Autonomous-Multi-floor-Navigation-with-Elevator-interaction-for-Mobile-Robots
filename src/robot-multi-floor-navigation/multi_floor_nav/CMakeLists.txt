cmake_minimum_required(VERSION 3.8)
project(multi_floor_nav)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(angles REQUIRED)
find_package(action_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_library(PAHO_MQTT_C_LIB paho-mqtt3as REQUIRED)
find_library(PAHO_MQTT_CPP_LIB paho-mqttpp3 REQUIRED)
find_package(nav2_util REQUIRED)
# Generate custom service
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/IntTrigger.srv"
  "msg/MissionRequest.msg"
  DEPENDENCIES std_msgs
)

ament_export_dependencies(rosidl_default_runtime)
include_directories(include)
# change_map_node
add_executable(change_map_node src/change_map.cpp)
ament_target_dependencies(change_map_node
  rclcpp geometry_msgs nav_msgs std_msgs tf2 tf2_ros tf2_geometry_msgs action_msgs)
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(change_map_node "${cpp_typesupport_target}")
# multi_floor_navigation_node
add_executable(multi_floor_navigation_node src/multi_floor_navigation.cpp)
ament_target_dependencies(multi_floor_navigation_node
  rclcpp geometry_msgs nav_msgs std_msgs tf2 tf2_ros tf2_geometry_msgs angles action_msgs nav2_msgs)
target_link_libraries(multi_floor_navigation_node "${cpp_typesupport_target}")
# elevator_bt_executor
add_executable(elevator_bt_executor src/elevator_bt_executor.cpp)
ament_target_dependencies(elevator_bt_executor
  rclcpp rclcpp_action geometry_msgs nav_msgs std_msgs tf2 tf2_ros tf2_geometry_msgs nav2_msgs nav2_util behaviortree_cpp ament_index_cpp)
add_executable(robot_visibility_node src/robot_visibility_node.cpp)
ament_target_dependencies(robot_visibility_node rclcpp std_msgs)
target_link_libraries(elevator_bt_executor "${cpp_typesupport_target}" ${PAHO_MQTT_C_LIB} ${PAHO_MQTT_CPP_LIB})
install(TARGETS
  change_map_node
  multi_floor_navigation_node
  elevator_bt_executor
  robot_visibility_node
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch maps configs behavior_trees
  DESTINATION share/${PROJECT_NAME})
install(PROGRAMS
  src/scan_reframer_node.py
  src/odom_covariance_fixer.py
  src/fleet_manager.py
  DESTINATION lib/${PROJECT_NAME}
)
install(
  DIRECTORY include/
  DESTINATION include/
)
ament_package()
