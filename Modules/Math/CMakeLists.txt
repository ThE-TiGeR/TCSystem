
aux_source_directory(. TC_MATH_SOURCES)

add_library (TCMath
	${TC_MATH_SOURCES}
	${TCSystem_SOURCE_DIR}/inc/TCMathApi.h
	${TCSystem_SOURCE_DIR}/inc/TCMathChecksum.h
	${TCSystem_SOURCE_DIR}/inc/TCMathCoord2D.h
	${TCSystem_SOURCE_DIR}/inc/TCMathCoord3D.h
	${TCSystem_SOURCE_DIR}/inc/TCMathCoord4D.h
	${TCSystem_SOURCE_DIR}/inc/TCMathCoordUtil.h
	${TCSystem_SOURCE_DIR}/inc/TCMathBox.h
	${TCSystem_SOURCE_DIR}/inc/TCMathFactory.h
	${TCSystem_SOURCE_DIR}/inc/TCMathUtil.h	
)

target_link_libraries(TCMath
    TCBasic
)
add_dependencies(TCMath
    TCBasic
)

set_target_properties(TCMath PROPERTIES COMPILE_DEFINITIONS TCMATH_EXPORTS)
set_target_properties(TCMath PROPERTIES FOLDER Modules)
