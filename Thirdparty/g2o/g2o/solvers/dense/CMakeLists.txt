add_library(${LIB_PREFIX}solver_dense ${G2O_LIB_TYPE}
  solver_dense.cpp linear_solver_dense.h
)

#set_target_properties(solver_dense PROPERTIES OUTPUT_NAME ${LIB_PREFIX}solver_dense)

target_link_libraries(${LIB_PREFIX}solver_dense ${LIB_PREFIX}core)

install(TARGETS ${LIB_PREFIX}solver_dense
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/solvers/dense)
