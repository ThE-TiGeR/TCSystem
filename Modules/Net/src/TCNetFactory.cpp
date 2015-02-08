//*******************************************************************************
//
// *******   ***   ***               *
//    *     *     *                  *
//    *    *      *                *****
//    *    *       ***  *   *   **   *    **    ***
//    *    *          *  * *   *     *   ****  * * *
//    *     *         *   *      *   * * *     * * *
//    *      ***   ***    *     **   **   **   *   *
//                        *
//*******************************************************************************
// see https://bitbucket.org/the____tiger/tcsystem for details.
// Copyright (C) 2003 - 2014 Thomas Goessler. All Rights Reserved. 
//*******************************************************************************
//
// TCSystem is the legal property of its developers.
// Please refer to the COPYRIGHT file distributed with this source distribution.
// 
// This library is free software; you can redistribute it and/or             
// modify it under the terms of the GNU Lesser General Public                
// License as published by the Free Software Foundation; either              
// version 2.1 of the License, or (at your option) any later version.        
//                                                                           
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
//                                                                           
// You should have received a copy of the GNU Lesser General Public          
// License along with this library; if not, write to the Free Software       
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
//*******************************************************************************
//  $Id$
//*******************************************************************************

#include "TCNetFactory.h"

#include "TCNetSocketServerImp.h"
#include "TCNetSocketImp.h"
#include "TCNetUtil.h"
#include "TCNetOS.h"

#include <cstring>

#include "TCNewEnable.h"

namespace tc
{
   namespace net
   {
      namespace factory
      {
         static const char* s_protocol_2_string[] =
         {
            "udp",
            "tcp"
         };

         static const char* Protocol2String(Protocol protocol)
         {
            return s_protocol_2_string[protocol];
         }

         ReadWriteSocketPtr Connect(const Address& ip_addr, PortNumber port, Protocol protocol)
         {
            SocketId s = imp::util::CreateSocket(protocol == TCP ? SOCK_STREAM : SOCK_DGRAM, Protocol2String(protocol));
            if (s == imp::invalid_socket)
            {
               return ReadWriteSocketPtr();
            }

            sockaddr_in sa;
            std::memset(&sa, 0, sizeof(sa));
            sa.sin_family = AF_INET;
            sa.sin_port   = htons(port);
            sa.sin_addr   = ip_addr;

            if(::connect(s, (sockaddr*) &sa, sizeof(sa)) == SOCKET_ERROR)
            {
               imp::util::PrintSocketError("tc::net::factory::Connect, connect failed", false);
               imp::util::CloseSocket(s);
               return ReadWriteSocketPtr();
            }

            imp::util::SetTcpNoDelay(s, true);

            return ReadWriteSocketPtr (new imp::SocketImp(s));
         }

         static SharedPtr<imp::SocketImp> CreateListenSocket(PortNumber port, uint32_t connections, Protocol protocol, const Address& ip_addr)
         {
            SocketId s = imp::util::CreateSocket(protocol == TCP ? SOCK_STREAM : SOCK_DGRAM, Protocol2String(protocol));
            if (s == imp::invalid_socket)
            {
               return SharedPtr<imp::SocketImp>();
            }

            imp::util::SetReuseAddress(s, true);

            sockaddr_in sa;
            std::memset(&sa, 0, sizeof(sa));
            sa.sin_family      = AF_INET;
            sa.sin_port        = htons(port);
            sa.sin_addr        = ip_addr;
            if (::bind(s, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
            {
               imp::util::PrintSocketError("tc::net::factory::CreateListenSocket, bind failed", true);
               imp::util::CloseSocket(s);

               return SharedPtr<imp::SocketImp>();
            }

            if (protocol != UDP)
            {
               if (::listen(s, connections) == SOCKET_ERROR)
               {
                  imp::util::PrintSocketError("tc::net::factory::CreateListenSocket, listen failed", true);
                  imp::util::CloseSocket(s);

                  return SharedPtr<imp::SocketImp>();
               }
            }

            return SharedPtr<imp::SocketImp>(new imp::SocketImp(s));
         }

         SocketPtr CreateTcpListenSocket(PortNumber port, uint32_t connections, const Address& ip_addr)
         {
            return ReadWriteSocketPtr(CreateListenSocket(port, connections, TCP, ip_addr));
         }

         BroadcastReadSocketPtr CreateUdpListenSocket(PortNumber port, uint32_t connections, const Address& ip_addr)
         {
            return CreateListenSocket(port, connections, UDP, ip_addr);
         }

         ReadWriteSocketPtr Accept(SocketPtr socket)
         {
            Address client_ip;
            return Accept(socket, client_ip);
         }

         ReadWriteSocketPtr Accept(SocketPtr socket, Address& ip)
         {
            PortNumber port;
            return Accept(socket, ip, port);
         }

         ReadWriteSocketPtr Accept(SocketPtr socket, Address& ip, PortNumber& port)
         {
            sockaddr_in s_address;
            imp::AddrLength sockaddr_size = sizeof(s_address);

            SocketId s = ::accept(socket->GetSocket(), 
               reinterpret_cast<sockaddr*>(&s_address), &sockaddr_size);

            if (s == imp::invalid_socket)
            {
               imp::util::PrintSocketError("tc::net::factory::Accept failed", true);
               return ReadWriteSocketPtr();
            }

            imp::util::SetTcpNoDelay(s, true);
            ip = s_address;
            port = ntohs(s_address.sin_port);

            return ReadWriteSocketPtr(new imp::SocketImp(s));
         }

         BroadcastWriteSocketPtr CreateUdpBroadcastSocket()
         {
            SocketId s = imp::util::CreateSocket(SOCK_DGRAM, Protocol2String(UDP));
            if (s == imp::invalid_socket)
            {
               return BroadcastWriteSocketPtr();
            }

            imp::util::SetReuseAddress(s, true);
            int enable_broadcast = 1;
            if (::setsockopt(s, SOL_SOCKET, SO_BROADCAST,
               reinterpret_cast<const char*>(&enable_broadcast), sizeof(enable_broadcast))
               == SOCKET_ERROR)
            {
               imp::util::PrintSocketError("tc::net::factory::CreateUdpBroadcastSocket "
                  "setsockopt SO_BROADCAST failed", true);
               return BroadcastWriteSocketPtr();
            }
            return BroadcastWriteSocketPtr(new imp::SocketImp(s));

//             ReadWriteSocketPtr socket = CreateUdpListenSocket(port, connections);
//             int enable_broadcast = 1;
//             if (::setsockopt(socket->GetSocket(), SOL_SOCKET, SO_BROADCAST,
//                reinterpret_cast<const char*>(&enable_broadcast), sizeof(enable_broadcast))
//                == SOCKET_ERROR)
//             {
//                imp::util::PrintSocketError("tc::net::factory::CreateUdpBroadcastSocket "
//                   "setsockopt SO_BROADCAST failed", true);
//                return ReadWriteSocketPtr();
//             }
// 
//             return socket;
         }

         SocketServerPtr CreateSocketServer()
         {
            return SocketServerPtr(new imp::SocketServerImp);
         }
      }
   }
}
