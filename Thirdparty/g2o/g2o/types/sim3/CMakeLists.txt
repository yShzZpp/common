add_library(${LIB_PREFIX}types_sim3 ${G2O_LIB_TYPE}
  types_seven_dof_expmap.cpp
  types_seven_dof_expmap.h
)

#set_target_properties(types_sim3 PROPERTIES OUTPUT_NAME ${LIB_PREFIX}types_sim3)

target_link_libraries(${LIB_PREFIX}types_sim3 ${LIB_PREFIX}types_sba)

install(TARGETS ${LIB_PREFIX}types_sim3
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/types/sim3)

