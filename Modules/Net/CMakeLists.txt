include (FindOpenSSL)

add_library (TCNet
	TCNetAddress.cpp
	TCNetBase.cpp
	TCNetBase.h
	TCNetDebugNew.cpp
	TCNetFactory.cpp
	TCNetOS.h
	TCNetSocketImp.cpp
	TCNetSocketImp.h
	TCNetSocketServerImp.cpp
	TCNetSocketServerImp.h
	TCNetSSLFactory.cpp
	TCNetSSLSocket.cpp
	TCNetSslSocket.h
	TCNetUtil.cpp
	TCNetUtil.h
	${TC_NET_TESTS_SOURCES}
	${TCSystem_SOURCE_DIR}/inc/TCNetApi.h
	${TCSystem_SOURCE_DIR}/inc/TCNetAddress.h
	${TCSystem_SOURCE_DIR}/inc/TCNetBroadCastSocket.h
	${TCSystem_SOURCE_DIR}/inc/TCNetReadWriteSocket.h
	${TCSystem_SOURCE_DIR}/inc/TCNetFactory.h
	${TCSystem_SOURCE_DIR}/inc/TCNetSSLFactory.h
	${TCSystem_SOURCE_DIR}/inc/TCNetSocket.h
	${TCSystem_SOURCE_DIR}/inc/TCNetSocketServer.h
	${TCSystem_SOURCE_DIR}/inc/TCNetTypes.h
)

if (OPENSSL_FOUND)
	include_directories (${OPENSSL_INCLUDE_DIR})
	target_link_libraries (TCNet ${OPENSSL_LIBRARIES})
	set_target_properties(TCNet PROPERTIES COMPILE_DEFINITIONS TCNET_EXPORTS)
else ()
	set_target_properties(TCNet PROPERTIES COMPILE_DEFINITIONS "TCNET_NO_OPENSSL;TCNET_EXPORTS")
endif ()

if (MSVC)
	target_link_libraries (TCNet ws2_32.lib)
endif ()

target_link_libraries(TCNet
    TCThread
    TCBasic
)
add_dependencies(TCNet
    TCThread
    TCBasic
)

set_target_properties(TCNet PROPERTIES FOLDER Modules)
