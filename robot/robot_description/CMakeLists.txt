cmake_minimum_required(VERSION 3.0.2)
project(robot_description)

add_compile_options(-std=c++11)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_module)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
  roslib
  common
)

find_package(ProtoBuf REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(YAMLCPP yaml-cpp REQUIRED)
if(YAMLCPP_VERSION VERSION_GREATER "0.5.0")
    add_definitions(-DHAVE_YAMLCPP_GT_0_5_0)
endif()

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/proto/*.proto")
cti_protobuf_generate_cpp(${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/proto
	RobotDescriptionrcs
	RobotDescriptionHeaders
	${ProtoFiles}
)

catkin_package(
#  INCLUDE_DIRS include
  #LIBRARIES robot_description
  CATKIN_DEPENDS roscpp tf common
#  DEPENDS system_lib
)


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${PROTOBUF_INCLUDE_DIR}
  ${YAMLCPP_INCLUDE_DIRS}
)

add_library(static_transform
  ${RobotDescriptionrcs}
  ${RobotDescriptionHeaders}
  src/transform_sender.cc
  src/static_transform_component.cc
)
add_dependencies(static_transform ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(static_transform
  ${catkin_LIBRARIES}
	${PROTOBUF_LIBRARIES}
  ${YAMLCPP_LIBRARIES}
)

add_executable(static_transform_node
  node/static_transform_node.cc
)
add_dependencies(static_transform_node 
  ${${PROJECT_NAME}_EXPORTED_TARGETS} 
  ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(static_transform_node
  static_transform
  ${catkin_LIBRARIES}
	${PROTOBUF_LIBRARIES}
  ${YAMLCPP_LIBRARIES}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# catkin_install_python(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
install(TARGETS static_transform_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
install(TARGETS static_transform
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
install(DIRECTORY launch config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
