include(FindBZip2)
include(FindZLIB)

include_directories (
   inc
   "${BZIP2_INCLUDE_DIR}"
   "${ZLIB_INCLUDE_DIRS}"
)

set (TC_BASIC_PUBLIC_HEADER
	inc/TCApplication.h
	inc/TCAssert.h
	inc/TCCodec.h
	inc/TCException.h
	inc/TCOutput.h
	inc/TCDebugNew.h
	inc/TCDefines.h
	inc/TCDelete.h
	inc/TCFactory.h
	inc/TCFile.h
	inc/TCFileName.h
	inc/TCHashTable.h
	inc/TCHeap.h
	inc/TCInterlocked.h
	inc/TCMLSHandler.h
	inc/TCMemoryMappedFile.h
	inc/TCNew.h
	inc/TCNewDisable.h
	inc/TCNewEnable.h
	inc/TCNonCopyable.h
	inc/TCRng.h
	inc/TCScopedPtr.h
	inc/TCScopedArray.h
	inc/TCSharedPtr.h
	inc/TCSharedPtrImpl.h
	inc/TCStream.h
	inc/TCStreamAble.h
	inc/TCString.h
	inc/TCSystem.h
	inc/TCStlAlgorithm.h
	inc/TCStlTypes.h
	inc/TCTime.h
	inc/TCTypes.h
	inc/TCUtil.h
	inc/TCValue.h
	inc/TCWeakPtr.h
	inc/TCWFileName.h
	inc/TCWString.h
	inc/TCUnitApi.h
	inc/TCUnitCleanLiness.h
	inc/TCUnitFailure.h
	inc/TCUnitSimpleTestResult.h
	inc/TCUnitTestCase.h
	inc/TCUnitTest.h
	inc/TCUnitTestResult.h
	inc/TCUnitTestSuite.h
	inc/TCUnitTreeTestResult.h
	inc/TCUnitTreeTestRunner.h
)

add_library (TCBasic
	src/TCApplication.cpp
	src/TCAsciiCodec.cpp
	src/TCAsciiCodec.h
	src/TCBinaryCodec.cpp
	src/TCBinaryCodec.h
	src/TCBz2FileStream.cpp
	src/TCBz2FileStream.h
	src/TCClock.cpp
	src/TCClock.h
	src/TCDebugHeap.cpp
	src/TCDebugHeap.h
	src/TCDebugNew.cpp
	src/TCException.cpp
	src/TCFactory.cpp
	src/TCFile.cpp
	src/TCFileName.cpp
	src/TCFileNameImp.h
	src/TCFilePosix.cpp
	src/TCFileStream.cpp
	src/TCFileStream.h
	src/TCFileWin32.cpp
	src/TCGzFileStream.cpp
	src/TCGzFileStream.h
	src/TCHeapCrtlib.cpp
	src/TCHeapCrtlib.h
	src/TCHeapMutex.h
	src/TCInterlocked.cpp
	src/TCMemoryMappedFilePosix.cpp
	src/TCMemoryMappedFilePosix.h
	src/TCMemoryMappedFileWin32.cpp
	src/TCMemoryMappedFileWin32.h
	src/TCMemoryStream.cpp
	src/TCMemoryStream.h
	src/TCMLSHandler.cpp
	src/TCNew.cpp
	src/TCOutput.cpp
	src/TCRng69069.cpp
	src/TCRng69069.h
	src/TCSettings.cpp
	src/TCSettings.h
	src/TCStreamAble.cpp
	src/TCStreamBase.cpp
	src/TCStreamBase.h
	src/TCString.cpp
	src/TCStringImp.h
	src/TCStringStream.cpp
	src/TCStringStream.h
	src/TCStringTraits.h
	src/TCSystem.cpp
	src/TCTime.cpp
	src/TCUnitSimpleTestResult.cpp
	src/TCUnitTestCase.cpp
	src/TCUnitTestSuite.cpp
	src/TCUnitTreeTestResult.cpp
	src/TCUnitTreeTestRunner.cpp
	src/TCUtil.cpp
	src/TCWFileName.cpp
	src/TCWstring.cpp
	src/TCWStringTraits.h
	tests/TCWStringTest.h
	tests/TCBasicTestSuite.cpp
	tests/TCBasicTestSuite.h
	tests/TCFileNameTest.cpp
	tests/TCFileNameTest.h
	tests/TCHashTableTest.cpp
	tests/TCHashTableTest.h
	tests/TCStreamTest.cpp
	tests/TCStreamTest.h
	tests/TCStringTest.cpp
	tests/TCStringTest.h
	tests/TCTimeTest.cpp
	tests/TCTimeTest.h
	tests/TCUtilTest.cpp
	tests/TCUtilTest.h
	tests/TCWFileNameTest.cpp
	tests/TCWFileNameTest.h
	tests/TCWStringTest.cpp	
	${TC_BASIC_PUBLIC_HEADER}

	../../cmake/FindFOX.cmake
	../../cmake/FindGLext.cmake
	../../cmake/FindMPG123.cmake
	../../cmake/FindOpenAL.cmake
)

if (MSVC)
   target_link_libraries (TCBasic iphlpapi.lib)
elseif (UNIX)
   target_link_libraries (TCBasic -lrt)
endif ()

target_link_libraries (TCBasic ${BZIP2_LIBRARIES} ${ZLIB_LIBRARIES})

set_target_properties(TCBasic PROPERTIES FOLDER Modules)
set_target_properties(TCBasic PROPERTIES COMPILE_DEFINITIONS "TCBASE_EXPORTS")
set_target_properties(TCBasic PROPERTIES PUBLIC_HEADER "${TC_BASIC_PUBLIC_HEADER}")

install(TARGETS TCBasic
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Base
)
