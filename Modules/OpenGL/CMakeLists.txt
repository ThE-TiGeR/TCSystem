include (FindFOX)
include (FindOpenGL)
include (FindGLext)

add_library (TCOpenGL
	TCOpenGL.h
	TCOpenGLApi.h
	TCOpenGLColor.cpp
	TCOpenGLColor.h
	TCOpenGLCoordinateCross.cpp
	TCOpenGLCoordinateCross.h
	TCOpenGLCrossHair.cpp
	TCOpenGLCrossHair.h
	TCOpenGLDebugNew.cpp
	TCOpenGLFace.cpp
	TCOpenGLFace.h
	TCOpenGLFactory.cpp
	TCOpenGLFactory.h
	TCOpenGLLineRenderer.cpp
	TCOpenGLLineRenderer.h
	TCOpenGLMaterial.cpp
	TCOpenGLMaterial.h
	TCOpenGLObject.cpp
	TCOpenGLObject.h
	TCOpenGLObject2D.cpp
	TCOpenGLObject2D.h
	TCOpenGLObject3D.cpp
	TCOpenGLObject3D.h
	TCOpenGLObjectGroup.cpp
	TCOpenGLObjectGroup.h
	TCOpenGLPatchRenderer.cpp
	TCOpenGLPatchRenderer.h
	TCOpenGLTextRenderer.cpp
	TCOpenGLTextRenderer.h
	TCOpenGLTypes.h
	TCOpenGLUtil.cpp
	TCOpenGLUtil.h
	TCOpenGLViewer.cpp
	TCOpenGLViewer.h
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
set_target_properties(TCOpenGL PROPERTIES LINK_FLAGS  "${FOX_LDFLAGS}")
set_target_properties(TCOpenGL PROPERTIES FOLDER Modules)
