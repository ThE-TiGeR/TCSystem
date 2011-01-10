
aux_source_directory(. TC_GUI_SOURCES)

add_library (TCGui
	${TC_GUI_SOURCES}
)

target_link_libraries(TCGui
    TCBasic
)
add_dependencies(TCGui
    TCBasic
)

set_target_properties(TCGui PROPERTIES COMPILE_DEFINITIONS TCGUI_EXPORTS)
set_target_properties(TCGui PROPERTIES FOLDER Modules)
