add_library(${LIB_PREFIX}types_sba ${G2O_LIB_TYPE}
  types_sba.h     types_six_dof_expmap.h
  types_sba.cpp   types_six_dof_expmap.cpp
  g2o_types_sba_api.h
)

#set_target_properties(types_sba PROPERTIES OUTPUT_NAME ${LIB_PREFIX}types_sba)

target_link_libraries(${LIB_PREFIX}types_sba ${LIB_PREFIX}core ${LIB_PREFIX}types_slam3d)

install(TARGETS ${LIB_PREFIX}types_sba
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/types/sba)
