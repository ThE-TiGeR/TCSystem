
include_directories (
   "../../Modules/Basic/inc"
   "../../Modules/Math"
)

add_executable (TCMathSheet
    MathSheet.cpp
)

target_link_libraries(TCMathSheet
    TCBasic
)

set_target_properties(TCMathSheet PROPERTIES FOLDER Programs)

install(TARGETS TCMathSheet 
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Programs
)
