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
// Copyright (C) 2003 - 2012 Thomas Goessler. All Rights Reserved. 
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

#include "TCNetUtil.h"

#include "TCFactory.h"
#include "TCNetOS.h"
#include "TCSystem.h"
#include "TCOutput.h"

#include <cstring>

#include "TCNewEnable.h"

static bool s_is_initialised;

namespace tc
{
   namespace net
   {
      namespace imp
      {
         namespace util
         {

            bool InitSocket()
            {
#ifdef TCOS_WINDOWS
               // the first thing what we do in windows we init the sockets
               if (s_is_initialised == false)
               {
                  WORD wVersionReq = MAKEWORD(2, 2);
                  WSADATA wsaData;
                  sint32 err = ::WSAStartup(wVersionReq, &wsaData);
                  if (err == 0)
                  {
                     s_is_initialised = true;
                  }
               }
#else
               s_is_initialised = true;
#endif
               return s_is_initialised;
            }

            bool DeInitSocket()
            {
#ifdef TCOS_WINDOWS
               ::WSACleanup();
#endif
               s_is_initialised = false;

               return true;
            }

            PortNumber FindFreePort()
            {
               RngPtr rng = tc::factory::Create69069Rng();

               PortNumber port = 0;
               while(port == 0)
               {
                  port = static_cast<PortNumber>(rng->GetRandomNumber(1, 65536));
                  SocketId id = CreateSocket(SOCK_STREAM, 0);

                  SocketAddress ssa;
                  std::memset(&ssa, 0, sizeof(ssa));

                  ssa.sin_family      = AF_INET;
                  ssa.sin_addr.s_addr = htonl(INADDR_ANY);
                  ssa.sin_port        = htons(port);

                  // check binding
                  if (::bind(id, reinterpret_cast<const struct sockaddr*>(&ssa),
                     sizeof(SocketAddress)) < 0)
                  {
                     CloseSocket(id);
                     port = 0;
                     continue;
                  }

                  // start listening socket
                  if(::listen(id, 1) != 0)
                  {
                     CloseSocket(id);
                     port = 0;
                     continue;
                  }

                  CloseSocket(id);
               }

               return port;
            }

            SocketId CreateSocket(sint32 type, const std::string& protocol)
            {
               // make shure sockkets are initialized
               InitSocket();

               struct protoent *p = 0;
               if (!protocol.empty())
               {
                  p = ::getprotobyname(protocol.c_str());
                  if (!p)
                  {
                     return static_cast<SocketId>(invalid_socket);
                  }
               }

               SocketId id = static_cast<SocketId>(::socket(AF_INET, type, p ? p -> p_proto : 0));
               if (id == invalid_socket)
               {
                  return static_cast<SocketId>(invalid_socket);
               }

               if (type == SOCK_STREAM)
               {
                  int optval = 1;
                  if (::setsockopt(id, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) == socket_error)
                  {
                     CloseSocket(id);
                     return static_cast<SocketId>(invalid_socket);
                  }

                  if (::setsockopt(id, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval, sizeof(optval)) == socket_error)
                  {
                     CloseSocket(id);
                     return static_cast<SocketId>(invalid_socket);
                  }
               }

               return id;
            }

            bool CloseSocket(SocketId &id)
            {
               if (id != invalid_socket)
               {
#ifdef TCOS_WINDOWS
                  ::closesocket(id);
#else
                  ::close(id);
#endif
               }

               id = static_cast<SocketId>(invalid_socket);
               return true;
            }

            bool SetTcpNoDelay(SocketId id, bool on_or_off)
            {
               int para = on_or_off ? 1 : 0;

#if TCOS_WINDOWS
               char* p = (char*) &para;
#else
               int* p = &para;
#endif
               if (::setsockopt(id, IPPROTO_TCP, TCP_NODELAY, p, sizeof(para)) == socket_error)
               {
                  return false;
               }

               return true;
            }

            bool SetNoBlocking(SocketId id, bool on_off)
            {
#ifdef TCOS_WINDOWS
               unsigned long on = on_off ? 1 : 0;
               if (::ioctlsocket(id, FIONBIO, &on) != 0)
               {
                  return false;
               }
#else
               sint32 opts = fcntl(id, F_GETFL);
               // unable to get options
               if (opts < 0)
               {
                  return false;
               }
               if (on_off)
               {
                  opts = opts | O_NONBLOCK;
               }
               else
               {
                  opts = opts & ~O_NONBLOCK;
               }

               if (fcntl(id, F_SETFL, opts) < 0)
               {
                  return false;
               }
#endif
               return true;
            }

            bool SetReuseAddress(SocketId sd, bool reuse)
            {
               int para = reuse ? 1 : 0;

#ifdef TCOS_WINDOWS
               char* p = (char*) &para;
#else
               int* p = &para;
#endif
               if (::setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, p, sizeof(para)) == SOCKET_ERROR)
               {
                  PrintSocketError("tc::net::util::CreateSocketDescriptor failed create socket",
                     true);
                  return false;
               }

               return true;
            }

            void PrintSocketError(const char* error_message, bool is_error)
            {
#ifdef TCOS_WINDOWS
               int error = WSAGetLastError();
#else
               int error = errno;
#endif
               std::string error_str = system::GetErrorMessage(error);
               if (is_error)
               {
                  TCERROR3("NET", "%s(%d, %s)", error_message, error, error_str.c_str());
               }
               else
               {
                  TCTRACE3("NET", 10, "%s(%d, %s)", error_message, error, error_str.c_str());
               }
            }

         }
      }
   }
}
