add_library(${LIB_PREFIX}types_slam2d ${G2O_LIB_TYPE}
  se2.h
  edge_se2_pointxy_bearing.h  edge_se2_prior.h
  edge_se2.cpp                  edge_se2_pointxy_calib.cpp  types_slam2d.cpp
  edge_se2.h                    edge_se2_pointxy_calib.h    vertex_point_xy.cpp
  edge_se2_pointxy.cpp        vertex_point_xy.h
  edge_se2_pointxy.h          vertex_se2.cpp
  edge_se2_pointxy_bearing.cpp  edge_se2_prior.cpp          vertex_se2.h
  parameter_se2_offset.cpp     parameter_se2_offset.h
  edge_se2_offset.cpp          edge_se2_offset.h
  edge_se2_pointxy_offset.cpp  edge_se2_pointxy_offset.h
  edge_se2_xyprior.cpp edge_se2_xyprior.h
  edge_pointxy.cpp             edge_pointxy.h
  edge_se2_twopointsxy.cpp	edge_se2_twopointsxy.h
  edge_se2_lotsofxy.cpp		edge_se2_lotsofxy.h
  g2o_types_slam2d_api.h
)

#set_target_properties(types_slam2d PROPERTIES OUTPUT_NAME ${LIB_PREFIX}types_slam2d)

target_link_libraries(${LIB_PREFIX}types_slam2d ${LIB_PREFIX}core)

if(G2O_HAVE_OPENGL)
  target_link_libraries(${LIB_PREFIX}types_slam2d ${LIB_PREFIX}opengl_helper ${OPENGL_gl_LIBRARY})
endif()

install(TARGETS ${LIB_PREFIX}types_slam2d
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/types/slam2d)
