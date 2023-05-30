add_library(freeglut_minimal ${G2O_LIB_TYPE}
  freeglut_font.cpp
  freeglut_stroke_mono_roman.cpp
  freeglut_stroke_roman.cpp
)

target_link_libraries(freeglut_minimal ${G2O_OPENGL_TARGET} ${G2O_EIGEN3_EIGEN_TARGET})

set_target_properties(freeglut_minimal PROPERTIES OUTPUT_NAME ${LIB_PREFIX}ext_freeglut_minimal)

install(TARGETS freeglut_minimal
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/freeglut)
