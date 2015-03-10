
include_directories (
   ../../Modules/Basic/inc
   ../../Modules/Thread/inc
   ../../Modules/Net/inc
)

add_executable (TCPSK2Hausbuch
    psk2hausbuch.cpp
)

target_link_libraries(TCPSK2Hausbuch
    TCBasic
)

set_target_properties(TCPSK2Hausbuch PROPERTIES FOLDER Programs)

install(TARGETS TCPSK2Hausbuch 
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Programs
)
