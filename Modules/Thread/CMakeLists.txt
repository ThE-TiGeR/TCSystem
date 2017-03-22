find_package(Threads)

include_directories (
   inc
   ../Basic/inc
)

set (TC_THREAD_PUBLIC_HEADER
    inc/TCMTApi.h
    inc/TCMTCondition.h
    inc/TCMTCommandMessage.h
    inc/TCMTEvent.h
    inc/TCMTFactory.h
    inc/TCMTLock.h
    inc/TCMTLockable.h
    inc/TCMTMessage.h
    inc/TCMTMessageDispatcher.h
    inc/TCMTMutex.h
    inc/TCMTSemaphore.h
    inc/TCMTThread.h
    inc/TCMTThreadObject.h
)

if (TCSYSTEM_TESTS)
  set (TC_THREAD_TESTS
	tests/TCMTThreadTestSuite.h
	tests/TCMTEventTest.cpp
	tests/TCMTEventTest.h
	tests/TCMTMutexTest.cpp
	tests/TCMTMutexTest.h
	tests/TCMTSemaphoreTest.cpp
	tests/TCMTSemaphoreTest.h
	tests/TCMTThreadTest.cpp
	tests/TCMTThreadTest.h
	tests/TCMTThreadTestSuite.cpp
  )
endif()

add_library (TCThread
	src/TCMTCommandExecutionThread.h
	src/TCMTConditionPthread.cpp
	src/TCMTConditionPthread.h
	src/TCMTConditionWin32.cpp
	src/TCMTConditionWin32.h
	src/TCMTDebugNew.cpp
	src/TCMTEventPthread.cpp
	src/TCMTEventPthread.h
	src/TCMTEventWin32.cpp
	src/TCMTEventWin32.h
	src/TCMTFactory.cpp
	src/TCMTMessage.cpp
	src/TCMTMessageDispatcherImp.h
	src/TCMTMessageQueue.cpp
	src/TCMTMessageQueue.h
	src/TCMTMutexPthread.cpp
	src/TCMTMutexPthread.h
	src/TCMTMutexWin32.cpp
	src/TCMTMutexWin32.h
	src/TCMTOS.h
	src/TCMTSemaphorePthread.cpp
	src/TCMTSemaphorePthread.h
	src/TCMTSemaphoreWin32.cpp
	src/TCMTSemaphoreWin32.h
	src/TCMTThreadBase.cpp
	src/TCMTThreadBase.h
	src/TCMTThreadPthread.cpp
	src/TCMTThreadPthread.h
	src/TCMTThreadWin32.cpp
	src/TCMTThreadWin32.h
	${TC_THREAD_PUBLIC_HEADER}
	${TC_THREAD_TESTS}
)

target_link_libraries(TCThread
    TCBasic
    ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(TCThread PROPERTIES FOLDER Modules)
set_target_properties(TCThread PROPERTIES COMPILE_DEFINITIONS "TCMT_EXPORTS")
set_target_properties(TCThread PROPERTIES PUBLIC_HEADER "${TC_THREAD_PUBLIC_HEADER}")

if (TCSYSTEM_ENABLE_INSTALL)
    install(TARGETS TCThread
       RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
       LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
       ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
       PUBLIC_HEADER DESTINATION include
       COMPONENT Threading
    )
endif()
