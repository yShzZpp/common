cmake_minimum_required(VERSION 2.8.3)
project(vectormap_msgs)

find_package(catkin REQUIRED COMPONENTS
	std_msgs
	message_generation
	geometry_msgs
)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall ${CMAKE_CXX_FLAGS}")

add_message_files(
	FILES
#	Lane.msg
#	LaneArray.msg
	RoadEdge.msg
	RoadEdgeArray.msg
)

generate_messages(
	DEPENDENCIES
	std_msgs
	geometry_msgs
)

catkin_package(
	CATKIN_DEPENDS message_runtime std_msgs 
)
