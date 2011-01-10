cmake_minimum_required (VERSION 2.6)

project (TCSystem)

# The version number.
set (TCSystem_VERSION_MAJOR 1)
set (TCSystem_VERSION_MINOR 6)

set (LIBRARY_OUTPUT_PATH ${TCSystem_BINARY_DIR}/bin)
set (EXECUTABLE_OUTPUT_PATH ${TCSystem_BINARY_DIR}/bin)
set (CMAKE_MODULE_PATH ${TCSystem_BINARY_DIR}/cmake)

# global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# add the binary tree to the search path for include files
# so that we will find TCSystemConfig.h
# also add the Basic directory for all sub projects
include_directories (
   "${PROJECT_BINARY_DIR}/inc"
)

# set our compiler flags
if (UNIX)
   set (TC_COMPILE_FLAGS "")
   set (TC_COMPILE_FLAGS_DEBUG "-g")
   set (TC_COMPILE_FLAGS_RELEASE "-O3")

   set (TC_EXE_LINK_FLAGS         "")
   set (TC_EXE_LINK_FLAGS_DEBUG   "")
   set (TC_EXE_LINK_FLAGS_RELEASE "")
endif()

if (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
   set (TC_COMPILE_FLAGS         "/EHsc /W4 /D_SCL_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS")
   set (TC_COMPILE_FLAGS_DEBUG   "")
   set (TC_COMPILE_FLAGS_RELEASE "/Ox /Ob2 /Ot /Oi /GL /GS- /fp:fast /D_SECURE_SCL=0")
   
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

add_subdirectory (Modules)
add_subdirectory (Tests)
