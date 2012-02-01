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

#ifndef _TCNET_READ_WRITE_SOCKET_H
#define _TCNET_READ_WRITE_SOCKET_H

#include "TCNetSocket.h"
#include "TCTime.h"
#include "TCNetAddress.h"

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
      class ReadWriteSocket: public Socket
      {
      public:
         /**
         * @brief Reads data from socket
         * @param buffer [out] pointer to a buffer where the data should be written to.
         * @param size [in]  size of \c buffer
         * @return number of bytes received
         */
         virtual uint64 ReadBytes(void* buffer, uint64 size) = 0;

         /**
         * @brief Reads data with length 'size' from socket with timeout
         * @param buffer [out] pointer to a buffer where the data should be written to.
         * @param size [in]  size of \c buffer
         * @param timeout [in]  timeout in milliseconds
         * @return true  -> all data received in given time
         *         false -> by timeout or error or not all data received
         */
         virtual uint64 ReadBytes(void* buffer, uint64 size, const Time& timeout) = 0;

         /**
         * @brief  Writes bytes to the socket
         * @param buffer [out] data which should be sent
         * @param size [in] size of \c buffer
         */
         virtual uint64 WriteBytes(const void* buffer, uint64 size) = 0;

         virtual ~ReadWriteSocket() {}
      };
      /** Type for an read write socket pointer object */
      typedef SharedPtr<ReadWriteSocket> ReadWriteSocketPtr;

      /**
      * @}
      */

   } // namespace net
} // namespace tc

#endif // _TCNET_READ_WRITE_SOCKET_H
