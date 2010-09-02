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

         MutexPthread::MutexPthread(bool locked)
         {
            pthread_mutexattr_t attr;
            ::pthread_mutexattr_init(&attr);
            ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            ::pthread_mutex_init(&m_mutex, &attr);
            ::pthread_mutexattr_destroy(&attr);

            // lock it if it should be initially locked
            if (locked) Lock();
         }

         MutexPthread::~MutexPthread()
         {
            ::pthread_mutex_destroy(&m_mutex);
         }

         bool MutexPthread::Lock()
         {
            return ::pthread_mutex_lock(&m_mutex) == 0;
         }

         bool MutexPthread::TryLock()
         {
            return ::pthread_mutex_trylock(&m_mutex) == 0;
         }

         bool MutexPthread::TryLock(const Time& timeout)
         {
            Time time = Time::Now() + timeout;
            timespec t = {time.Seconds(), time.NanoSeconds()};
            return ::pthread_mutex_timedlock(&m_mutex, &t) == 0;
         }

         bool MutexPthread::UnLock()
         {
            return ::pthread_mutex_unlock(&m_mutex) == 0;
         }

         MutexSharedPthread::MutexSharedPthread(const std::string& shared_name, bool locked)
         {
            m_semaphore = SemaphorePtr(new SemaphorePthread(shared_name, 1));
            // lock it if it should be initially locked
            if (locked) m_semaphore->Wait();
         }

         MutexSharedPthread::~MutexSharedPthread()
         {
            m_semaphore = SemaphorePtr();
         }

         bool MutexSharedPthread::Lock()
         {
            return m_semaphore->Wait();
         }

         bool MutexSharedPthread::TryLock()
         {
            return m_semaphore->Try();
         }

         bool MutexSharedPthread::TryLock(const Time& timeout)
         {
            return m_semaphore->TryWait(timeout);
         }

         bool MutexSharedPthread::UnLock()
         {
            return m_semaphore->Post();
         }

      } // namespace Impl
   } // namespace MT
} // namespace TC

#endif // TCOS_POSIX
