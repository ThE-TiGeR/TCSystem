# - Try to find MPG123 Toolkit
# Once done this will define
#
#  MPG123_FOUND - system has MPG123
#  MPG123_INCLUDE_DIR - the MPG123 include directory
#  MPG123_DEFINES - the MPG123 defines
#  MPG123_LIBRARIES - Link these to use MPG123

#=============================================================================
# Copyright 2006-2010 Thomas Goessler the
#=============================================================================

FIND_PATH(MPG123_INCLUDE_DIR mpg123.h )

FIND_LIBRARY(MPG123_LIBRARIES NAMES libmpg123 mpg123 )

# handle the QUIETLY and REQUIRED arguments and set MPG123_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MPG123 DEFAULT_MSG MPG123_LIBRARIES MPG123_INCLUDE_DIR)

MARK_AS_ADVANCED(MPG123_INCLUDE_DIR MPG123_LIBRARIES)


