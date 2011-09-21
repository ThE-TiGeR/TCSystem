
include_directories (
   "../../Modules/Basic/inc"
   "../../Modules/Math/inc"
   "../../Modules/Thread/inc"
   "../../Modules/Net/inc"
)

add_executable (TCUnitTests
    TCUnitTest.cpp
)

include_directories (
   "${TCSystem_SOURCE_DIR}/Modules/Basic/Tests"
   "${TCSystem_SOURCE_DIR}/Modules/Thread/Tests"
   "${TCSystem_SOURCE_DIR}/Modules/Net/Tests"
   "${TCSystem_SOURCE_DIR}/Modules/Math/Tests"
)

target_link_libraries(TCUnitTests
    TCMath
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

add_test(NAME TCUnitTests COMMAND TCUnitTests)
