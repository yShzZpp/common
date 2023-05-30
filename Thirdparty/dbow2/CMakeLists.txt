cmake_minimum_required(VERSION 2.8.3)
project(dbow2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -O3 -march=native")

find_package(catkin REQUIRED)

find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

# set(ABSL_PUBLIC_LIBRARIES
#   dbow2)

catkin_package(
  INCLUDE_DIRS .
  LIBRARIES dbow2
)

add_subdirectory(dbow2)
