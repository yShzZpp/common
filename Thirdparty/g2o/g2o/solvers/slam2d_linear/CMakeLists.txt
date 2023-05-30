
add_library(${LIB_PREFIX}solver_slam2d_linear ${G2O_LIB_TYPE}
  slam2d_linear.cpp
  solver_slam2d_linear.h solver_slam2d_linear.cpp
  g2o_slam2d_linear_api.h
)

include_directories(${CSPARSE_INCLUDE_DIR})

#set_target_properties(solver_slam2d_linear PROPERTIES OUTPUT_NAME ${LIB_PREFIX}solver_slam2d_linear)

target_link_libraries(${LIB_PREFIX}solver_slam2d_linear ${LIB_PREFIX}solver_csparse ${LIB_PREFIX}types_slam2d)

install(TARGETS ${LIB_PREFIX}solver_slam2d_linear
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/solvers/slam2d_linear)
