cmake_minimum_required (VERSION 2.6)

project (TCSystem)

#-----------------------------------------------------------------------------
# TCSYSTEM version number, useful for packaging and doxygen doc:
set (TCSystem_VERSION_MAJOR 1)
set (TCSystem_VERSION_MINOR 6)
set (TCSystem_VERSION_PATCH 0)

SET(TCSystem_VERSION
  "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}")
SET(PACKAGE_VERSION
  "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}")
# This setting of SOVERSION assumes that any API change
# will increment either the minor or major version number of openjpeg
SET(TCSystem_LIBRARY_PROPERTIES
  VERSION   "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}.${TCSystem_VERSION_PATCH}"
  SOVERSION "${TCSystem_VERSION_MAJOR}.${TCSystem_VERSION_MINOR}"
)

if (CMAKE_CL_64)
    set (TC_SYSTEM_LIB_INSTALL_DIR lib/x64)
    set (TC_SYSTEM_BIN_INSTALL_DIR bin/x64)
else ()
    set (TC_SYSTEM_LIB_INSTALL_DIR lib)
    set (TC_SYSTEM_BIN_INSTALL_DIR bin)
endif ()

SET(CPACK_PACKAGE_VERSION_MAJOR "${TCSystem_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${TCSystem_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${TCSystem_VERSION_PATCH}")
include (CPack)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_LIB_INSTALL_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_LIB_INSTALL_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${TC_SYSTEM_BIN_INSTALL_DIR})
set (CMAKE_MODULE_PATH ${TCSystem_SOURCE_DIR}/cmake)

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

# add the binary tree to the search path for include files
# so that we will find TCSystemConfig.h
# also add the Basic directory for all sub projects
include_directories (
   "${PROJECT_BINARY_DIR}/inc"
)

# set our compiler flags
if (UNIX)
   set (TC_COMPILE_FLAGS "-Wall -Wno-multichar -ggdb -DHAVE_UNIT_TESTS")
   set (TC_COMPILE_FLAGS_DEBUG "-ggdb -DDEBUG")
   set (TC_COMPILE_FLAGS_RELEASE "-O3 -DNDEBUG")

   if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
      set (TC_COMPILE_FLAGS "${TC_COMPILE_FLAGS} -DTCOS_LINUX")
   endif ()

   set (TC_EXE_LINK_FLAGS         "")
   set (TC_EXE_LINK_FLAGS_DEBUG   "")
   set (TC_EXE_LINK_FLAGS_RELEASE "")
endif()

if (WIN32)
   set (TC_COMPILE_FLAGS         "/DHAVE_UNIT_TESTS /EHsc /W4 /D_SCL_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS")
   set (TC_COMPILE_FLAGS_DEBUG   "/DDEBUG")
   set (TC_COMPILE_FLAGS_RELEASE "/DNDEBUG /Ox /Ob2 /Ot /Oi /GL /GS- /fp:fast /D_SECURE_SCL=0")
   
   set (TC_EXE_LINK_FLAGS         "")
   set (TC_EXE_LINK_FLAGS_DEBUG   "")
   set (TC_EXE_LINK_FLAGS_RELEASE "/LTCG")
endif()

set (CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         ${TC_COMPILE_FLAGS}")
set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   ${TC_COMPILE_FLAGS_DEBUG}")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${TC_COMPILE_FLAGS_RELEASE}")

set (CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}           ${TC_COMPILE_FLAGS}")
set (CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     ${TC_COMPILE_FLAGS_DEBUG}")
set (CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   ${TC_COMPILE_FLAGS_RELEASE}")

set (CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS}         ${TC_EXE_LINK_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS_DEBUG   "${CMAKE_EXE_LINKER_FLAGS_DEBUG}   ${TC_EXE_LINK_FLAGS_DEBUG}")
set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${TC_EXE_LINK_FLAGS_RELEASE}")

enable_testing()

add_subdirectory (Modules)
add_subdirectory (Tests)
add_subdirectory (Programs)
add_subdirectory (doc)

