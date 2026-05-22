cmake_minimum_required(VERSION 3.8)
project(jackal_elevator)

find_package(ament_cmake REQUIRED)

install(
  DIRECTORY
    launch
    worlds
    urdf
    rviz
  DESTINATION share/${PROJECT_NAME}
)
install(
  PROGRAMS
    scripts/goal_relay.py
    scripts/elevator_mqtt_bridge.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()

