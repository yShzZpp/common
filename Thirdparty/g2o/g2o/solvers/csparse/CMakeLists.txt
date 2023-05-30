
include_directories(${CSPARSE_INCLUDE_DIR})

add_library(${LIB_PREFIX}csparse_extension ${G2O_LGPL_LIB_TYPE}
  csparse_helper.cpp csparse_helper.h
  g2o_csparse_extension_api.h
)

#set_target_properties(csparse_extension PROPERTIES OUTPUT_NAME ${LIB_PREFIX}csparse_extension)

if(BUILD_CSPARSE)
  target_link_libraries(${LIB_PREFIX}csparse_extension csparse ${G2O_EIGEN3_EIGEN_TARGET})
else()
  target_link_libraries(${LIB_PREFIX}csparse_extension ${CSPARSE_LIBRARY} ${G2O_EIGEN3_EIGEN_TARGET})
endif()

add_library(${LIB_PREFIX}solver_csparse ${G2O_LIB_TYPE}
  solver_csparse.cpp
  linear_solver_csparse.h
  g2o_csparse_api.h
)

#set_target_properties(solver_csparse PROPERTIES OUTPUT_NAME ${LIB_PREFIX}solver_csparse)

target_link_libraries(${LIB_PREFIX}solver_csparse
  ${LIB_PREFIX}csparse_extension
  ${LIB_PREFIX}core
)


install(TARGETS ${LIB_PREFIX}solver_csparse ${LIB_PREFIX}csparse_extension
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/solvers/csparse)
