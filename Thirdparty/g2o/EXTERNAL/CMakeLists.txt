# Only build CSPARSE if we need to
if(BUILD_CSPARSE)
  add_subdirectory(csparse)
endif()

if (G2O_HAVE_OPENGL)
  add_subdirectory(freeglut)
endif()
