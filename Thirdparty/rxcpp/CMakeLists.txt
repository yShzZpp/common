cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(rxcpp)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED)

find_package(Boost REQUIRED)

catkin_package(
   INCLUDE_DIRS include
)

include_directories(
  include
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
