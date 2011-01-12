
set (TC_MATH_PUBLIC_HEADER
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

set (TC_MATH_SOURCE
	TCMathDebugNew.cpp
	TCMathFactory.cpp
	TCMathUtil.cpp
	TCMD5.cpp
	TCMD5.h
)

add_library (TCMath
        ${TC_MATH_SOURCE}
        ${TC_MATH_PUBLIC_HEADER}
)

target_link_libraries(TCMath
    TCBasic
)
add_dependencies(TCMath
    TCBasic
)

set_target_properties(TCMath PROPERTIES COMPILE_DEFINITIONS TCMATH_EXPORTS)
set_target_properties(TCMath PROPERTIES FOLDER Modules)
set_target_properties(TCMath PROPERTIES PUBLIC_HEADER "${TC_MATH_PUBLIC_HEADER}")

install(TARGETS TCMath
   LIBRARY DESTINATION lib
   PUBLIC_HEADER DESTINATION include
   COMPONENT Mathematics
)

