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

#include "TCNetSocketImp.h"

#include "TCNetUtil.h"
#include "TCNetOS.h"
#include "TCOutput.h"
#include "TCSystem.h"

#include <cstring>

#include "TCNewEnable.h"

namespace tc
{
   namespace net
   {
      namespace imp
      {
         SocketImp::SocketImp(SocketId s)
            :m_socket(s)
         {

         }

         SocketImp::~SocketImp()
         {
            Close();
         }

         bool SocketImp::IsOpened() const
         {
            return m_socket != invalid_socket;
         }

         bool SocketImp::Close()
         {
            return util::CloseSocket(m_socket);
         }

         SocketId SocketImp::GetSocket()
         {
            return m_socket;
         }

         uint64_t SocketImp::ReadBytes(void* buffer, uint64_t size)
         {
            if ((!IsOpened()) || (size == 0))
            {
               return 0;
            }

            int nbytes = ::recv(m_socket, (char*)buffer, int(size), 0);
            if(nbytes == 0 || nbytes == socket_error)
            {
               util::PrintSocketError("net::imp::SocketImp::ReceiveBytes, recv failed", nbytes == socket_error);
               Close();
               return 0;
            }

            return nbytes;
         }

         uint64_t SocketImp::ReadBytes(void* buffer, uint64_t size, const Time& timeout_in)
         {
            if ((!IsOpened()) || (size == 0) || !buffer)
            {
               return false;
            }

            fd_set read_set;
            FD_ZERO(&read_set);
#ifdef _MSC_VER
            // disable warning warning C4127: conditional expression is constant
            // happens for FD_SET
#   pragma warning (disable: 4127)
#endif
            FD_SET(m_socket, &read_set);

            Time timeout(timeout_in);
            Time start_time = Time::Now();
            uint64_t bytes_received = 0;
            do
            {
               timeval rx_timeout = 
               {
                  static_cast<long>(timeout.Seconds()), 
                  static_cast<long>(timeout.NanoSeconds() / 1000)
               };

               int32_t s = ::select(1 + 1, &read_set, 0, 0, &rx_timeout);
               if (s == socket_error)
               {
                  util::PrintSocketError("SocketImp::ReceiveBytes with timeout select failed", true);
                  return false;
               }
               else if (s == 0)
               {
                  util::PrintSocketError("SocketImp::ReceiveBytes TIMEOUT!!!", true);
                  return false;
               }

               uint64_t rx_len = ReadBytes(buffer, size);
               if(rx_len == 0)
               {
                  return false;
               }
               bytes_received += rx_len;

               if (bytes_received < size)
               {
                  Time elapsed = Time::Since(start_time);
                  if (elapsed < timeout)
                  {
                     timeout -= elapsed;
                  }
                  else
                  {
                     return bytes_received;
                  }
               }

            } while (bytes_received < size);

            return bytes_received;
         }

         uint64_t SocketImp::ReadBytesFrom(void* buffer, uint64_t size, Address& ip)
         {
            ip = 0;
            if ((!IsOpened()) || (size==0))
            {
               return 0;
            }

            sockaddr_in s_address;
            AddrLength sockaddr_size = sizeof(s_address);
            int nbytes = ::recvfrom(m_socket, (char*)buffer, int(size), 0, 
                reinterpret_cast<sockaddr*>(&s_address),  &sockaddr_size);
            if(nbytes == 0 || nbytes == socket_error)
            {
               util::PrintSocketError("net::imp::SocketImp::ReceiveBytesFrom, recvfrom failed", nbytes == socket_error);
               Close();
               return 0;
            }

            ip = s_address;

            return nbytes;
         }

         uint64_t SocketImp::WriteBytes(const void* buffer_in, uint64_t size)
         {
            if (!IsOpened())
            {
               return 0;
            }

            const char* buffer = static_cast<const char*>(buffer_in);
            uint64_t nbytes_total = 0;
            while (nbytes_total < size)
            {
               int nbytes = ::send(m_socket, buffer, int(size-nbytes_total), 0);
               if(nbytes == 0 || nbytes == socket_error)
               {
                  util::PrintSocketError("net::imp::SocketImp::SendBytes, send failed", true);
                  Close();
                  return 0;
               }

               nbytes_total += nbytes;
               buffer += nbytes;
            }

            return nbytes_total;
         }

         uint64_t SocketImp::WriteBytesTo(const void* buffer_in, uint64_t size, const Address& ip, PortNumber port)
         {
            if (!IsOpened())
            {
               return 0;
            }

            sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_port = htons(port);
            address.sin_addr = ip;

            const char* buffer = static_cast<const char*>(buffer_in);
            uint64_t nbytes_total = 0;
            while (nbytes_total < size)
            {
               int32_t nbytes = ::sendto(m_socket, buffer, int(size-nbytes_total), 
                   0, reinterpret_cast<sockaddr*>(&address), sizeof(address));
               if(nbytes == 0 || nbytes == socket_error)
               {
                  util::PrintSocketError("net::imp::SocketImp::SendBytesTo, sendto failed", true);
                  Close();
                  return 0;
               }

               nbytes_total += nbytes;
               buffer += nbytes;
            }

            return nbytes_total;
         }

      }
   }
}

