
add_executable (TCUnitTests
    TCUnitTest.cpp
)

include_directories (
   "${TCSystem_SOURCE_DIR}/Modules/Basic/Tests"
   "${TCSystem_SOURCE_DIR}/Modules/Thread/Tests"
   "${TCSystem_SOURCE_DIR}/Modules/Net/Tests"
)

target_link_libraries(TCUnitTests
    TCNet
	TCThread
    TCBasic
)
add_dependencies(TCUnitTests
    TCNet
	TCThread
    TCBasic
)

set_target_properties(TCUnitTests PROPERTIES FOLDER Tests)

install(TARGETS TCUnitTests 
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Tests
)
