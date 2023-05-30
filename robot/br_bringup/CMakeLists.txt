cmake_minimum_required(VERSION 2.8.3)
project(br_bringup)

find_package(catkin REQUIRED COMPONENTS)

catkin_package()

install(DIRECTORY launch param scripts rules
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
