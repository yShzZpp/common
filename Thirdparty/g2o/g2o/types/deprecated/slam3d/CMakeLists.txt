add_library(${LIB_PREFIX}deprecated_types_slam3d ${G2O_LIB_TYPE}
  se3quat_gradients.cpp
  se3quat_gradients.h
  vertex_se3_quat.cpp
  vertex_se3_quat.h
  edge_se3_quat.cpp
  edge_se3_quat.h
  vertex_pointxyz.cpp
  vertex_pointxyz.h
  parameter_se3_offset.cpp
  parameter_se3_offset.h
  edge_se3_offset.cpp
  edge_se3_offset.h
  edge_se3_prior.cpp
  edge_se3_prior.h
  edge_se3_pointxyz.cpp
  edge_se3_pointxyz.h
  parameter_camera.cpp
  parameter_camera.h
  edge_se3_pointxyz_disparity.cpp
  edge_se3_pointxyz_disparity.h
  edge_se3_pointxyz_depth.cpp
  edge_se3_pointxyz_depth.h
  types_slam3d.cpp
  types_slam3d.h
  g2o_deprecated_types_slam3d_api.h
)

# target_link_libraries(${LIB_PREFIX}deprecated_types_slam3d ${LIB_PREFIX}core ${LIB_PREFIX}types_slam3d)

target_link_libraries(${LIB_PREFIX}deprecated_types_slam3d ${LIB_PREFIX}core ${LIB_PREFIX}types_slam3d)
  
if(G2O_HAVE_OPENGL)
  target_link_libraries(${LIB_PREFIX}deprecated_types_slam3d ${OPENGL_gl_LIBRARY})
endif()

install(TARGETS ${LIB_PREFIX}deprecated_types_slam3d
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/types/deprecated/slam3d)
