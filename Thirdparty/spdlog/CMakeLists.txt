cmake_minimum_required(VERSION 2.8.3)
project(spdlog)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED)

find_package(Boost REQUIRED)

catkin_package(
   INCLUDE_DIRS include
#  LIBRARIES spdlog
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

include_directories(
  include
  # ${catkin_INCLUDE_DIRS}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  # PATTERN ".svn" EXCLUDE
)

install(FILES include/cti_spdlog.h
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
)
