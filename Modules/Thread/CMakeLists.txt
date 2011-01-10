aux_source_directory(. TC_THREAD_SOURCES)

add_library (TCThread
	${TC_THREAD_SOURCES}
	${TCSystem_SOURCE_DIR}/inc/TCMTApi.h
	${TCSystem_SOURCE_DIR}/inc/TCMTCondition.h
	${TCSystem_SOURCE_DIR}/inc/TCMTCommandMessage.h
	${TCSystem_SOURCE_DIR}/inc/TCMTEvent.h
	${TCSystem_SOURCE_DIR}/inc/TCMTFactory.h
	${TCSystem_SOURCE_DIR}/inc/TCMTLock.h
	${TCSystem_SOURCE_DIR}/inc/TCMTLockable.h
	${TCSystem_SOURCE_DIR}/inc/TCMTMessage.h
	${TCSystem_SOURCE_DIR}/inc/TCMTMessageDispatcher.h
	${TCSystem_SOURCE_DIR}/inc/TCMTMutex.h
	${TCSystem_SOURCE_DIR}/inc/TCMTSemaphore.h
	${TCSystem_SOURCE_DIR}/inc/TCMTThread.h
	${TCSystem_SOURCE_DIR}/inc/TCMTThreadObject.h
)

target_link_libraries(TCThread
    TCBasic
)
add_dependencies(TCThread
    TCBasic
)

set_target_properties(TCThread PROPERTIES FOLDER Modules)
set_target_properties(TCThread PROPERTIES COMPILE_DEFINITIONS "TCMT_EXPORTS")
