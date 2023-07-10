cmake_minimum_required(VERSION 2.8.3)
project(yong_unmanned_sol_udp_client)

# find_package(catkin REQUIRED COMPONENTS)


# catkin_package(
#   INCLUDE_DIRS include
#   LIBRARIES client_comm_utm_lib
#   CATKIN_DEPENDS roscpp std_msgs
# )

include_directories(
	include

)

add_executable(udp_ros_client src/client_node.cpp)
target_link_libraries(udp_ros_client

)