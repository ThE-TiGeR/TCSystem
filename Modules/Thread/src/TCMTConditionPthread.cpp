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
#include "TCDefines.h"

#ifdef TCOS_POSIX

#include "TCMTConditionPthread.h"

#include "TCMTOS.h"
#include "TCTime.h"
#include "TCMTMutexPthread.h"

#include <errno.h>

#include "TCNewEnable.h"

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {

         ConditionPthread::ConditionPthread()
            :m_mutex(new MutexPthread)
         {
            m_mutex->Init(false);
            ::pthread_cond_init(&m_condition, 0);
         }

         ConditionPthread::ConditionPthread(MutexPtr mutex)
            :m_mutex(SharedPtr<MutexPthread>::DynamicCast(mutex))
         {
            ::pthread_cond_init(&m_condition, 0);
         }

         ConditionPthread::~ConditionPthread()
         {
            ::pthread_cond_destroy(&m_condition);
         }

         MutexPtr ConditionPthread::GetMutex()
         {
            return m_mutex;
         }

         bool ConditionPthread::Wait()
         {
            return ::pthread_cond_wait(&m_condition, &m_mutex->m_mutex) == 0;
         }

         bool ConditionPthread::TryWait(const Time& timeout)
         {
            Time time = Time::Now() + timeout;
            timespec t = {time.Seconds(), time.NanoSeconds()};
            int32_t rc;

            do
            {
               rc = pthread_cond_timedwait(&m_condition, &m_mutex->m_mutex, &t);
               switch (rc)
               {
               case ETIMEDOUT:   // timed out
                  return false;

               case EINTR:
                  // Some versions of unix produces this errno when the wait was
                  // interrupted by a unix signal or fork.
                  // Some versions of the glibc 2.0.x produces this errno when the 
                  // program is debugged under gdb. Straightly speaking this is non-posix
                  // compliant. We catch this here to make debugging possible.
                  break;
               }
            }while (rc != 0);

            return true;
         }

         bool ConditionPthread::Signal()
         {
            return ::pthread_cond_signal(&m_condition) == 0;
         }

         bool ConditionPthread::Broadcast()
         {
            return ::pthread_cond_broadcast(&m_condition) == 0;
         }

      } // namespace imp
   } // namespace multi_threading
} // namespace tc

#endif
