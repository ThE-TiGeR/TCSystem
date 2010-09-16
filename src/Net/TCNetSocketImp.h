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

#ifndef _TCNET_SOCKET_IMP_H
#define _TCNET_SOCKET_IMP_H

#include "TCNetReadWriteSocket.h"
#include "TCNetBroadCastSocket.h"
#include "TCNetBase.h"

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
         * class for makeing comminication over sockets
         */
         class TC_DLL_LOCAL SocketImp: virtual public ReadWriteSocket, virtual public BroadcastReadSocket, virtual public BroadcastWriteSocket, protected Base
         {
         public:
            explicit SocketImp(SocketId s);
            virtual ~SocketImp();

            virtual bool IsOpened() const;
            virtual bool Close();
            virtual SocketId GetSocket();

            virtual uint64 ReadBytes(void* buffer, uint64 size);
            virtual uint64 ReadBytes(void* buffer, uint64 size, const Time& timeout);
            virtual uint64 ReadBytesFrom(void* buffer, uint64 size, Address& ip);
            virtual uint64 WriteBytes(const void* buffer, uint64 size);
            virtual uint64 WriteBytesTo(const void* buffer, uint64 size, const Address& ip, PortNumber port);

         private:
            SocketId m_socket;
         };

         /**
         * @}
         */

      } // namespace Impl
   } // namespace Net
} // namespace TC

#endif // _TCNET_SOCKET_H
