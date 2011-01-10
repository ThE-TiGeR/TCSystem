include(FindBZip2)
include(FindZLIB)

include_directories (
   "${BZIP2_INCLUDE_DIR}"
   "${ZLIB_INCLUDE_DIRS}"
)

aux_source_directory(. TC_BASIC_SOURCES)

add_library (TCBasic
	${TC_BASIC_SOURCES}
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

if (MSVC)
	target_link_libraries (TCBasic iphlpapi.lib)
endif ()

target_link_libraries (TCBasic ${BZIP2_LIBRARIES} ${ZLIB_LIBRARIES})

set_target_properties(TCBasic PROPERTIES FOLDER Modules)
set_target_properties(TCBasic PROPERTIES COMPILE_DEFINITIONS "TCBASE_EXPORTS")
