cmake_minimum_required (VERSION 2.6)

project (TCSystem)

#-----------------------------------------------------------------------------
# TCSYSTEM version number, useful for packaging and doxygen doc:
set (TCSystem_VERSION_MAJOR 2)
set (TCSystem_VERSION_MINOR 1)
set (TCSystem_VERSION_PATCH 0)

SET(TCSystem_VERSION
  "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}")
SET(PACKAGE_VERSION
  "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}")
# This setting of SOVERSION assumes that any API change
# will increment either the minor or major version number of TCSystem
SET(TCSystem_LIBRARY_PROPERTIES
  VERSION   "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}"
  SOVERSION "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}"
)

SET(CPACK_PACKAGE_VERSION_MAJOR "${TCSystem_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${TCSystem_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${TCSystem_VERSION_PATCH}")
include (CPack)

set (CMAKE_MODULE_PATH ${TCSystem_SOURCE_DIR}/cmake)
include(TCSystemCompileFlags)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_LIB_INSTALL_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_LIB_INSTALL_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_BIN_INSTALL_DIR})

# global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# package config file
configure_file(TCSystem.pc.in "${PROJECT_BINARY_DIR}/TCSystem.pc" @ONLY)
configure_file(TCSystemNet.pc.in "${PROJECT_BINARY_DIR}/TCSystemNet.pc" @ONLY)
configure_file(TCSystemGui.pc.in "${PROJECT_BINARY_DIR}/TCSystemGui.pc" @ONLY)
configure_file(TCSystemAudio.pc.in "${PROJECT_BINARY_DIR}/TCSystemAudio.pc" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/TCSystem.pc" DESTINATION "lib/pkgconfig" COMPONENT dev)
install(FILES "${PROJECT_BINARY_DIR}/TCSystemNet.pc" DESTINATION "lib/pkgconfig" COMPONENT dev)
install(FILES "${PROJECT_BINARY_DIR}/TCSystemGui.pc" DESTINATION "lib/pkgconfig" COMPONENT dev)
install(FILES "${PROJECT_BINARY_DIR}/TCSystemAudio.pc" DESTINATION "lib/pkgconfig" COMPONENT dev)

enable_testing()

if (NOT TCSYSTEM_BASIC)
  find_package(Boost)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory (Modules)
if (NOT TCSYSTEM_BASIC)
  add_subdirectory (Tests)
  add_subdirectory (Programs)
  add_subdirectory (doc)
endif()

