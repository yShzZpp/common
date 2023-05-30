
add_library(${LIB_PREFIX}solver_cholmod ${G2O_LIB_TYPE}
  solver_cholmod.cpp
  linear_solver_cholmod.h
)

include_directories(${CHOLMOD_INCLUDE_DIR})

#set_target_properties(solver_cholmod PROPERTIES OUTPUT_NAME ${LIB_PREFIX}solver_cholmod)


add_definitions(${BLAS_DEFINITIONS} ${LAPACK_DEFINITIONS})
target_link_libraries(${LIB_PREFIX}solver_cholmod ${CHOLMOD_LIBRARIES} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${LIB_PREFIX}core)

install(TARGETS ${LIB_PREFIX}solver_cholmod
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/solvers/cholmod)
