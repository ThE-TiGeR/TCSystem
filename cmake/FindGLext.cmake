# - Try to find GLEXT
# Once done this will define
#
#  GLEXT_FOUND - system has GLEXT
#  GLEXT_INCLUDE_DIR - the GLEXT include directory

#=============================================================================
# Copyright 2006-2010 Thomas Goessler
#=============================================================================

FIND_PATH(GLEXT_INCLUDE_DIR GL/glext.h)

# handle the QUIETLY and REQUIRED arguments and set GLEXT_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(glext DEFAULT_MSG GLEXT_INCLUDE_DIR)

MARK_AS_ADVANCED(GLEXT_INCLUDE_DIR)


