include (FindThreads)

set (TC_THREAD_PUBLIC_HEADER
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

add_library (TCThread
	TCMTCommandExecutionThread.h
	TCMTConditionPthread.cpp
	TCMTConditionPthread.h
	TCMTConditionWin32.cpp
	TCMTConditionWin32.h
	TCMTDebugNew.cpp
	TCMTEventPthread.cpp
	TCMTEventPthread.h
	TCMTEventWin32.cpp
	TCMTEventWin32.h
	TCMTFactory.cpp
	TCMTMessage.cpp
	TCMTMessageDispatcherImp.h
	TCMTMessageQueue.cpp
	TCMTMessageQueue.h
	TCMTMutexPthread.cpp
	TCMTMutexPthread.h
	TCMTMutexWin32.cpp
	TCMTMutexWin32.h
	TCMTOS.h
	TCMTSemaphorePthread.cpp
	TCMTSemaphorePthread.h
	TCMTSemaphoreWin32.cpp
	TCMTSemaphoreWin32.h
	TCMTThreadBase.cpp
	TCMTThreadBase.h
	TCMTThreadPthread.cpp
	TCMTThreadPthread.h
	TCMTThreadWin32.cpp
	TCMTThreadWin32.h
	Tests/TCMTThreadTestSuite.h
	Tests/TCMTEventTest.cpp
	Tests/TCMTEventTest.h
	Tests/TCMTMutexTest.cpp
	Tests/TCMTMutexTest.h
	Tests/TCMTSemaphoreTest.cpp
	Tests/TCMTSemaphoreTest.h
	Tests/TCMTThreadTest.cpp
	Tests/TCMTThreadTest.h
	Tests/TCMTThreadTestSuite.cpp
	${TC_THREAD_PUBLIC_HEADER}
)

target_link_libraries(TCThread
    TCBasic
    ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(TCThread PROPERTIES FOLDER Modules)
set_target_properties(TCThread PROPERTIES COMPILE_DEFINITIONS "TCMT_EXPORTS")
set_target_properties(TCThread PROPERTIES PUBLIC_HEADER "${TC_THREAD_PUBLIC_HEADER}")

install(TARGETS TCThread
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Threading
)

