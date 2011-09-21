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

#ifndef _TCNET_OS_H_
#define _TCNET_OS_H_

#ifdef TCOS_WINDOWS

#include <windows.h>
#include <winsock2.h>

#else
 
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
 
#endif

#include <fcntl.h>
#include <errno.h>

// define SOCKET_ERROR with -1 if notdefiend on some systems
#ifndef SOCKET_ERROR
#  define SOCKET_ERROR (-1)
#endif

// define INADDR_NONE with -1 if notdefiend on some systems
#ifndef INADDR_NONE
#  define INADDR_NONE (-1)
#endif

// define INVALID_SOCKET with ~0 if notdefiend on some systems
#ifndef INVALID_SOCKET
#  define INVALID_SOCKET (~0)
#endif

namespace TC
{
namespace Net
{
namespace Impl
{
   enum
   {
      /** constant value when a error with a socket happened */
      socket_error = SOCKET_ERROR,
      /** constant value for an undefined address */
      inaddr_none = INADDR_NONE,
      /** constant value for an invalid socket id*/
      invalid_socket = INVALID_SOCKET
   };

#if TCOS_LINUX || TCOS_IBM
   typedef socklen_t AddrLength;
#else
   typedef sint32  AddrLength;
#endif

} // namespace Impl
} // namespace Net
} // namespace TC

#endif // _TCNET_OS_H_
