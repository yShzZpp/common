cmake_minimum_required(VERSION 2.8.3)
project(httplib)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(catkin REQUIRED)

# find_package(Boost REQUIRED COMPONENTS system)


catkin_package(
    INCLUDE_DIRS include
#  LIBRARIES json
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

#include_directories(
#  include
# ${catkin_INCLUDE_DIRS}
#)

install(DIRECTORY include/
   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}/../
   FILES_MATCHING PATTERN "*.h"
   PATTERN ".svn" EXCLUDE
)
