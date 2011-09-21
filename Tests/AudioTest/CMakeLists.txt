
include_directories (
   "../../Modules/Basic/inc"
   "../../Modules/Thread/inc"
   "../../Modules/Audio"
)

add_executable (TCAudioTest
    audio_test.cpp
)

target_link_libraries(TCAudioTest
    TCAudio
)

set_target_properties(TCAudioTest PROPERTIES FOLDER Tests)

install(TARGETS TCAudioTest 
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Tests
)
