cmake_minimum_required(VERSION 2.8.3)
project(g2o)

find_package(catkin REQUIRED)

#find_package(Cholmod REQUIRED)
# find_package(Eigen3 REQUIRED)

catkin_package(
  INCLUDE_DIRS .
  LIBRARIES 
    g2o_core g2o_csparse_extension g2o_solver_cholmod
    g2o_solver_csparse g2o_solver_dense g2o_solver_eigen
    g2o_solver_pcg g2o_solver_slam2d_linear g2o_solver_structure_only
    g2o_stuff g2o_types_data g2o_types_icp
    g2o_types_sba g2o_types_sclam2d g2o_types_sim3 
    g2o_types_slam2d g2o_types_slam2d_addons
    g2o_types_slam3d g2o_types_slam3d_addons
)

include_directories(
  include
  ${EIGEN3_INCLUDE_DIR}
  ${CHOLMOD_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}
)

# set(g2o_SOURCE_DIR ${PROJECT_SOURCE_DIR}/g2o)
# The library prefix
set(LIB_PREFIX g2o_)
set(g2o_C_FLAGS)
set(g2o_CXX_FLAGS)

# default built type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "BUILD TYPE:" ${CMAKE_BUILD_TYPE})

# postfix, based on type
set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "postfix applied to debug build of libraries")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "postfix applied to release build of libraries")
set(CMAKE_RELWITHDEBINFO_POSTFIX "_rd" CACHE STRING "postfix applied to release-with-debug-information libraries")
set(CMAKE_MINSIZEREL_POSTFIX "_s" CACHE STRING "postfix applied to minimium-size-build libraries")

# work out the postfix; required where we use OUTPUT_NAME
if(CMAKE_BUILD_TYPE MATCHES Release)
  set(EXE_POSTFIX)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
  set(EXE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  set(EXE_POSTFIX ${CMAKE_RELWITHDEBINFO_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
  set(EXE_POSTFIX ${CMAKE_MINSIZEREL_POSTFIX})
endif(CMAKE_BUILD_TYPE MATCHES Release)

# Allow the developer to select if Dynamic or Static libraries are built
option (BUILD_SHARED_LIBS "Build Shared Libraries (preferred and required for the g2o plugin system)" ON)
set (G2O_LIB_TYPE STATIC)
if (BUILD_SHARED_LIBS)
  set (G2O_LIB_TYPE SHARED)
endif()

# There seems to be an issue with MSVC8
# see http://eigen.tuxfamily.org/bz/show_bug.cgi?id=83
if(MSVC90)
  add_definitions(-DEIGEN_DONT_ALIGN_STATICALLY=1)
  message(STATUS "Disabling memory alignment for MSVC8")
endif(MSVC90)

# Set the output directory for the build executables and libraries
set(g2o_RUNTIME_OUTPUT_DIRECTORY ${g2o_SOURCE_DIR}/bin CACHE PATH "Target for the binaries")
if(WIN32)
  set(g2o_LIBRARY_OUTPUT_DIRECTORY ${g2o_SOURCE_DIR}/bin CACHE PATH "Target for the libraries")
else(WIN32)
  set(g2o_LIBRARY_OUTPUT_DIRECTORY ${g2o_SOURCE_DIR}/lib CACHE PATH "Target for the libraries")
endif(WIN32)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${g2o_LIBRARY_OUTPUT_DIRECTORY})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${g2o_LIBRARY_OUTPUT_DIRECTORY})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${g2o_RUNTIME_OUTPUT_DIRECTORY})

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)


# Detect OS and define macros appropriately
if(WIN32)
  add_definitions(-DWINDOWS)
  message(STATUS "Compiling on Windows")
elseif(CYGWIN)
  message(STATUS "Compiling on Cygwin")
  add_definitions(-DCYGWIN)
elseif(APPLE)
  add_definitions(-DUNIX)
  message(STATUS "Compiling on OSX")
elseif(UNIX)
  add_definitions(-DUNIX)
  message(STATUS "Compiling on Unix")
endif(WIN32)

# For building the CHOLMOD / CSPARSE solvers
find_package(Cholmod)
find_package(BLAS)
find_package(LAPACK)

# CHOLMOD
option (G2O_USE_CHOLMOD "Build g2o with CHOLMOD support" ON)
if(G2O_USE_CHOLMOD AND CHOLMOD_FOUND AND BLAS_FOUND AND LAPACK_FOUND)
  message(STATUS "Found CHOLMOD and its dependencies")
  set(CHOLMOD_FOUND TRUE)
else()
  set(CHOLMOD_FOUND FALSE)
endif()

# CSPARSE
option (G2O_USE_CSPARSE "Build g2o with CSParse support" ON)
find_package(CSparse)
if (G2O_USE_CSPARSE)
  if(CSPARSE_FOUND)
    set(BUILD_CSPARSE OFF CACHE BOOL "Build local CSparse library")
  else(CSPARSE_FOUND)
    set(BUILD_CSPARSE ON CACHE BOOL "Build local CSparse library")
    if(BUILD_CSPARSE)
      set(CSPARSE_FOUND TRUE)
    endif()
  endif(CSPARSE_FOUND)
else(G2O_USE_CSPARSE)
  set(BUILD_CSPARSE OFF "Build local CSparse library")
endif(G2O_USE_CSPARSE)

