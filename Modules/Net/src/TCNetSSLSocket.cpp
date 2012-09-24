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

#ifndef TCNET_NO_OPENSSL

#include "TCNetSslSocket.h"

#include "TCNetOS.h"
#include "TCOutput.h"

#include <openssl/ssl.h>

#include "TCNewEnable.h"

namespace tc
{
   namespace net
   {
      namespace imp
      {

         SslSocket::SslSocket(SocketPtr socket)
            :m_socket(socket)
            ,m_ssl_ctx(0)
            ,m_ssl(0)
         {
            TCTRACES("TCNET", 10, "");

            SSLeay_add_ssl_algorithms();
            const SSL_METHOD* meth = SSLv3_client_method();
            SSL_load_error_strings();
            m_ssl_ctx = SSL_CTX_new(meth);

         }

         SslSocket::~SslSocket()
         {
            TCTRACES("TCNET", 10, "");

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
            TCTRACES("TCNET", 20, "");

            if (m_ssl)
            {
               SSL_shutdown(m_ssl);
               SSL_free(m_ssl);
               m_ssl = 0;
            }

            return m_socket->Close();
         }

         uint64_t SslSocket::WriteBytes(const void* buffer_in, uint64_t size)
         {
            TCTRACES("TCNET", 300, "(" << uint64_t(buffer_in) << ", " << size);

            const char* buffer = static_cast<const char*>(buffer_in);
            uint64_t nbytes_total = 0;
            while (nbytes_total < size)
            {
                int nbytes = SSL_write(m_ssl, buffer, int(size));
                if (nbytes == -1)
                {
                   return 0;
                }

                nbytes_total += nbytes;
                buffer += nbytes;
            }

            return nbytes_total;
         }

         uint64_t SslSocket::ReadBytes(void* buffer, uint64_t size, const Time& timeout)
         {
            TCTRACES("TCNET", 300, "(" << uint64_t(buffer) << ", " << size);

            uint64_t read_bytes = 0;

            Time start_time(Time::NowMonotonic());
            do
            {
               int nbytes = SSL_read(m_ssl, buffer, int(size - read_bytes));
               if (nbytes < 0)
               {
                  int32_t error_code = SSL_get_error(m_ssl, nbytes);
                  TCERRORS("TCNET", "Error reading from socket error=" << error_code);
                  Close();
                  break;
               }
               else if (nbytes == 0)
               {
                  //          int32_t error_code = SSL_get_error(m_ssl, len);
                  //          TCWARNING1("TCNET", "Other side closed socket connection error=%d",
                  //             error_code);
                  //          Close();
                  break;
               }
               else
               {
                  read_bytes += nbytes;
                  buffer = static_cast<char*>(buffer) + nbytes;
               }
            } while ((size > read_bytes) && Time::SinceMonotonic(start_time) < timeout);

            return read_bytes;
         }

         uint64_t SslSocket::ReadBytes(void* buffer, uint64_t size)
         {
            return ReadBytes(buffer, size, Time::Zero());
         }

         bool SslSocket::IsOpened() const
         {
            return m_socket->IsOpened();
         }

         tc::net::SocketId SslSocket::GetSocket()
         {
            return m_socket->GetSocket();
         }
      }
   }
}

#endif // TCNET_NO_OPENSSL
