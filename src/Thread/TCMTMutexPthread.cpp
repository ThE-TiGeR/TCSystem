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
#ifdef TCOS_POSIX

#include "TCMTMutexPthread.h"

#include "TCMTThreadPthread.h"
#include "TCTime.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace MT
   {
      namespace Impl
      {

         MutexPthread::MutexPthread()
         {
         }

         bool MutexPthread::Init(bool locked)
         {
            ::pthread_mutexattr_t attr;
            if (::pthread_mutexattr_init(&attr) != 0)
            {
               ::perror("::pthread_mutexattr_init failed");
               return false;
            }

            if (::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
            {
               ::perror("::pthread_mutexattr_settype failed");
               ::pthread_mutexattr_destroy(&attr);
               return false;
            }

            if (::pthread_mutex_init(&m_mutex, &attr) != 0)
            {
               ::perror("::pthread_mutex_init failed");
               ::pthread_mutexattr_destroy(&attr);
               return false;
            }

            ::pthread_mutexattr_destroy(&attr);

            if (locked)
            {
               return Lock();
            }

            return true;
         }

         MutexPthread::~MutexPthread()
         {
            ::pthread_mutex_destroy(&m_mutex);
         }

         bool MutexPthread::Lock()
         {
            if (::pthread_mutex_lock(&m_mutex) != 0)
            {
               ::perror("::pthread_mutex_lock failed");
               return false;
            }

            return true;
         }

         bool MutexPthread::TryLock()
         {
            if (::pthread_mutex_trylock(&m_mutex) != 0)
            {
               if (errno == EBUSY)
               {
                  return false;
               }

               ::perror("::pthread_mutex_trylock failed");
               return false;        
            }

            return true;
         }

         bool MutexPthread::TryLock(const Time& timeout)
         {
            Time time = Time::Now() + timeout;
            timespec t = {time.Seconds(), time.NanoSeconds()};

            if (::pthread_mutex_timedlock(&m_mutex, &t) != 0)
            {
               if (errno == ETIMEDOUT)
               {
                  return false;
               }

               ::perror("::pthread_mutex_timedlock failed");
               return false;        
            }

            return true;
         }

         bool MutexPthread::UnLock()
         {
            if (::pthread_mutex_unlock(&m_mutex) != 0)
            {
               ::perror("::pthread_mutex_lock failed");
               return false;
            }

            return true;
         }

         MutexSharedPthread::MutexSharedPthread() 
            :m_nesting_level(0),
            m_owner_process(0),
            m_semaphore()
         {
         }

         MutexSharedPthread::~MutexSharedPthread()
         {
         }

         bool MutexSharedPthread::Init(const std::string& shared_name, bool locked, Factory::CreationMode mode)
         {
            m_semaphore = MT::Factory::CreateSemaphore(shared_name, locked ? 0 : 1, mode);
            if (locked)
            {
               SetOwnerShip();
            }

            return m_semaphore != 0;
         }

         bool MutexSharedPthread::Lock()
         {
            if (TryAndGetIfOwner())
            {
               return true;
            }

            if (m_semaphore->Wait())
            {
               SetOwnerShip();
               return true;
            }

            return false;
         }

         bool MutexSharedPthread::TryLock()
         {
            if (TryAndGetIfOwner())
            {
               return true;
            }

            if (m_semaphore->Try())
            {
               SetOwnerShip();
               return true;
            }

            return false;
         }

         bool MutexSharedPthread::TryLock(const Time& timeout)
         {
            if (TryAndGetIfOwner())
            {
               return true;
            }

            if (m_semaphore->TryWait(timeout))
            {
               SetOwnerShip();
               return true;
            }

            return false;
         }

         bool MutexSharedPthread::UnLock()
         {
            if (UnGetIfOwner())
            {
               return m_semaphore->Post();
            }
            return true;
         }

         bool MutexSharedPthread::TryAndGetIfOwner()
         {
            if (::pthread_equal(::pthread_self(), m_owner_process))
            {
               m_nesting_level++;
               return true;
            }

            return false;
         }

         void MutexSharedPthread::SetOwnerShip()
         {
            m_owner_process = ::pthread_self();
            m_nesting_level = 1;
         }

         bool MutexSharedPthread::UnGetIfOwner()
         {
            if (::pthread_equal(::pthread_self(), m_owner_process) == 0)
            {
               return false;
            }
            if (m_nesting_level == 0)
            {
               return false;
            }

            m_nesting_level--;
            if (m_nesting_level == 0)
            {
               m_owner_process = 0;
            }

            return m_nesting_level == 0;
         }

      } // namespace Impl
   } // namespace MT
} // namespace TC

#endif // TCOS_POSIX
