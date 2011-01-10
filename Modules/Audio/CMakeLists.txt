include(FindOpenAL)
include(FindMPG123)

include_directories (
   "${OPENAL_INCLUDE_DIR}"
   "${MPG123_INCLUDE_DIR}"
)

aux_source_directory(. TC_AUDIO_SOURCES)

add_library (TCAudio
	${TC_AUDIO_SOURCES}
)

target_link_libraries (TCAudio
    TCThread
	TCBasic
	${OPENAL_LIBRARY}
	${MPG123_LIBRARIES}
)

add_dependencies (TCAudio
    TCThread
	TCBasic
)

set_target_properties(TCAudio PROPERTIES FOLDER Modules)
set_target_properties(TCAudio PROPERTIES COMPILE_DEFINITIONS "TCAUDIO_EXPORTS")
