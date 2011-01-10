include (FindFOX)

aux_source_directory(. TC_GUI_SOURCES)

add_library (TCGui
    ${TC_GUI_SOURCES}
)

include_directories (
   "${FOX_INCLUDE_DIR}"
)

target_link_libraries(TCGui
    TCBasic
    ${FOX_LIBRARIES}
)
add_dependencies(TCGui
    TCBasic
)

set_target_properties(TCGui PROPERTIES COMPILE_DEFINITIONS "TCGUI_EXPORTS;${FOX_DEFINES}")
set_target_properties(TCGui PROPERTIES FOLDER Modules)
