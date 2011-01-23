include (FindFOX)

set (TC_GUI_PUBLIC_HEADER
        TCGuiActionBox.h
        TCGuiApi.h
        TCGuiApplication.h
        TCGuiBase.h
        TCGuiButton.h
        TCGuiDialogBox.h
        TCGuiImageHandler.h
        TCGuiLabel.h
        TCGuiMenu.h
        TCGuiMessageBox.h
        TCGuiShutterMenu.h
        TCGuiTreeList.h
        TCGuiWidget.h
        TCGuiWindow.h
)

add_library (TCGui
	TCGuiActionBox.cpp
	TCGuiApplication.cpp
	TCGuiBase.cpp
	TCGuiButton.cpp
	TCGuiDebugNew.cpp
	TCGuiDialogBox.cpp
	TCGuiImageHandler.cpp
	TCGuiImages.cpp
	TCGuiLabel.cpp
	TCGuiMenu.cpp
	TCGuiMessageBox.cpp
	TCGuiMLS.cpp
	TCGuiShutterMenu.cpp
	TCGuiTreeList.cpp
	TCGuiWidget.cpp
	TCGuiWindow.cpp
    ${TC_GUI_PUBLIC_HEADER}
)

include_directories (
   "${FOX_INCLUDE_DIRS}"
)

target_link_libraries(TCGui
    TCBasic
    ${FOX_LIBRARIES}
)
add_dependencies(TCGui
    TCBasic
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FOX_CFLAGS}")

set_target_properties(TCGui PROPERTIES COMPILE_DEFINITIONS "TCGUI_EXPORTS")
set_target_properties(TCGui PROPERTIES FOLDER Modules)
set_target_properties(TCGui PROPERTIES PUBLIC_HEADER "${TC_GUI_PUBLIC_HEADER}")

install(TARGETS TCGui
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Gui
)

