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

#ifndef _TC_NET_UTIL_H_
#define _TC_NET_UTIL_H_

#include "TCNetBase.h"

namespace tc
{
namespace net
{
namespace imp
{

   /**
    * @addtogroup TC_NET_IMP
    * @{
    */
   namespace util
   {
      /**
      * returns port free port number between 1 and 65536
      */
      TC_DLL_LOCAL PortNumber FindFreePort();
      /**
      * open a new socket
      * @param socketType specifies the type of socket which should be opened
      * @param protocol name of the protocol to use e.g. "udp"
      * @return id of the socket (error is INVALID_SOCKET)
      */
      TC_DLL_LOCAL SocketId CreateSocket(int32_t socketType, const std::string& protocol);
      /**
      * closes a open socket
      * @param id is the open socket
      */
      TC_DLL_LOCAL bool CloseSocket(SocketId &id);
      /**
      * Set a socket to no blocking on or off
      * @param id is the open socket
      * @param on_or_off if no blocking should be turned on or off
      * @return if successful or not
      */
      TC_DLL_LOCAL bool SetNoBlocking(SocketId id, bool on_or_off);

      /**
       * Enable or disable the socket option TCP_NODELAY
       * which enables or disables the TCP´s Nagle algorithm
       * @param id is the open socket
       * @param on_or_off if no TCP_NODELAY should be turned on or off
       * @return if successful or not
       */
      TC_DLL_LOCAL bool SetTcpNoDelay(SocketId id, bool on_or_off);

      /**
       * Enable or disable the socket option SO_REUSEADDR
       * @param id is the open socket
       * @param reuse if no SO_REUSEADDR should be turned on or off
       * @return if successful or not
       */
      TC_DLL_LOCAL bool SetReuseAddress(SocketId sd, bool reuse);

      /**
       * init sockets (only needed on windows)
       * but to be compatible it is called also on other systems
       */
      TC_DLL_LOCAL bool InitSocket();

      /**
       * de init sockets (only needed on windows)
       * but to be compatible it is called also on other systems
       */
      TC_DLL_LOCAL bool DeInitSocket();

      /* Display current happend socket error */
      TC_DLL_LOCAL void PrintSocketError(const char* error_message, bool is_error);
   }

   /**
    * @}
    */

} // namespace imp
} // namespace net
} // namespace tc

#endif // _TC_NET_UTIL_H_
