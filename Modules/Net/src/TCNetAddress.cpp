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

#include "TCNetAddress.h"

#include "TCNetOS.h"
#include "TCOutput.h"
#include "TCNetBase.h"

#include <signal.h>
#include <setjmp.h>
#include <cstring>

#include "TCNewEnable.h"

typedef void (*TCSIG_PF)(int);

namespace tc
{
   namespace net
   {
#ifdef TCOS_WINDOWS
      static bool inet_aton(const char *cp, struct in_addr* pin)
      {
         unsigned long a = inet_addr(cp);
         if (a == (unsigned long)-1) return false;

         pin->s_addr = a;
         return true;
      }
#endif

#ifndef TCOS_WINDOWS
      static jmp_buf alarmEnv;
      static void onAlarm(int)
      {
         // jump back to setjmp.  this handles the race condition where we
         // set the alarm but gethostbyname() wouldn't have been called
         // until after the alarm goes off (resulting in an indeterminate
         // wait).
         longjmp(alarmEnv, 1);
      }
#endif

      Address::Address()
         :m_address(0)
         ,m_base(new imp::Base)
      {
         initAddress();
      }

      Address::Address(const std::string& name)
         :m_address(0)
         ,m_base(new imp::Base)
      {
         SetHostName(name);
      }

      Address::Address(const Address& _addr) 
         :m_address(0)
         ,m_base(new imp::Base)
      {
         SetAddress(_addr);
      }

      Address::Address(const InternetAddress& _addr)
         :m_address(0)
         ,m_base(new imp::Base)
      {
         SetAddress(_addr);
      }

      Address::Address(const SocketAddress& _addr)
         :m_address(0)
         ,m_base(new imp::Base)
      {
         SetAddress(_addr);
      }

      Address::Address(uint32 ip_address)
         :m_address(0)
         ,m_base(new imp::Base)
      {
         SetAddress(ip_address);
      }

      Address::~Address()
      {
         if (m_address)
         {
            delete m_address;
            m_address = 0;
         }

         delete m_base;
      }

      void Address::initAddress()
      {
         if (m_address == 0)
         {
            m_address = new InternetAddress;
         }

         std::memset(m_address, 0, sizeof(InternetAddress));
         m_address->s_addr = htonl(INADDR_ANY);
      }

      // methodes for setting new machine ore address
      void Address::SetHostName(const std::string& name)
      {
         setupInternetAdress(name);
      }

      void Address::SetAddress(const Address& _addr)
      {
         initAddress();
         *m_address = *_addr.m_address;
      }

      void Address::SetAddress(const InternetAddress& _addr)
      {
         initAddress();
         *m_address = _addr;
      }

      void Address::SetAddress(const SocketAddress& _addr)
      {
         initAddress();
         *m_address = _addr.sin_addr;
      }

      void Address::SetAddress(uint32 ip_address)
      {
         initAddress();
         m_address->s_addr = htonl(ip_address);
      }

      const InternetAddress& Address::GetInternetAddress() const
      {
         return *m_address;
      }

      std::string Address::GetDotNotation() const
      {
         return std::string(inet_ntoa(*m_address));
      }

      void Address::setupInternetAdress(const std::string &hnameIn)
      {
         initAddress();

         std::string hname = hnameIn;
         struct hostent* hent = 0;

         // local Address
         if (hname == "")
         {            
            char hostname[256];
            if (gethostname(hostname, sizeof(hostname)) >= 0)
               hname = hostname;
            else
            {
               TCERROR1("TCNET", "error setting up internet address of %s",
                  hname.c_str());
               return;
            }

         }

         // find local adress
         if (inet_aton(hname.c_str(), m_address))
            return;
         // non-local adress
         else
         {            
#ifndef TCOS_WINDOWS
            // set alarm to avoid waiting too long
            TCSIG_PF oldAlarm = signal(SIGALRM, TCSIG_PF(onAlarm));
            if (oldAlarm != SIG_ERR)
            {
               if (setjmp(alarmEnv) != 0)
               {
                  // alarm went off
                  TCERROR1("TCNET", "error setting up internet address of %s",
                     hname.c_str());
                  return;
               }

               // wait up to this many seconds
               alarm(3);
            }
#endif

            hent = gethostbyname(hname.c_str());

#ifndef TCOS_WINDOWS
            if (oldAlarm != SIG_ERR)
            {
               alarm(0);
               signal(SIGALRM, oldAlarm);
            }
#endif
         }

         if (!hent) 
         {
            TCERROR1("TCNET", "error setting up internet address of %s",
               hname.c_str());
            return;
         }

         std::memcpy(m_address, hent->h_addr_list[0], sizeof(InternetAddress));
      }

      std::string Address::GetHostName() const
      {
#ifndef TCOS_WINDOWS
         // set alarm to avoid waiting too long
         TCSIG_PF oldAlarm = signal(SIGALRM, TCSIG_PF(onAlarm));
         if (oldAlarm != SIG_ERR)
         {
            // alarm went off
            if (setjmp(alarmEnv) != 0) return std::string(inet_ntoa(*m_address));

            // wait up to this many seconds
            alarm(3);
         }
#endif

         sint32 addrLen = sizeof(InternetAddress);
         struct hostent* hent = gethostbyaddr((char*)m_address, addrLen, AF_INET);

#ifndef TCOS_WINDOWS
         if (oldAlarm != SIG_ERR)
         {
            alarm(0);
            signal(SIGALRM, oldAlarm);
         }
#endif

         // can't lookup name -- return in standard dot notation
         if (!hent) return GetDotNotation();

         return std::string(hent->h_name);
      }


      // operators for setting new machine or adress
      Address& Address::operator=(const std::string& _addr)
      {
         SetHostName(_addr);
         return *this;
      }

      Address& Address::operator=(const InternetAddress& _addr)
      {
         SetAddress(_addr);
         return *this;
      }

      Address& Address::operator=(const SocketAddress& _addr)
      {
         SetAddress(_addr);
         return *this;
      }

      Address& Address::operator=(const Address& _addr)
      {
         SetAddress(_addr);
         return *this;
      }

      Address& Address::operator=(uint32 _addr)
      {
         SetAddress(_addr);
         return *this;
      }

      // operators for comparing
      bool Address::operator==(const Address& address) const
      {
         return m_address->s_addr == address.m_address->s_addr;
      }

      bool Address::operator!=(const Address& address) const
      {
         return m_address->s_addr != address.m_address->s_addr;
      }

      Address Address::GetLocalAddress()
      {
         static Address s_local_address("");
         return s_local_address;
      }

      Address Address::GetAnyAddress()
      {
         Address address(INADDR_ANY);
         return address;
      }

      Address Address::GetBroadcastAddress()
      {
         Address address(INADDR_BROADCAST);
         return address;
      }

      Address Address::GetLoopBackAddress()
      {
         Address address(INADDR_LOOPBACK);
         return address;
      }
   }
}
