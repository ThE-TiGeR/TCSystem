include (FindFOX)
include (FindOpenGL)

aux_source_directory(. TC_OPENGL_SOURCES)

add_library (TCOpenGL
    ${TC_OPENGL_SOURCES}
)

include_directories (
   "${FOX_INCLUDE_DIR}"
)
if (!WIN32)
	include_directories (
	   "${OPENGL_INCLUDE_DIR}"
	)
endif ()

target_link_libraries(TCOpenGL
    TCGui
    TCBasic
    ${FOX_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
add_dependencies(TCOpenGL
    TCGui
    TCBasic
)

set_target_properties(TCOpenGL PROPERTIES COMPILE_DEFINITIONS "TCOGL_EXPORTS;${FOX_DEFINES}")
set_target_properties(TCOpenGL PROPERTIES FOLDER Modules)
