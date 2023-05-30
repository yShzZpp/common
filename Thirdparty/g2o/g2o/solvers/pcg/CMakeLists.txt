add_library(${LIB_PREFIX}solver_pcg ${G2O_LIB_TYPE}
  solver_pcg.cpp
  linear_solver_pcg.h  
  linear_solver_pcg.hpp
)

#set_target_properties(solver_pcg PROPERTIES OUTPUT_NAME ${LIB_PREFIX}solver_pcg)

target_link_libraries(${LIB_PREFIX}solver_pcg ${LIB_PREFIX}core)

install(TARGETS ${LIB_PREFIX}solver_pcg
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/solvers/pcg)
