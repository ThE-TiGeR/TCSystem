find_package(FOX)

include_directories (
   inc
   ../Basic/inc
   "${FOX_INCLUDE_DIRS}"
)

set (TC_GUI_PUBLIC_HEADER
	inc/TCGuiActionBox.h
	inc/TCGuiApi.h
	inc/TCGuiApplication.h
	inc/TCGuiBase.h
	inc/TCGuiButton.h
	inc/TCGuiDialogBox.h
	inc/TCGuiImageHandler.h
	inc/TCGuiLabel.h
	inc/TCGuiMenu.h
	inc/TCGuiMessageBox.h
	inc/TCGuiShutterMenu.h
	inc/TCGuiTreeList.h
	inc/TCGuiWidget.h
	inc/TCGuiWindow.h
)

add_library (TCGui
	src/TCGuiActionBox.cpp
	src/TCGuiApplication.cpp
	src/TCGuiBase.cpp
	src/TCGuiButton.cpp
	src/TCGuiDebugNew.cpp
	src/TCGuiDialogBox.cpp
	src/TCGuiImageHandler.cpp
	src/TCGuiImages.cpp
	src/TCGuiLabel.cpp
	src/TCGuiMenu.cpp
	src/TCGuiMessageBox.cpp
	src/TCGuiMLS.cpp
	src/TCGuiShutterMenu.cpp
	src/TCGuiTreeList.cpp
	src/TCGuiWidget.cpp
	src/TCGuiWindow.cpp
    ${TC_GUI_PUBLIC_HEADER}
)

target_link_libraries(TCGui
	TCBasic
	${FOX_LIBRARIES}
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

