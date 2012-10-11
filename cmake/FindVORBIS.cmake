
# Finds VORBIS library
#
#  VORBIS_INCLUDE_DIR - where to find vorbis.h, etc.
#  VORBIS_LIBRARIES   - List of libraries when using VORBIS.
#  VORBIS_FOUND       - True if VORBIS found.
#

if (VORBIS_INCLUDE_DIR)
  # Already in cache, be silent
  set(VORBIS_FIND_QUIETLY TRUE)
endif (VORBIS_INCLUDE_DIR)

find_path(VORBIS_INCLUDE_DIR vorbis/vorbisenc.h vorbisenc.h PATH_SUFFIXES include include/vorbis
  /opt/local/include
  /usr/local/include
  /usr/include
)

set(VORBIS_NAMES vorbis libvorbis)
find_library(VORBIS_LIBRARY
  NAMES ${VORBIS_NAMES}
  PATHS /usr/lib /usr/local/lib /opt/local/lib
)

if (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY)
   set(VORBIS_FOUND TRUE)
   set( VORBIS_LIBRARIES ${VORBIS_LIBRARY} )
else (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY)
   set(VORBIS_FOUND FALSE)
   set(VORBIS_LIBRARIES)
endif (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY)

if (VORBIS_FOUND)
   if (NOT VORBIS_FIND_QUIETLY)
      message(STATUS "Found VORBIS: ${VORBIS_LIBRARY}")
   endif (NOT VORBIS_FIND_QUIETLY)
else (VORBIS_FOUND)
   if (VORBIS_FIND_REQUIRED)
      message(STATUS "Looked for VORBIS libraries named ${VORBIS_NAMES}.")
      message(STATUS "Include file detected: [${VORBIS_INCLUDE_DIR}].")
      message(STATUS "Lib file detected: [${VORBIS_LIBRARY}].")
      message(FATAL_ERROR "=========> Could NOT find VORBIS library")
   endif (VORBIS_FIND_REQUIRED)
endif (VORBIS_FOUND)

mark_as_advanced(
  VORBIS_LIBRARY
  VORBIS_INCLUDE_DIR
  )
