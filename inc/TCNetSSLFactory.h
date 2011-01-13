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
//  $Id: TCNetFactory.h 1000 2010-07-14 23:27:05Z the_____tiger $
//*******************************************************************************

#ifndef _TCNET_SSL_FACTORY_H_
#define _TCNET_SSL_FACTORY_H_

#include "TCNetFactory.h"
#include "TCNetAddress.h"
#include "TCNetReadWriteSocket.h"
#include "TCNetSocketServer.h"

/**
* @addtogroup TC_NET
* @{
*/

/**
* @file
* @brief This file provides the definition of TC::Net::SSFactory
* @author Thomas Goessler
*/

/**
* @}
*/

namespace TC
{
   namespace Net
   {
      /**
      * @brief Factory for creating TC::Net SSL objects
      */
      namespace SSLFactory
      {
         /**
         * @addtogroup TC_NET
         * @{
         */

         /**
         * @brief Connects to specified server and returns the socket
         *
         * @param ip_addr Network name or ip address of the server to which to connect
         * @param port   Port on which to connect to the server
         * @param protocol @see Protocol
         */
         TCNET_API ReadWriteSocketPtr Connect(const Address& ip_addr, PortNumber port, Protocol protocol);

         /**
         * @}
         */
      }



   } // namespace Net
} // namespace TC


#endif // _TCNET_SSL_FACTORY_H_
