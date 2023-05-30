add_library(${LIB_PREFIX}types_icp ${G2O_LIB_TYPE}
  types_icp.cpp  types_icp.h
  g2o_types_icp_api.h
)

#set_target_properties(types_icp PROPERTIES OUTPUT_NAME ${LIB_PREFIX}types_icp)

target_link_libraries(${LIB_PREFIX}types_icp ${LIB_PREFIX}types_sba ${LIB_PREFIX}types_slam3d)

install(TARGETS ${LIB_PREFIX}types_icp
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/types/icp)
