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
//  $Id: TCNetSocket.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TCNET_SOCKET_H
#define _TCNET_SOCKET_H

#include "TCNetTypes.h"

namespace tc
{
   namespace net
   {
      /**
      * @addtogroup TC_NET
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of tc::net::Socket
      * @author Thomas Goessler
      */
      class Socket
      {
      public:
         /** @return true if the socket descriptor is valid */
         virtual bool IsOpened() const = 0;

         /**
         * Close the socket if open
         * @return true if unsuccessfully closed or already closed
         */
         virtual bool Close() = 0;

         /**
         * @brief Retrieves the socket descriptor id
         * @return socket descriptor id
         */
         virtual SocketId GetSocket() = 0;

         virtual ~Socket() {}
      };
      /** Type for an socket pointer object */
      typedef SharedPtr<Socket> SocketPtr;

      /**
      * @}
      */

   } // namespace net
} // namespace tc

#endif // _TCNET_SOCKET_H
