
# Finds OGG library
#
#  OGG_INCLUDE_DIR - where to find ogg.h, etc.
#  OGG_LIBRARIES   - List of libraries when using OGG.
#  OGG_FOUND       - True if OGG found.
#

if (OGG_INCLUDE_DIR)
  # Already in cache, be silent
  set(OGG_FIND_QUIETLY TRUE)
endif (OGG_INCLUDE_DIR)

find_path(OGG_INCLUDE_DIR ogg/ogg.h ogg.h PATH_SUFFIXES include include/ogg
  /opt/local/include
  /usr/local/include
  /usr/include
)

#message("OGG_INCLUDE_DIR = ${OGG_INCLUDE_DIR}")

set(OGG_NAMES ogg libogg)
find_library(OGG_LIBRARY
  NAMES ${OGG_NAMES}
  PATHS /usr/lib /usr/local/lib /opt/local/lib
)

#message("OGG_LIBRARY = ${OGG_LIBRARY}")

if (OGG_INCLUDE_DIR AND OGG_LIBRARY)
   set(OGG_FOUND TRUE)
   set( OGG_LIBRARIES ${OGG_LIBRARY} )
else (OGG_INCLUDE_DIR AND OGG_LIBRARY)
   set(OGG_FOUND FALSE)
   set(OGG_LIBRARIES)
endif (OGG_INCLUDE_DIR AND OGG_LIBRARY)

if (OGG_FOUND)
   if (NOT OGG_FIND_QUIETLY)
      message(STATUS "Found OGG: ${OGG_LIBRARY}")
   endif (NOT OGG_FIND_QUIETLY)
else (OGG_FOUND)
   if (OGG_FIND_REQUIRED)
      message(STATUS "Looked for OGG libraries named ${OGG_NAMES}.")
      message(STATUS "Include file detected: [${OGG_INCLUDE_DIR}].")
      message(STATUS "Lib file detected: [${OGG_LIBRARY}].")
      message(FATAL_ERROR "=========> Could NOT find OGG library")
   endif (OGG_FIND_REQUIRED)
endif (OGG_FOUND)

mark_as_advanced(
  OGG_LIBRARY
  OGG_INCLUDE_DIR
  )
