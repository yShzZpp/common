cmake_minimum_required(VERSION 2.8.3)
project(sqlite)

add_compile_options(-std=c++14)

find_package(catkin REQUIRED)

catkin_package(
    INCLUDE_DIRS include
    # DEPENDS sqlite3
    LIBRARIES libsqlite3.so
)

include_directories(
    include
)

install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
