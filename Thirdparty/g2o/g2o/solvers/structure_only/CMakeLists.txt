add_library(${LIB_PREFIX}solver_structure_only ${G2O_LIB_TYPE}
  structure_only.cpp
  structure_only_solver.h
)

#set_target_properties(solver_structure_only PROPERTIES OUTPUT_NAME ${LIB_PREFIX}solver_structure_only)

target_link_libraries(${LIB_PREFIX}solver_structure_only ${LIB_PREFIX}core)

install(TARGETS ${LIB_PREFIX}solver_structure_only
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/solvers/structure_only)
