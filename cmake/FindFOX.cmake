# - Try to find FOX Toolkit
# Once done this will define
#
#  FOX_FOUND - system has FOX
#  FOX_INCLUDE_DIR - the FOX include directory
#  FOX_DEFINES - the FOX defines
#  FOX_LIBRARIES - Link these to use FOX

#=============================================================================
# Copyright 2006-2010 Thomas Goessler the
#=============================================================================

FIND_PATH(FOX_INCLUDE_DIR fx.h )

FIND_LIBRARY(FOX_LIBRARIES NAMES FOXDLL-1.6 )

# handle the QUIETLY and REQUIRED arguments and set FOX_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FOX DEFAULT_MSG FOX_LIBRARIES FOX_INCLUDE_DIR)

MARK_AS_ADVANCED(FOX_INCLUDE_DIR FOX_LIBRARIES)
SET(FOX_DEFINES FOXDLL)

