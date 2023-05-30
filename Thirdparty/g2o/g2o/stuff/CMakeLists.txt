add_library(${LIB_PREFIX}stuff ${G2O_LIB_TYPE}
  filesys_tools.h    string_tools.h
  color_macros.h     macros.h         timeutil.cpp
  command_args.cpp   misc.h           sparse_helper.cpp  timeutil.h
  command_args.h     os_specific.c    sparse_helper.h
  filesys_tools.cpp  os_specific.h    string_tools.cpp
  property.cpp       property.h       
  sampler.cpp        sampler.h        unscented.h
  tictoc.cpp tictoc.h
  g2o_stuff_api.h
)

#set_target_properties(stuff PROPERTIES OUTPUT_NAME ${LIB_PREFIX}stuff)

target_link_libraries(${LIB_PREFIX}stuff ${G2O_EIGEN3_EIGEN_TARGET})

install(TARGETS ${LIB_PREFIX}stuff
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# build our OpenGL helper library
if(OPENGL_FOUND AND G2O_HAVE_OPENGL)
  add_library(${LIB_PREFIX}opengl_helper ${G2O_LIB_TYPE}
    opengl_primitives.cpp opengl_primitives.h
  )

  install(TARGETS ${LIB_PREFIX}opengl_helper
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

  target_link_libraries(${LIB_PREFIX}opengl_helper PUBLIC ${G2O_OPENGL_TARGET} ${G2O_EIGEN3_EIGEN_TARGET})
#  set_target_properties(opengl_helper PROPERTIES OUTPUT_NAME "${LIB_PREFIX}opengl_helper")

endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND NOT ANDROID)
  target_link_libraries(${LIB_PREFIX}stuff rt)
endif()

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/stuff)
