# - Try to find ZIP executable
#
#=============================================================================
# Copyright 2014 Thomas Goessler
#=============================================================================

find_program(ZIP_EXECUTABLE wzzip PATHS "$ENV{ProgramFiles}/WinZip")
if(ZIP_EXECUTABLE)
  set(ZIP_COMMAND "${ZIP_EXECUTABLE} -P <ARCHIVE> <FILELIST>")
  set(ZIP_NEED_QUOTES TRUE)
endif()

if(NOT ZIP_EXECUTABLE)
  find_program(ZIP_EXECUTABLE 7z PATHS "$ENV{ProgramFiles}/7-Zip" "$ENV{ProgramW6432}/7-Zip")
  if(ZIP_EXECUTABLE)
    set(ZIP_COMMAND "${ZIP_EXECUTABLE} a -tzip <ARCHIVE> <FILELIST>")
  set(ZIP_NEED_QUOTES TRUE)
  endif()
endif()

if(NOT ZIP_EXECUTABLE)
  find_package(Cygwin)
  find_program(ZIP_EXECUTABLE zip PATHS "${CYGWIN_INSTALL_PATH}/bin")
  if(ZIP_EXECUTABLE)
    set(ZIP_COMMAND "${ZIP_EXECUTABLE} -r <ARCHIVE> . -i<FILELIST>")
    set(ZIP_NEED_QUOTES FALSE)
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZIP DEFAULT_MSG ZIP_EXECUTABLE ZIP_COMMAND)
mark_as_advanced(ZIP_EXECUTABLE ZIP_COMMAND)

function(create_zip ARCHIVE FILELIST)

	file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/${ARCHIVE} ARCHIVE)
	file(TO_NATIVE_PATH "${FILELIST}" FILELIST)
    add_custom_command(
			OUTPUT ${ARCHIVE}
			COMMAND ${ZIP_EXECUTABLE} a -tzip ${ARCHIVE} "${FILELIST}"
			DEPENDS ${FILELIST}
			COMMENT Creating ${ARCHIVE} with  ${FILELIST})

    add_custom_target(CREATE_ZIP DEPENDS ${ARCHIVE})

endfunction()