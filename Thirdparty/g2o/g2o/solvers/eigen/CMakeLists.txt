INCLUDE_DIRECTORIES(${G2O_EIGEN3_INCLUDE})

add_library(${LIB_PREFIX}solver_eigen ${G2O_LIB_TYPE}
  solver_eigen.cpp
  linear_solver_eigen.h
)
#set_target_properties(solver_eigen PROPERTIES OUTPUT_NAME ${LIB_PREFIX}solver_eigen)

target_link_libraries(${LIB_PREFIX}solver_eigen ${LIB_PREFIX}core)

install(TARGETS ${LIB_PREFIX}solver_eigen
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/solvers/eigen)

