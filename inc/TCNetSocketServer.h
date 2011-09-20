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
//  $Id$
//*******************************************************************************

#ifndef _TCNET_SOCKET_SERVER_H_
#define _TCNET_SOCKET_SERVER_H_

#include "TCNetReadWriteSocket.h"

namespace TC
{
namespace Net
{
   /**
    * @addtogroup TC_NET
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TC::Net::SocketServer
    * @author Thomas Goessler
    */

   /**
    * class for running as net server which can communicate with any number of clients
    * on a specified port. Connections can be made with objects based on TCNetClient
    */
   class SocketServer: protected NonCopyAble
   {
   public:
      /** @brief Interface for retrieving incoming data on an socked */
      class DataReceiver: protected NonCopyAble
      {
      public:
         virtual ~DataReceiver() {}
         virtual bool OnNewData() = 0;
      };

      /** typedef for a data receiver pointer */
      typedef SharedPtr<DataReceiver> DataReceiverPtr;

   public:
      /**
      * deletes the object and stops the server
      * @see TC::Net::SocketServer::Stop()
      */
      virtual ~SocketServer() {}

      /**
      * Starts the server by setting the stop variable to false
      * @param wait_started Wait until the server has started
      */
      virtual void Start(bool wait_started) = 0;
      /**
       * stops the server by setting the stop variable to false
       * @param wait_stopped Wait until the server has stopped
       */
      virtual void Stop(bool wait_stopped) = 0;

      /**
      * @brief Add a socket to the server for selection of new data
      * @param socket The socket to add on which to listen for new data
      * @param data_receiver The data receiver which is informed if there is new incoming data
      * @return true is success
      */
      virtual bool AddSocket(SocketPtr socket, DataReceiverPtr data_receiver) = 0;
      /**
      * @brief Remove a socket to the server for selection of new data
      * @param socket The socket to remove
      * @return true is success
      */
      virtual bool RemoveSocket(SocketPtr socket) = 0;
   };

   /** Type for an socket server pointer object */
   typedef SharedPtr<SocketServer> SocketServerPtr;

   /**
    * @}
    */

} // namespace Net
} // namespace TC


#endif // _TCNET_SOCKET_SERVER_H_
