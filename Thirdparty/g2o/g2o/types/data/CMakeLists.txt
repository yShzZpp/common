add_library(${LIB_PREFIX}types_data ${G2O_LIB_TYPE}
  types_data.cpp
  robot_data.cpp robot_data.h
  vertex_tag.cpp vertex_tag.h
  vertex_ellipse.cpp vertex_ellipse.h
  laser_parameters.cpp laser_parameters.h
  raw_laser.cpp raw_laser.h
  robot_laser.cpp robot_laser.h
  data_queue.cpp data_queue.h
  g2o_types_data_api.h
)

#set_target_properties(types_data PROPERTIES OUTPUT_NAME ${LIB_PREFIX}types_data)

target_link_libraries(${LIB_PREFIX}types_data ${LIB_PREFIX}core ${LIB_PREFIX}types_slam2d)

if(G2O_HAVE_OPENGL)
  target_link_libraries(${LIB_PREFIX}types_data freeglut_minimal ${LIB_PREFIX}opengl_helper)
endif()

install(TARGETS ${LIB_PREFIX}types_data
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/types/data)
