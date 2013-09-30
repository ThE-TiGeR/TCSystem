
include_directories(
    inc
    ../Basic/inc
)

set (TC_MATH_PUBLIC_HEADER
    inc/TCMathApi.h
    inc/TCMathChecksum.h
    inc/TCMathCoordN.h
    inc/TCMathMatrixN.h
    inc/TCMathMatrix4x4.h
    inc/TCMathCoordUtil.h
    inc/TCMathBox.h
    inc/TCMathFactory.h
    inc/TCMathUtil.h 
)

set (TC_MATH_SOURCE
	src/TCMathDebugNew.cpp
	src/TCMathFactory.cpp
	src/TCMathUtil.cpp
	src/TCMD5.cpp
	src/TCMD5.h
)

set (TC_MATH_TEST_FILES
    tests/TCMathTestSuite.h
    tests/TCMathTestSuite.cpp
    tests/TCMathCoordNTestSuite.h
    tests/TCMathCoordNTestSuite.cpp
    tests/TCMathMatrixNTestSuite.h
    tests/TCMathMatrixNTestSuite.cpp
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

