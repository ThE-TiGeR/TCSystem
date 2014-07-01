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

#ifndef _TCNET_FACTORY_H_
#define _TCNET_FACTORY_H_

#include "TCNetApi.h"
#include "TCNetAddress.h"
#include "TCNetReadWriteSocket.h"
#include "TCNetBroadCastSocket.h"
#include "TCNetSocketServer.h"

namespace tc
{
   /**
    * @brief namspace including all network classes
    */
   namespace net
   {
      /**
      * @addtogroup TC_NET
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of tc::net::factory
      * @author Thomas Goessler
      */

      enum Protocol
      {
         UDP,
         TCP
      };

      /**
      * @}
      */

      /**
      * @brief factory for creating tc::net objects
      */
      namespace factory
      {
         /**
         * @addtogroup TC_NET
         * @{
         */

         /**
         * @brief Connects to specified server and returns the socket
         *
         * @param ip_addr  Network name or ip address of the server to which to connect
         * @@param port    Port on which to connect to the server
         * @@param protocol Protocol to use @see Protocol
         */
         TCNET_API ReadWriteSocketPtr Connect(const Address& ip_addr, PortNumber port, Protocol protocol);

         /**
         * @brief Creates a socket which can accept new TCP connections
         *
         * @param port        Port on which to listen for incoming connections
         * @param connections Number of connections it will accept
         * @param ip_addr     Ip Address to find the correct network adapter to listen on
         *                    (Any addres is used to listen on all adapters)
         */
         TCNET_API SocketPtr CreateTcpListenSocket(PortNumber port, uint32_t connections, const Address& ip_addr=Address::GetAnyAddress());

         /**
         * @brief Creates a socket which can accept new UPD connections
         *
         * @param port        Port on which to listen for incoming connections
         * @param connections Number of connections it will accept
         * @param ip_addr     Ip Address to find the correct network adapter to listen on
         *                    (Any addres is used to listen on all adapters)
         */
         TCNET_API BroadcastReadSocketPtr CreateUdpListenSocket(PortNumber port, uint32_t connections, const Address& ip_addr=Address::GetAnyAddress());

         /**
         * @brief Creates a socket which can accept new UPD Broadcast messages
         */
         TCNET_API BroadcastWriteSocketPtr CreateUdpBroadcastSocket();

         /**
         * @brief Accepts new socket connection on the opened socket
         *
         * @param socket Opened Listen socket on which to accept the new connection
         * @return The new opened socket
         */
         TCNET_API ReadWriteSocketPtr Accept(SocketPtr socket);

         /**
         * @brief Accepts new socket connection on the opened socket
         *
         * @param socket Opened Listen socket on which to accept the new connection
         * @param ip_addr [out]ip address of remote site
         * @return The new opened socket
         */
         TCNET_API ReadWriteSocketPtr Accept(SocketPtr socket, Address& ip_addr);
         /**
         * @brief Accepts new socket connection on the opened socket
         *
         * @param socket Opened Listen socket on which to accept the new connection
         * @param ip_addr [out]ip address of remote site
         * @param port [out]port number of remote site
         * @return The new opened socket
         */
         TCNET_API ReadWriteSocketPtr Accept(SocketPtr socket, Address& ip, PortNumber& port);
         /**
         * @short Create a socket server object
         */
         TCNET_API SocketServerPtr CreateSocketServer();
      }


      /**
      * @}
      */

   } // namespace net
} // namespace tc


#endif // _TCNET_FACTORY_H_