option(BUILD_LGPL_SHARED_LIBS "Build LGPL Code as Shared Libraries (LGPL Code)" ON)
set (G2O_LGPL_LIB_TYPE STATIC)
if (BUILD_LGPL_SHARED_LIBS)
  set (G2O_LGPL_LIB_TYPE SHARED)
else()
  message(STATUS "Building LGPL code as static library (affects license of the binary)")
endif()

# Eigen library parallelise itself, though, presumably due to performance issues
# OPENMP is experimental. We experienced some slowdown with it
set(G2O_USE_OPENMP OFF CACHE BOOL "Build g2o with OpenMP support (EXPERIMENTAL)")
if(G2O_USE_OPENMP)
  find_package(OpenMP)
  if(OPENMP_FOUND)
    set (G2O_OPENMP 1)
    set(g2o_C_FLAGS "${g2o_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(g2o_CXX_FLAGS "${g2o_CXX_FLAGS} -DEIGEN_DONT_PARALLELIZE ${OpenMP_CXX_FLAGS}")
    message(STATUS "Compiling with OpenMP support")
  endif(OPENMP_FOUND)
endif(G2O_USE_OPENMP)

# OpenGL is used in the draw actions for the different types, as well
# as for creating the GUI itself
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL)
# If OpenGL was found, use the import target if available. If not, use old-style includes
set(G2O_USE_OPENGL OFF CACHE BOOL "Build g2o with OpenGL support for visualization")
if (OPENGL_FOUND AND G2O_USE_OPENGL)
  if (TARGET OpenGL::GL)
    set(G2O_OPENGL_TARGET "OpenGL::GL;OpenGL::GLU")
  else()
    set(G2O_OPENGL_TARGET "${OPENGL_LIBRARIES}")
    include_directories(${OPENGL_INCLUDE_DIR})
  endif()
  set (G2O_HAVE_OPENGL 0)
endif()

# Compiler specific options for gcc
if(CMAKE_COMPILER_IS_GNUCXX)
  option (BUILD_WITH_MARCH_NATIVE "Build with \"-march native\"" OFF)
  message(STATUS "Compiling with GCC")
  
  if(NOT ${ARCH} MATCHES arm)
    # Generic settings for optimisation
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -msse4.2")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -msse4.2")
  endif()
  # switch off optimization for debug builds
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

  # OS X
  #IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}") 
    #SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}") 
  #ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # Linux
  if(BUILD_WITH_MARCH_NATIVE AND NOT ${ARCH} MATCHES arm AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=native")
  endif()
  # activate warnings !!!
  set(g2o_C_FLAGS "${g2o_C_FLAGS} -Wall -W")
  set(g2o_CXX_FLAGS "${g2o_CXX_FLAGS} -Wall -W")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message (STATUS "Compiling with Clang")
  # activate all warnings
  #SET(g2o_C_FLAGS "${g2o_C_FLAGS} -Weverything")
  #SET(g2o_CXX_FLAGS "${g2o_CXX_FLAGS} -Weverything")
  set(g2o_C_FLAGS "${g2o_C_FLAGS} -Wall")
  set(g2o_CXX_FLAGS "${g2o_CXX_FLAGS} -Wall")
  #SET(g2o_CXX_FLAGS "${g2o_CXX_FLAGS} -Wall -stdlib=libc++")
endif()

# C++11 support
IF(NOT MSVC)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif()
endif()

# specifying compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${g2o_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${g2o_C_FLAGS}")

# # Compiler specific options for gcc
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")
# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native")

# # activate warnings !!!
# set(g2o_C_FLAGS "${g2o_C_FLAGS} -Wall -W")
# set(g2o_CXX_FLAGS "${g2o_CXX_FLAGS} -Wall -W")

# # specifying compiler flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${g2o_CXX_FLAGS}")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${g2o_C_FLAGS}")

# # C++11 support
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Eigen3
set(EIGEN3_INCLUDE_DIR ${G2O_EIGEN3_INCLUDE})
find_package(Eigen3 REQUIRED)
if(EIGEN3_FOUND)
  set(G2O_EIGEN3_INCLUDE ${EIGEN3_INCLUDE_DIR} CACHE PATH "Directory of Eigen3")
else(EIGEN3_FOUND)
  set(G2O_EIGEN3_INCLUDE "" CACHE PATH "Directory of Eigen3")
endif(EIGEN3_FOUND)

# Set up the top-level include directories
include_directories(${g2o_SOURCE_DIR} ${G2O_EIGEN3_INCLUDE})

# Generate config.h
set(G2O_OPENGL_FOUND ${OPENGL_FOUND})
set(G2O_HAVE_CHOLMOD ${CHOLMOD_FOUND})
set(G2O_HAVE_CSPARSE ${CSPARSE_FOUND})
set(G2O_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(G2O_LGPL_SHARED_LIBS ${BUILD_LGPL_SHARED_LIBS})
set(G2O_CXX_COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER}")

# configure_file(config.h.in ${g2o_SOURCE_DIR}/config.h)

configure_file(config.h.in ${PROJECT_BINARY_DIR}/g2o/config.h)
include_directories(${PROJECT_BINARY_DIR})
install(FILES ${PROJECT_BINARY_DIR}/g2o/config.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o)

# Include the subdirectories
add_subdirectory(EXTERNAL)
add_subdirectory(g2o)

install(FILES package.xml DESTINATION share/g2o)
