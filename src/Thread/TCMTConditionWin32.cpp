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
#include "TCDefines.h"

#if TCOS_WINDOWS

#include "TCMTConditionWin32.h"

#include "TCMTOS.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace MT
   {
      namespace Impl
      {

         class ConditionWin32::ConditionData
         {
         public:
            uint32 waiters_count;
            // Count of the number of waiters.

            CRITICAL_SECTION waiters_count_lock;
            // Serialize access to <waiters_count>.

            enum 
            {
               SIGNAL = 0,
               BROADCAST = 1,
               MAX_EVENTS = 2
            };

            HANDLE events[MAX_EVENTS];
            // Signal and broadcast event HANDLEs.
         };

         ConditionWin32::ConditionWin32()
            :m_handle(0),
            m_mutex(new MutexWin32)
         {
            Init();
         }

         ConditionWin32::ConditionWin32(MutexPtr mutex)
            :m_handle(0),
            m_mutex(SharedPtr<MutexWin32>::DynamicCast(mutex))
         {
            Init();
         }

         void ConditionWin32::Init()
         {
            if (!m_mutex->Init(false))
            {
               return;
            }

            m_handle = new ConditionData;

            // Initialize the count to 0.
            m_handle->waiters_count = 0;
            ::InitializeCriticalSection(&m_handle->waiters_count_lock);

            // Create an auto-reset event.
            m_handle->events[ConditionData::SIGNAL] = ::CreateEventA(0,  // no security
               FALSE, // auto-reset event
               FALSE, // non-signaled initially
               0); // unnamed

            // Create a manual-reset event.
            m_handle->events[ConditionData::BROADCAST] = ::CreateEventA(0,  // no security
               TRUE,  // manual-reset
               FALSE, // non-signaled initially
               0); // unnamed
         }
         ConditionWin32::~ConditionWin32()
         {
            ::CloseHandle(m_handle->events[ConditionData::SIGNAL]);
            ::CloseHandle(m_handle->events[ConditionData::BROADCAST]);

            ::DeleteCriticalSection(&m_handle->waiters_count_lock);

            delete m_handle;
            m_handle = 0;
         }

         MutexPtr ConditionWin32::GetMutex()
         {
            return m_mutex;
         }

         bool ConditionWin32::Wait()
         {
            return TryWait(Time::Infinite());
         }

         bool ConditionWin32::TryWait(const Time& timeout)
         {
            // Avoid race conditions.
            ::EnterCriticalSection (&m_handle->waiters_count_lock);
            m_handle->waiters_count++;
            ::LeaveCriticalSection (&m_handle->waiters_count_lock);

            // It's ok to release the <external_mutex> here since Win32
            // manual-reset events maintain state when used with
            // <SetEvent>.  This avoids the "lost wakeup" bug...
            ::ReleaseMutex(m_mutex->m_handle);

            // Wait for either event to become signaled due to <pthread_cond_signal>
            // being called or <pthread_cond_broadcast> being called.
            DWORD result;
            if (timeout == Time::Infinite())
            {
               result = ::WaitForMultipleObjects (2, m_handle->events, FALSE, INFINITE);
            }
            else
            {
               result = ::WaitForMultipleObjects (2, m_handle->events, FALSE, static_cast<DWORD>(timeout.ToMilliSeconds()));
            }

            ::EnterCriticalSection (&m_handle->waiters_count_lock);
            m_handle->waiters_count--;
            bool last_waiter =  (result == WAIT_OBJECT_0 + ConditionData::BROADCAST) &&
               (m_handle->waiters_count == 0);
            ::LeaveCriticalSection (&m_handle->waiters_count_lock);

            // Some thread called <pthread_cond_broadcast>.
            if (last_waiter)
            {
               // We're the last waiter to be notified or to stop waiting, so
               // reset the manual event. 
               ::ResetEvent (m_handle->events[ConditionData::BROADCAST]); 
            }

            // Reacquire the <external_mutex>.
            ::WaitForSingleObject(m_mutex->m_handle, INFINITE);

            return result!=WAIT_TIMEOUT; 
         }

         //Calling SetEvent on a manual-reset event sets the m_handle->events[BROADCAST] event to the signaled state. This releases all threads until the event is manually reset in the pthread_cond_wait function above. 
         //The pthread_cond_signal function wakes up a thread waiting on the condition variable cv: 
         bool ConditionWin32::Signal()
         {
            // Avoid race conditions.
            ::EnterCriticalSection(&m_handle->waiters_count_lock);
            int have_waiters = m_handle->waiters_count > 0;
            ::LeaveCriticalSection (&m_handle->waiters_count_lock);

            if (have_waiters)
            {
               ::SetEvent(m_handle->events[ConditionData::SIGNAL]);
            }

            return true;
         }

         //The release of the external_mutex and the subsequent wait for either of the events to become signaled is still non-atomic. This implementation avoids the lost wakeup bug, however, by relying on the ``stickiness'' of the manual-reset event, the use of SetEvent rather than PulseEvent, and the waiters_count count. Note how the last waiter thread in pthread_cond_wait resets the broadcast manual-reset event to non-signaled before exiting the function. This event is signaled in pthread_cond_broadcast, which wakes up all threads waiting on the condition variable cv, as follows: 
         bool ConditionWin32::Broadcast()
         {
            // Avoid race conditions.
            ::EnterCriticalSection(&m_handle->waiters_count_lock);
            bool have_waiters = m_handle->waiters_count > 0;
            ::LeaveCriticalSection(&m_handle->waiters_count_lock);

            if (have_waiters)
            {
               ::SetEvent (m_handle->events[ConditionData::BROADCAST]);
            }

            return true;
         }

      } // namespace Impl
   } // namespace MT
} // namespace TC

#endif

