include(FindBZip2)
include(FindZLIB)

set (TC_BASIC_PUBLIC_HEADER
	${TCSystem_SOURCE_DIR}/inc/TCApplication.h
	${TCSystem_SOURCE_DIR}/inc/TCAssert.h
	${TCSystem_SOURCE_DIR}/inc/TCCodec.h
	${TCSystem_SOURCE_DIR}/inc/TCException.h
	${TCSystem_SOURCE_DIR}/inc/TCOutput.h
	${TCSystem_SOURCE_DIR}/inc/TCDebugNew.h
	${TCSystem_SOURCE_DIR}/inc/TCDefines.h
	${TCSystem_SOURCE_DIR}/inc/TCDelete.h
	${TCSystem_SOURCE_DIR}/inc/TCFactory.h
	${TCSystem_SOURCE_DIR}/inc/TCFile.h
	${TCSystem_SOURCE_DIR}/inc/TCFileName.h
	${TCSystem_SOURCE_DIR}/inc/TCHashTable.h
	${TCSystem_SOURCE_DIR}/inc/TCHeap.h
	${TCSystem_SOURCE_DIR}/inc/TCInterlocked.h
	${TCSystem_SOURCE_DIR}/inc/TCMLSHandler.h
	${TCSystem_SOURCE_DIR}/inc/TCMemoryMappedFile.h
	${TCSystem_SOURCE_DIR}/inc/TCNew.h
	${TCSystem_SOURCE_DIR}/inc/TCNewDisable.h
	${TCSystem_SOURCE_DIR}/inc/TCNewEnable.h
	${TCSystem_SOURCE_DIR}/inc/TCNonCopyable.h
	${TCSystem_SOURCE_DIR}/inc/TCRng.h
	${TCSystem_SOURCE_DIR}/inc/TCScopedPtr.h
	${TCSystem_SOURCE_DIR}/inc/TCScopedArray.h
	${TCSystem_SOURCE_DIR}/inc/TCSharedPtr.h
	${TCSystem_SOURCE_DIR}/inc/TCSharedPtrImpl.h
	${TCSystem_SOURCE_DIR}/inc/TCStream.h
	${TCSystem_SOURCE_DIR}/inc/TCStreamAble.h
	${TCSystem_SOURCE_DIR}/inc/TCString.h
	${TCSystem_SOURCE_DIR}/inc/TCSystem.h
	${TCSystem_SOURCE_DIR}/inc/TCStlAlgorithm.h
	${TCSystem_SOURCE_DIR}/inc/TCStlTypes.h
	${TCSystem_SOURCE_DIR}/inc/TCTime.h
	${TCSystem_SOURCE_DIR}/inc/TCTypes.h
	${TCSystem_SOURCE_DIR}/inc/TCUtil.h
	${TCSystem_SOURCE_DIR}/inc/TCValue.h
	${TCSystem_SOURCE_DIR}/inc/TCWeakPtr.h
	${TCSystem_SOURCE_DIR}/inc/TCWFileName.h
	${TCSystem_SOURCE_DIR}/inc/TCWString.h
)

add_library (TCBasic
	TCApplication.cpp
	TCAsciiCodec.cpp
	TCAsciiCodec.h
	TCBinaryCodec.cpp
	TCBinaryCodec.h
	TCBz2FileStream.cpp
	TCBz2FileStream.h
	TCClock.cpp
	TCClock.h
	TCDebugHeap.cpp
	TCDebugHeap.h
	TCDebugNew.cpp
	TCException.cpp
	TCFactory.cpp
	TCFile.cpp
	TCFileName.cpp
	TCFileNameImp.h
	TCFilePosix.cpp
	TCFileStream.cpp
	TCFileStream.h
	TCFileWin32.cpp
	TCGzFileStream.cpp
	TCGzFileStream.h
	TCHeapCrtlib.cpp
	TCHeapCrtlib.h
	TCHeapMutex.h
	TCInterlocked.cpp
	TCMemoryMappedFilePosix.cpp
	TCMemoryMappedFilePosix.h
	TCMemoryMappedFileWin32.cpp
	TCMemoryMappedFileWin32.h
	TCMemoryStream.cpp
	TCMemoryStream.h
	TCMLSHandler.cpp
	TCNew.cpp
	TCOutput.cpp
	TCRng69069.cpp
	TCRng69069.h
	TCSettings.cpp
	TCSettings.h
	TCStreamAble.cpp
	TCStreamBase.cpp
	TCStreamBase.h
	TCString.cpp
	TCStringImp.h
	TCStringStream.cpp
	TCStringStream.h
	TCStringTraits.h
	TCSystem.cpp
	TCTime.cpp
	TCUnitSimpleTestResult.cpp
	TCUnitTestCase.cpp
	TCUnitTestSuite.cpp
	TCUnitTreeTestResult.cpp
	TCUnitTreeTestRunner.cpp
	TCUtil.cpp
	TCWFileName.cpp
	TCWstring.cpp
	TCWStringTraits.h
	Tests/TCWStringTest.h
	Tests/TCBasicTestSuite.cpp
	Tests/TCBasicTestSuite.h
	Tests/TCFileNameTest.cpp
	Tests/TCFileNameTest.h
	Tests/TCHashTableTest.cpp
	Tests/TCHashTableTest.h
	Tests/TCStreamTest.cpp
	Tests/TCStreamTest.h
	Tests/TCStringTest.cpp
	Tests/TCStringTest.h
	Tests/TCTimeTest.cpp
	Tests/TCTimeTest.h
	Tests/TCUtilTest.cpp
	Tests/TCUtilTest.h
	Tests/TCWFileNameTest.cpp
	Tests/TCWFileNameTest.h
	Tests/TCWStringTest.cpp	
        ${TC_BASIC_PUBLIC_HEADER}
)


include_directories (
   "${BZIP2_INCLUDE_DIR}"
   "${ZLIB_INCLUDE_DIRS}"
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
   LIBRARY DESTINATION lib
   PUBLIC_HEADER DESTINATION include
   COMPONENT Base
)
