cmake_minimum_required(VERSION 2.8.3)
project(cti_wifi_sdk)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	spdlog
)

# find_package(Boost REQUIRED COMPONENTS system)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBNM libnm REQUIRED)
pkg_check_modules(UUID uuid REQUIRED)

catkin_package(
	INCLUDE_DIRS include
    LIBRARIES ctiwifisdk
    # CATKIN_DEPENDS
	DEPENDS libnm uuid
)

include_directories(
	include
	${catkin_INCLUDE_DIRS}
	${LIBNM_INCLUDE_DIRS}
    ${UUID_INCLUDE_DIRS}
)

## ctiwifisdk lib
add_library(ctiwifisdk
	src/wifi.cpp
	src/ap.cpp
	src/dbus.cpp
	src/misc.cpp
	src/wifi-fun.c
)

add_dependencies(ctiwifisdk ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(ctiwifisdk
	${catkin_LIBRARIES}
	${LIBNM_LIBRARIES}
	${UUID_LIBRARIES}
)

add_executable(test_wifi
			  src/test.cpp
              )

add_dependencies(test_wifi ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(test_wifi
  ${catkin_LIBRARIES}
  ctiwifisdk
)

install(TARGETS ctiwifisdk test_wifi
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

