cmake_minimum_required (VERSION 2.6)

project (TCSystem)

#-----------------------------------------------------------------------------
# TCSYSTEM version number, useful for packaging and doxygen doc:
set (TCSystem_VERSION_MAJOR 2)
set (TCSystem_VERSION_MINOR 1)
set (TCSystem_VERSION_PATCH 0)

set(TCSystem_VERSION
  "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}")
set(PACKAGE_VERSION
  "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}")
# This setting of SOVERSION assumes that any API change
# will increment either the minor or major version number of TCSystem
set(TCSystem_LIBRARY_PROPERTIES
  VERSION   "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}"
  SOVERSION "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}"
)

option(TCSYSTEM_ALL "Enable/Disable building all libs" TRUE)
option(TCSYSTEM_DOC "Enable/Disable building of doxygen" TRUE)
option(TCSYSTEM_TESTS "Enable/Disable building tests" TRUE)
option(TCSYSTEM_PROGS "Enable/Disable building programs" TRUE)
option(TCSYSTEM_NO_BOOST "Enable/Disable usage of boost" FALSE)
option(TCSYSTEM_NO_BZIB2 "Enable/Disable usage of boost" FALSE)
option(TCSYSTEM_NO_ZLIB "Enable/Disable usage of boost" FALSE)

option(TCSYSTEM_BASIC "Enable/Disable building basic lib" FALSE)
option(TCSYSTEM_MATH "Enable/Disable building basic lib" FALSE)
option(TCSYSTEM_NET "Enable/Disable building basic lib" FALSE)
option(TCSYSTEM_AUDIO "Enable/Disable building basic lib" FALSE)
option(TCSYSTEM_THREAD "Enable/Disable building basic lib" FALSE)



set(CPACK_PACKAGE_VERSION_MAJOR "${TCSystem_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${TCSystem_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${TCSystem_VERSION_PATCH}")
include (CPack)

set (CMAKE_MODULE_PATH ${TCSystem_SOURCE_DIR}/cmake)
include(TCSystemCompileFlags)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_LIB_INSTALL_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_LIB_INSTALL_DIR})
if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_BIN_INSTALL_DIR})
endif()

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

if (NOT TCSYSTEM_NO_BOOST)
  find_package(Boost)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(Modules)
if (TCSYSTEM_TESTS)
  add_subdirectory(Tests)
endif()
if (TCSYSTEM_PROGS)
  add_subdirectory(Programs)
endif()
if (TCSYSTEM_DOC)
  add_subdirectory(doc)
endif()

