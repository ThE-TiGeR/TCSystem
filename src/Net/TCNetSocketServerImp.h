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
//  $Id: TCNetSocketServerImp.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TCNET_SOCKET_SERVER_IMP_H_
#define _TCNET_SOCKET_SERVER_IMP_H_

#include "TCMTThread.h"
#include "TCNetFactory.h"
#include "TCNetSocketServer.h"

#include <vector>

namespace TC
{
namespace Net
{
namespace Impl
{

   /**
    * @addtogroup TC_NET_IMP
    * @{
    */

   /**
   * class for running as net server which can communicate with any number of clients
   * on a specified port. Connections can be made with objects based on TCNetClient
   */
   class TC_DLL_LOCAL SocketServerImp: public SocketServer
   {
   protected:
      /**
       * It will listen on all added socket objects and call their receivers
       */
      SocketServerImp();
   public:
      /**
      * deletes the object and stops the server
      * @see TC::Net::SocketServer::Stop()
      */
      virtual ~SocketServerImp();

      /**
      * Starts the server by setting the stop variable to false
      * @param wait_stopped Wait until the server has stopped
      */
      virtual void Start(bool wait_started);
      /**
       * stops the server by setting the stop variable to false
       * @param wait_stopped Wait until the server has stopped
       */
      virtual void Stop(bool wait_stopped);

      /**
      * @brief Add a socket to the server for selection of new data
      * @param socket The socket to add
      * @param true is success
      */
      bool AddSocket(SocketPtr socket,
                     DataReceiverPtr data_receiver);
      /**
      * @brief Remove a socket to the server for selection of new data
      * @param socket The socket to remove
      * @param true is success
      */
      bool RemoveSocket(SocketPtr socket);

   protected:
      /**
      * run the server and accept incoming data   
      * @return 0 is ok other values are error
      */
      virtual bool Run();
      /**
      * wait for incoming data and accept
      * returning depends on then returnType
      */
      virtual bool Accept();
      /**
       * @brief Methode handles messages received by this thread
       * Method can be overloaded to receive his own messages
       * @return false if the server should be stopped
       */
      virtual bool HandleMessage(MT::MessagePtr message);

   private:
      /** socket id`s of all connected clients */
      std::vector< SocketPtr > m_sockets;
      /** receiver objects for all sockets */
      std::vector< DataReceiverPtr > m_receivers;
      /** pointer to the thread object of this server */
      MT::ThreadPtr m_server_thread;

      enum
      {
         SELECT_TIMEOUT = 300 ///< Timeout in milliseconds for the select method
      };

      Time m_current_timeout;
      bool m_running;

      friend SocketServerPtr Factory::CreateSocketServer();
      friend class ThreadObject;
   };

   /**
    * @}
    */

} // namespace Impl
} // namespace Net
} // namespace TC


#endif // _TCNET_SOCKET_SERVER_H_
