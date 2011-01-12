include (FindFOX)

add_library (TCGui
	TCGuiActionBox.cpp
	TCGuiActionBox.h
	TCGuiApi.h
	TCGuiApplication.cpp
	TCGuiApplication.h
	TCGuiBase.cpp
	TCGuiBase.h
	TCGuiButton.cpp
	TCGuiButton.h
	TCGuiDebugNew.cpp
	TCGuiDialogBox.cpp
	TCGuiDialogBox.h
	TCGuiImageHandler.cpp
	TCGuiImageHandler.h
	TCGuiImages.cpp
	TCGuiLabel.cpp
	TCGuiLabel.h
	TCGuiMenu.cpp
	TCGuiMenu.h
	TCGuiMessageBox.cpp
	TCGuiMessageBox.h
	TCGuiMLS.cpp
	TCGuiShutterMenu.cpp
	TCGuiShutterMenu.h
	TCGuiTreeList.cpp
	TCGuiTreeList.h
	TCGuiWidget.cpp
	TCGuiWidget.h
	TCGuiWindow.cpp
	TCGuiWindow.h
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
set_target_properties(TCGui PROPERTIES LINK_FLAGS  "${FOX_LDFLAGS}")
set_target_properties(TCGui PROPERTIES FOLDER Modules)
