
aux_source_directory(. TC_TESTS_AUDIO_SOURCES)

include_directories (
   "${TCSystem_BINARY_DIR}/Modules/Audio"
)
add_library (TCTestAudio
    ${TC_TESTS_AUDIO_SOURCES}
)

target_link_libraries(TCTestAudio
    TCAudio
)
add_dependencies(TCTestAudio
    TCAudio
)

set_target_properties(TCTestAudio PROPERTIES FOLDER Tests)
