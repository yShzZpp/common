cmake_minimum_required(VERSION 2.8.3)
project(serialport)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
#    roscpp
#    sensor_msgs
#   serial
#	geometry_msgs
#	tf
#	cti_msgs
#	spdlog
)

# find_package(Boost REQUIRED COMPONENTS system)

# catkin_python_setup()

catkin_package(
    LIBRARIES serialport
    # CATKIN_DEPENDS roscpp sensor_msgs serial geometry_msgs tf cti_msgs
	# CATKIN_DEPENDS roscpp sensor_msgs geometry_msgs tf cti_msgs spdlog
)

# FIND_LIBRARY(YAML_CPP_LIBRARIES yaml-cpp)
# if(NOT YAML_CPP_LIBRARIES)
#	FIND_PACKAGE(yaml-cpp REQUIRED)
#	INCLUDE_DIRECTORIES(${YAML_CPP_INCLUDE_DIRS})
# endif(NOT YAML_CPP_LIBRARIES)

include_directories(
	include
	include/serialport
	${catkin_INCLUDE_DIRS}
)

add_library(serialport src/serialport.cpp)

add_dependencies(serialport 
	${catkin_EXPORTED_TARGETS}
)

install(TARGETS serialport
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
)

# install(DIRECTORY launch param 
#	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )
