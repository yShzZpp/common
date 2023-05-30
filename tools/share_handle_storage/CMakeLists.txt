cmake_minimum_required(VERSION 2.8.3)
project(share_handle_storage)

add_compile_options(-std=c++14)

find_package(catkin REQUIRED
  roscpp
  cti_msgs
  geometry_msgs
  std_msgs
  common
  spdlog
  nav_msgs
)

find_package(Boost REQUIRED)
find_package(OpenCV 3 REQUIRED)

catkin_package(
   INCLUDE_DIRS include
   LIBRARIES ${PROJECT_NAME}
   CATKIN_DEPENDS roscpp cti_msgs geometry_msgs std_msgs spdlog common
   DEPENDS Boost crypto cpprest
)

include_directories(
	include
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${CPPREST_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} 
  src/BoostTool.cpp
  src/JsonToMsg.cpp
  src/MD5.cpp
  src/MsgToJson.cpp
  src/MsgToProto.cpp
  src/ProtoToMsg.cpp
  src/TaskOperation.cpp
  src/CommonTool.cpp
  src/UniversalServiceClient.cpp
)

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} )

target_link_libraries(${PROJECT_NAME} 
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
  cpprest
)

install(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
