include (FindFOX)
include (FindOpenGL)
include (FindGLext)

set (TC_OPENGL_PUBLIC_HEADER
    TCOpenGL.h
    TCOpenGLApi.h
    TCOpenGLColor.h
    TCOpenGLCoordinateCross.h
    TCOpenGLCrossHair.h
    TCOpenGLFace.h
    TCOpenGLFactory.h
    TCOpenGLLineRenderer.h
    TCOpenGLMaterial.h
    TCOpenGLObject.h
    TCOpenGLObject2D.h
    TCOpenGLObject3D.h
    TCOpenGLObjectGroup.h
    TCOpenGLPatchRenderer.h
    TCOpenGLTextRenderer.h
    TCOpenGLTypes.h
    TCOpenGLUtil.h
    TCOpenGLViewer.h
)
add_library (TCOpenGL
    TCOpenGLColor.cpp
    TCOpenGLCoordinateCross.cpp
    TCOpenGLCrossHair.cpp
    TCOpenGLDebugNew.cpp
    TCOpenGLFace.cpp
    TCOpenGLFactory.cpp
    TCOpenGLLineRenderer.cpp
    TCOpenGLMaterial.cpp
    TCOpenGLObject.cpp
    TCOpenGLObject2D.cpp
    TCOpenGLObject3D.cpp
    TCOpenGLObjectGroup.cpp
    TCOpenGLPatchRenderer.cpp
    TCOpenGLTextRenderer.cpp
    TCOpenGLUtil.cpp
    TCOpenGLViewer.cpp
    ${TC_OPENGL_PUBLIC_HEADER}
)

include_directories (
   "${FOX_INCLUDE_DIRS}"
   "${GLEXT_INCLUDE_DIR}"
)
if (UNIX)
    include_directories (
    "${OPENGL_INCLUDE_DIR}"
    )
endif ()

target_link_libraries(TCOpenGL
    TCGui
    TCBasic
    ${FOX_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
add_dependencies(TCOpenGL
    TCGui
    TCBasic
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FOX_CFLAGS}")

set_target_properties(TCOpenGL PROPERTIES COMPILE_DEFINITIONS "TCOGL_EXPORTS")
set_target_properties(TCOpenGL PROPERTIES FOLDER Modules)
set_target_properties(TCOpenGL PROPERTIES PUBLIC_HEADER "${TC_OPENGL_PUBLIC_HEADER}")

install(TARGETS TCOpenGL 
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Gui
)
