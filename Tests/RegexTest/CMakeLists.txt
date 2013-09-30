find_package(Boost COMPONENTS regex)

include_directories (
   ../../Modules/Basic/inc
)

add_executable(TCRegexTest
    TCRegexTest
)

target_link_libraries(TCRegexTest
    TCBasic
    ${Boost_regex_LIBS}
)

set_target_properties(TCRegexTest PROPERTIES FOLDER Tests)

install(TARGETS TCRegexTest
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Tests
)
