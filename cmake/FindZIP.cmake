# - Try to find ZIP executable
#
#=============================================================================
# Copyright 2014 Thomas Goessler
#=============================================================================

if(NOT ZIP_EXECUTABLE)
  find_program(ZIP_EXECUTABLE 7z PATHS "$ENV{ProgramFiles}/7-Zip" "$ENV{ProgramW6432}/7-Zip")
  if(ZIP_EXECUTABLE)
    set(ZIP_COMMAND "${ZIP_EXECUTABLE} a -tzip <ARCHIVE> <FILELIST>")
  set(ZIP_NEED_QUOTES TRUE)
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZIP DEFAULT_MSG ZIP_EXECUTABLE ZIP_COMMAND)
mark_as_advanced(ZIP_EXECUTABLE ZIP_COMMAND)

function(create_zip ARCHIVE FILELIST)

    add_custom_target(CREATE_ZIP DEPENDS ${FILELIST})

	file(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR}/${ARCHIVE} ARCHIVE)
    foreach(F ${FILELIST}) 
        message(STATUS "create_zip ${F}")
        file(TO_NATIVE_PATH "${F}" F)
        add_custom_command(TARGET CREATE_ZIP
			COMMAND ${ZIP_EXECUTABLE} a -tzip ${ARCHIVE} ${F} -bd
			DEPENDS ${F}
			COMMENT Adding ${F} to ${ARCHIVE})
    endforeach()
	
endfunction()