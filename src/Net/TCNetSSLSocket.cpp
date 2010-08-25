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
//  $Id: TCNetSSLSocket.cpp 1001 2010-07-18 22:10:55Z the_____tiger $
//*******************************************************************************

#ifndef TCNET_NO_OPENSSL

#include "TCNetSslSocket.h"

#include "TCNetOS.h"
#include "TCOutput.h"

#include <openssl/ssl.h>

#include "TCNewEnable.h"

namespace TC
{
   namespace Net
   {
      namespace Impl
      {

         SslSocket::SslSocket(SocketPtr socket)
            :m_socket(socket)
            ,m_ssl_ctx(0)
            ,m_ssl(0)
         {
            TCTRACEF("TCNET", 10);

            SSLeay_add_ssl_algorithms();
            SSL_METHOD* meth = SSLv2_client_method();
            SSL_load_error_strings();
            m_ssl_ctx = SSL_CTX_new(meth);

         }

         SslSocket::~SslSocket()
         {
            TCTRACEF("TCNET", 10);

            Close();
         }

         bool SslSocket::Connect()
         {
            /* ----------------------------------------------- */
            /* Now we have TCP conncetion. Start SSL negotiation. */
            m_ssl = SSL_new(m_ssl_ctx);
            if (!m_ssl)
            {
               return false;
            }
            SSL_set_fd(m_ssl, static_cast<int>(m_socket->GetSocket()));
            if (SSL_connect(m_ssl) == -1)
            {
               return false;
            }

            // Following two steps are optional and not required for
            // data exchange to be successful.

            // Get the cipher - opt
            //printf ("SSL connection using %s\n", SSL_get_cipher(m_ssl));

            // Get server's certificate (note: beware of dynamic allocation) - opt

            X509* server_cert = SSL_get_peer_certificate(m_ssl);
            //printf ("Server certificate:\n");

            char* str = X509_NAME_oneline (X509_get_subject_name(server_cert),0,0);
            //printf ("\t subject: %s\n", str);
            OPENSSL_free (str);

            str = X509_NAME_oneline (X509_get_issuer_name  (server_cert),0,0);
            //printf ("\t issuer: %s\n", str);
            OPENSSL_free (str);

            // We could do all sorts of certificate verification stuff here before
            // deallocating the certificate.

            X509_free (server_cert);

            return true;
         }

         bool SslSocket::Close()
         {
            TCTRACEF("TCNET", 20);

            if (m_ssl)
            {
               SSL_shutdown(m_ssl);
               SSL_free(m_ssl);
               m_ssl = 0;
            }

            return m_socket->Close();
         }

         uint32 SslSocket::WriteBytes(const void* buffer, uint32 size)
         {
            TCTRACE2("TCNET", 300, "(%x, %d)", buffer, size);

            sint32 len = SSL_write(m_ssl, buffer, size);
            if (len == -1)
            {
               return 0;
            }
            return len;
         }

         uint32 SslSocket::ReadBytes(void* buffer, uint32 size, const Time& timeout)
         {
            TCTRACE2("TCNET", 300, "(%x, %d)", buffer, size);

            uint32 read_bytes = 0;

            Time start_time(Time::NowMonotonic());
            do
            {
               sint32 len = SSL_read(m_ssl, buffer, size - read_bytes);
               if (len < 0)
               {
                  sint32 error_code = SSL_get_error(m_ssl, len);
                  TCERROR1("TCNET", "Error reading from socket error=%d",
                     error_code);
                  Close();
                  break;
               }
               else if (len == 0)
               {
                  //          sint32 error_code = SSL_get_error(m_ssl, len);
                  //          TCWARNING1("TCNET", "Other side closed socket connection error=%d",
                  //             error_code);
                  //          Close();
                  break;
               }
               else
               {
                  read_bytes += static_cast<uint32>(len);
                  buffer = static_cast<char*>(buffer) + len;
               }
            } while ((size > read_bytes) && Time::SinceMonotonic(start_time) < timeout);

            return read_bytes;
         }

         uint32 SslSocket::ReadBytes(void* buffer, uint32 size)
         {
            return ReadBytes(buffer, size, Time::Zero());
         }

         bool SslSocket::IsOpened() const
         {
            return m_socket->IsOpened();
         }

         TC::Net::SocketId SslSocket::GetSocket()
         {
            return m_socket->GetSocket();
         }

         uint32 SslSocket::ReadBytesFrom(void* /*buffer*/, uint32 /*size*/, Address& /*ip*/)
         {
            return 0;
         }

         uint32 SslSocket::WriteBytesTo(const void* /*buffer*/, uint32 /*size*/, const Address& /*ip*/, PortNumber /*port*/)
         {
            return 0;
         }
      }
   }
}

#endif // TCNET_NO_OPENSSL
