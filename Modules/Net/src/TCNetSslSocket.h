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
//  $Id: TCNetSslSocket.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TCNET_SSL_SOCKET_H
#define _TCNET_SSL_SOCKET_H

#ifndef TCNET_NO_OPENSSL

#include "TCNetReadWriteSocket.h"
#include "TCNetBase.h"

typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;

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

         /**
         * class for makeing comminication over sockets with ssl using OpenSSL lib
         */
         class TC_DLL_LOCAL SslSocket: public ReadWriteSocket, protected Base
         {
         public:
            /**
            * Constructs an SslSocket object and makes default initialisation
            */
            SslSocket(SocketPtr socket);
            /** Additional it connects with ssl */
            virtual bool Connect();

            virtual bool IsOpened() const;
            virtual bool Close();
            virtual SocketId GetSocket();

            virtual uint64_t ReadBytes(void* buffer, uint64_t size);
            virtual uint64_t ReadBytes(void* buffer, uint64_t size, const Time& timeout);
            virtual uint64_t WriteBytes(const void* buffer, uint64_t size);

            virtual ~SslSocket();
         private:
            SocketPtr m_socket;
            SSL_CTX* m_ssl_ctx;
            SSL*     m_ssl;
         };

         /**
         * @}
         */

      } // namespace imp
   } // namespace net
} // namespace tc

#endif // TCNET_NO_OPENSSL

#endif // _TCNET_SSL_SOCKET_H
