
include_directories (
   "${TCSystem_BINARY_DIR}/Modules/Audio"
)
add_executable (TCAudioTest
    audio_test.cpp
)

target_link_libraries(TCAudioTest
    TCAudio
)
add_dependencies(TCAudioTest
    TCAudio
)

set_target_properties(TCAudioTest PROPERTIES FOLDER Tests)
