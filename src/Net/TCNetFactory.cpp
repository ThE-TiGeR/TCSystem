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
// see http://sourceforge.net/projects/tcsystem/ for details.
// Copyright (C) 2003 - 2010 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCNetFactory.cpp 1007 2010-07-21 05:52:41Z the_____tiger $
//*******************************************************************************

#include "TCNetFactory.h"

#include "TCNetSocketServerImp.h"
#include "TCNetSocketImp.h"
#include "TCNetUtil.h"
#include "TCNetOS.h"

#include <cstring>

#include "TCNewEnable.h"

namespace TC
{
   namespace Net
   {
      namespace Factory
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
            SocketId s = Impl::Util::CreateSocket(SOCK_STREAM, Protocol2String(protocol));
            if (s == Impl::invalid_socket)
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
               Impl::Util::PrintSocketError("TC::Net::Factory::Connect, connect failed", false);
               Impl::Util::CloseSocket(s);
               return ReadWriteSocketPtr();
            }

            Impl::Util::SetTcpNoDelay(s, true);

            return ReadWriteSocketPtr (new Impl::SocketImp(s));
         }

         static SharedPtr<Impl::SocketImp> CreateListenSocket(PortNumber port, uint32 connections, Protocol protocol)
         {
            SocketId s = Impl::Util::CreateSocket(protocol == TCP ? SOCK_STREAM : SOCK_DGRAM, Protocol2String(protocol));
            if (s == Impl::invalid_socket)
            {
               return SharedPtr<Impl::SocketImp>();
            }

            Impl::Util::SetReuseAddress(s, true);

            sockaddr_in sa;
            std::memset(&sa, 0, sizeof(sa));
            sa.sin_family      = AF_INET;
            sa.sin_port        = htons(port);
            sa.sin_addr.s_addr = INADDR_ANY;
            if (::bind(s, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
            {
               Impl::Util::PrintSocketError("TC::Net::Factory::CreateListenSocket, bind failed", true);
               Impl::Util::CloseSocket(s);

               return SharedPtr<Impl::SocketImp>();
            }

            if (protocol != UDP)
            {
               if (::listen(s, connections) == SOCKET_ERROR)
               {
                  Impl::Util::PrintSocketError("TC::Net::Factory::CreateListenSocket, listen failed", true);
                  Impl::Util::CloseSocket(s);

                  return SharedPtr<Impl::SocketImp>();
               }
            }

            return SharedPtr<Impl::SocketImp>(new Impl::SocketImp(s));
         }

         SocketPtr CreateTcpListenSocket(PortNumber port, uint32 connections)
         {
            return ReadWriteSocketPtr(CreateListenSocket(port, connections, TCP));
         }

         BroadcastReadSocketPtr CreateUdpListenSocket(PortNumber port, uint32 connections)
         {
            return CreateListenSocket(port, connections, UDP);
         }

         ReadWriteSocketPtr Accept(SocketPtr socket)
         {
            Address client_ip;
            return Accept(socket, client_ip);
         }

         ReadWriteSocketPtr Accept(SocketPtr socket, Address& ip)
         {
            sockaddr_in s_address;
            Impl::AddrLength sockaddr_size = sizeof(s_address);

            SocketId s = ::accept(socket->GetSocket(), 
               reinterpret_cast<sockaddr*>(&s_address), &sockaddr_size);

            if (s == Impl::invalid_socket)
            {
               Impl::Util::PrintSocketError("TC::Net::Factory::Accept failed", true);
               return ReadWriteSocketPtr();
            }

            Impl::Util::SetTcpNoDelay(s, true);
            ip = s_address;

            return ReadWriteSocketPtr(new Impl::SocketImp(s));
         }

         BroadcastWriteSocketPtr CreateUdpBroadcastSocket()
         {
            SocketId s = Impl::Util::CreateSocket(SOCK_DGRAM, Protocol2String(UDP));
            if (s == Impl::invalid_socket)
            {
               return BroadcastWriteSocketPtr();
            }

            Impl::Util::SetReuseAddress(s, true);
            int enable_broadcast = 1;
            if (::setsockopt(s, SOL_SOCKET, SO_BROADCAST,
               reinterpret_cast<const char*>(&enable_broadcast), sizeof(enable_broadcast))
               == SOCKET_ERROR)
            {
               Impl::Util::PrintSocketError("TC::Net::Factory::CreateUdpBroadcastSocket "
                  "setsockopt SO_BROADCAST failed", true);
               return BroadcastWriteSocketPtr();
            }
            return BroadcastWriteSocketPtr(new Impl::SocketImp(s));

//             ReadWriteSocketPtr socket = CreateUdpListenSocket(port, connections);
//             int enable_broadcast = 1;
//             if (::setsockopt(socket->GetSocket(), SOL_SOCKET, SO_BROADCAST,
//                reinterpret_cast<const char*>(&enable_broadcast), sizeof(enable_broadcast))
//                == SOCKET_ERROR)
//             {
//                Impl::Util::PrintSocketError("TC::Net::Factory::CreateUdpBroadcastSocket "
//                   "setsockopt SO_BROADCAST failed", true);
//                return ReadWriteSocketPtr();
//             }
// 
//             return socket;
         }

         SocketServerPtr CreateSocketServer()
         {
            return SocketServerPtr(new Impl::SocketServerImp);
         }
      }
   }
}
