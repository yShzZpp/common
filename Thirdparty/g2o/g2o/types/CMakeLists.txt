# Pre-canned types
add_subdirectory(data)
add_subdirectory(slam2d)
add_subdirectory(slam3d)
add_subdirectory(sba)
add_subdirectory(sim3)
add_subdirectory(icp)
add_subdirectory(sclam2d)

add_subdirectory(slam2d_addons)
add_subdirectory(slam3d_addons)
OPTION(G2O_BUILD_DEPRECATED_TYPES "Compile the deprecated types" OFF)
IF(G2O_BUILD_DEPRECATED_TYPES)
  ADD_SUBDIRECTORY(deprecated)
ENDIF()