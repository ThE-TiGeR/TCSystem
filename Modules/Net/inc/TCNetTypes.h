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

#ifndef _TC_NET_TYPES_H_
#define _TC_NET_TYPES_H_

#include "TCSharedPtr.h"

struct in_addr;
struct sockaddr_in;

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
      * @brief This file provides the definition of data types needed for tc::net classes
      * @author Thomas Goessler
      */

      /** typedef for port numbers */
      typedef uint16_t PortNumber;

#ifdef TCOS_WINDOWS
      /** socket type */
#if defined(_WIN64)
      typedef unsigned __int64 SocketId;
#else
      typedef unsigned int SocketId;
#endif
#else
      /** socket type */
      typedef signed int SocketId;
#endif

      /** structure of the socket address */
      typedef struct sockaddr_in SocketAddress;
      /** structure of the internet address */
      typedef struct in_addr InternetAddress;

      /**
      * @}
      */

   } // namespace net
} // namespace tc


#endif // _TC_NET_TYPES_H_
