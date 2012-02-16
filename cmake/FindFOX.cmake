# - Try to find FOX Toolkit
# Once done this will define
#
#  FOX_FOUND - system has FOX
#  FOX_INCLUDE_DIRS - the FOX include directory
#  FOX_CFLAGS - the FOX used c flags
#  FOX_LIBRARIES - Link these to use FOX

#=============================================================================
# Copyright 2006-2012 Thomas Goessler
#=============================================================================

if (UNIX)
   include (FindPkgConfig)
   pkg_search_module(FOX REQUIERED fox)
else ()
   FIND_PATH(FOX_INCLUDE_DIRS fx.h )

   FIND_LIBRARY(FOX_LIB_RELEASE NAMES FOXDLL-1.6)
   if (FOX_LIB_RELEASE)
     set(FOX_LIBRARIES
	   optimized 
	   ${FOX_LIB_RELEASE}
	 )
   endif()

   FIND_LIBRARY(FOX_LIB_DEBUG NAMES FOXDLLD-1.6)
   if (FOX_LIB_DEBUG)
     set(FOX_LIBRARIES
	   ${FOX_LIBRARIES}
	   debug 
	   ${FOX_LIB_DEBUG}
	 )
   endif()
   
   # handle the QUIETLY and REQUIRED arguments and set FOX_FOUND to TRUE if 
   # all listed variables are TRUE
   INCLUDE(FindPackageHandleStandardArgs)
   FIND_PACKAGE_HANDLE_STANDARD_ARGS(FOX DEFAULT_MSG FOX_LIBRARIES FOX_INCLUDE_DIRS)

   MARK_AS_ADVANCED(FOX_INCLUDE_DIRS FOX_LIBRARIES)
   SET(FOX_CFLAGS /DFOXDLL)
endif ()

