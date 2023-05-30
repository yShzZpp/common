cmake_minimum_required(VERSION 2.8.3)
project(cti_record_log)

add_compile_options(-std=c++14)

find_package(catkin REQUIRED COMPONENTS
  rospy
  cti_msgs
  roscpp
  save_crash_log
  spdlog
  upload_crash_log
  share_handle_storage
)

find_package(Boost REQUIRED)

catkin_package()

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  src
)

add_executable(cti_bag_monitor src/monitor.cpp)

add_dependencies(cti_bag_monitor ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(cti_bag_monitor
  ${catkin_LIBRARIES}
	cpprest
)

install(TARGETS cti_bag_monitor
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

catkin_install_python(PROGRAMS
  scripts/record.py
  scripts/monitor.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
        PATTERN ".svn" EXCLUDE)

install(FILES 
        config/cti_record_config.yaml
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config)
