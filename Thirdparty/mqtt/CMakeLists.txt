cmake_minimum_required(VERSION 3.0.2)
project(mqtt)

add_compile_options(-std=c++14)

find_package(catkin REQUIRED COMPONENTS)

catkin_package(
  INCLUDE_DIRS include
  #直接写对外依赖此库的名称
  LIBRARIES libpaho-mqtt3c.so
)

set(mqtt_LOCATION ${PROJECT_SOURCE_DIR}/lib/)
set(mqtt_DESTINATION ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_LIB_DESTINATION})

# include_directories(
#   include
# )
# link_directories(${PROJECT_SOURCE_DIR}/lib)
#这里必须设定一个target,如没有则会出现catkin_package()中的LIBRARIES尚未编译或安装
#目前先注释掉再分析 
# add_library(${PROJECT_NAME} SHARED IMPORTED)
# set_property(TARGET ${PROJECT_NAME} PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib)

#引入的第三方库catkin无法自动将其拷贝到devel/lib下, 必须手动拷贝
file(COPY ${mqtt_LOCATION}
  DESTINATION ${mqtt_DESTINATION}
)

install(DIRECTORY
  lib/
  DESTINATION ${CATKIN_DEVEL_PREFIX}/lib
)

# install
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(DIRECTORY lib/
	DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

#这里尚不清楚依赖此package的其他程序运行时为什么需要libpaho-mqtt3c.so.1
#猜测:有可能跟libpaho-mqtt3c.so编译时的生成软连接有关
#后续有时间可继续查找此问题
# install(FILES lib/libpaho-mqtt3c.so
#   DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RENAME "libpaho-mqtt3c.so.1"  
# )
