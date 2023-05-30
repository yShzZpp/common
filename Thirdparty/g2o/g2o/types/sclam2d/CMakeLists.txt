add_library(${LIB_PREFIX}types_sclam2d ${G2O_LIB_TYPE}
  edge_se2_sensor_calib.cpp edge_se2_sensor_calib.h
  vertex_odom_differential_params.cpp vertex_odom_differential_params.h
  edge_se2_odom_differential_calib.cpp edge_se2_odom_differential_calib.h
  odometry_measurement.cpp odometry_measurement.h
  types_sclam2d.cpp types_sclam2d.h
  g2o_types_sclam2d_api.h
)

#set_target_properties(types_sclam2d PROPERTIES OUTPUT_NAME ${LIB_PREFIX}types_sclam2d)

target_link_libraries(${LIB_PREFIX}types_sclam2d ${LIB_PREFIX}types_slam2d ${LIB_PREFIX}core)

if(G2O_HAVE_OPENGL)
  target_link_libraries(${LIB_PREFIX}types_sclam2d ${OPENGL_gl_LIBRARY})
endif()

install(TARGETS ${LIB_PREFIX}types_sclam2d
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/types/sclam2d)
