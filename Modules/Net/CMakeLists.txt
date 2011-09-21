include (FindOpenSSL)

if (OPENSSL_FOUND)
	include_directories(
		inc
        ../Basic/inc
        ../Thread/inc
		${OPENSSL_INCLUDE_DIR}
	)
else ()
	include_directories(
		inc
	)
endif ()

set (TC_NET_PUBLIC_HEADER
    inc/TCNetApi.h
    inc/TCNetAddress.h
    inc/TCNetBroadCastSocket.h
    inc/TCNetReadWriteSocket.h
    inc/TCNetFactory.h
    inc/TCNetSSLFactory.h
    inc/TCNetSocket.h
    inc/TCNetSocketServer.h
    inc/TCNetTypes.h
)

add_library (TCNet
	src/TCNetAddress.cpp
	src/TCNetBase.cpp
	src/TCNetBase.h
	src/TCNetDebugNew.cpp
	src/TCNetFactory.cpp
	src/TCNetOS.h
	src/TCNetSocketImp.cpp
	src/TCNetSocketImp.h
	src/TCNetSocketServerImp.cpp
	src/TCNetSocketServerImp.h
	src/TCNetSSLFactory.cpp
	src/TCNetSSLSocket.cpp
	src/TCNetSslSocket.h
	src/TCNetUtil.cpp
	src/TCNetUtil.h
	tests/TCNetAddressTest.cpp
	tests/TCNetAddressTest.h
	tests/TCNetTestSuit.cpp
	tests/TCNetTestSuite.h
	${TC_NET_PUBLIC_HEADER}
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

set_target_properties(TCNet PROPERTIES FOLDER Modules)
set_target_properties(TCNet PROPERTIES PUBLIC_HEADER "${TC_NET_PUBLIC_HEADER}")

install(TARGETS TCNet
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Networking
)

