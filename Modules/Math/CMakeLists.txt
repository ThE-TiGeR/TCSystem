include(FindBoost)
include_directories(${Boost_INCLUDE_DIRS})

set (TC_MATH_PUBLIC_HEADER
        ${TCSystem_SOURCE_DIR}/inc/TCMathApi.h
        ${TCSystem_SOURCE_DIR}/inc/TCMathChecksum.h
        ${TCSystem_SOURCE_DIR}/inc/TCMathCoord2D.h
        ${TCSystem_SOURCE_DIR}/inc/TCMathCoord3D.h
        ${TCSystem_SOURCE_DIR}/inc/TCMathCoord4D.h
        ${TCSystem_SOURCE_DIR}/inc/TCMathCoordN.h
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

set (TC_MATH_TEST_FILES
    Tests/TCMathTestSuite.h
    Tests/TCMathTestSuite.cpp
)

add_library (TCMath
        ${TC_MATH_SOURCE}
        ${TC_MATH_PUBLIC_HEADER}
        ${TC_MATH_TEST_FILES}
)

target_link_libraries(TCMath
    TCBasic
)

set_target_properties(TCMath PROPERTIES COMPILE_DEFINITIONS TCMATH_EXPORTS)
set_target_properties(TCMath PROPERTIES FOLDER Modules)
set_target_properties(TCMath PROPERTIES PUBLIC_HEADER "${TC_MATH_PUBLIC_HEADER}")

install(TARGETS TCMath
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Mathematics
)

