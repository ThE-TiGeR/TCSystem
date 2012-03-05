find_package(FOX)
find_package(OpenGL)
find_package(GLext)
find_package(Boost)

set (TC_OPENGL_PUBLIC_HEADER
    inc/TCOpenGL.h
    inc/TCOpenGLApi.h
    inc/TCOpenGLColor.h
    inc/TCOpenGLCoordinateCross.h
    inc/TCOpenGLCrossHair.h
    inc/TCOpenGLFace.h
    inc/TCOpenGLFactory.h
    inc/TCOpenGLLineRenderer.h
    inc/TCOpenGLMaterial.h
    inc/TCOpenGLObject.h
    inc/TCOpenGLObject2D.h
    inc/TCOpenGLObject3D.h
    inc/TCOpenGLObjectGroup.h
    inc/TCOpenGLPatchRenderer.h
    inc/TCOpenGLTextRenderer.h
    inc/TCOpenGLTypes.h
    inc/TCOpenGLUtil.h
    inc/TCOpenGLViewer.h
)
add_library (TCOpenGL
    src/TCOpenGLColor.cpp
    src/TCOpenGLCoordinateCross.cpp
    src/TCOpenGLCrossHair.cpp
    src/TCOpenGLDebugNew.cpp
    src/TCOpenGLFace.cpp
    src/TCOpenGLFactory.cpp
    src/TCOpenGLLineRenderer.cpp
    src/TCOpenGLMaterial.cpp
    src/TCOpenGLObject.cpp
    src/TCOpenGLObject2D.cpp
    src/TCOpenGLObject3D.cpp
    src/TCOpenGLObjectGroup.cpp
    src/TCOpenGLPatchRenderer.cpp
    src/TCOpenGLTextRenderer.cpp
    src/TCOpenGLUtil.cpp
    src/TCOpenGLViewer.cpp
    ${TC_OPENGL_PUBLIC_HEADER}
)

include_directories (
   inc
   ../Basic/inc
   ../Math/inc
   ${FOX_INCLUDE_DIRS}
   ${GLEXT_INCLUDE_DIR}
   ${Boost_INCLUDE_DIRS}
   ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(TCOpenGL
    TCMath
    TCBasic
    ${FOX_LIBRARIES}
    ${OPENGL_LIBRARIES}
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
