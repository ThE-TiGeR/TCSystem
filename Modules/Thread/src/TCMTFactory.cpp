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
//  $Id: TCMTFactory.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCMTFactory.h"

#include "TCMTOS.h"

#ifdef TCOS_WINDOWS
#  include "TCMTMutexWin32.h"
#  include "TCMTEventWin32.h"
#  include "TCMTSemaphoreWin32.h"
#  include "TCMTThreadWin32.h"
#  include "TCMTConditionWin32.h"
#else
#  include "TCMTMutexPthread.h"
#  include "TCMTEventPthread.h"
#  include "TCMTSemaphorePthread.h"
#  include "TCMTThreadPthread.h"
#  include "TCMTConditionPthread.h"
#endif

#include "TCMTMessageDispatcherImp.h"
#include "TCMTCommandExecutionThread.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace multi_threading
   {

      ThreadPtr factory::CreateThread(const std::string& thread_name,
         uint32_t stack_size/* =0 */, 
         Thread::ThreadPriority priority/* =Thread::PRIORITY_NORMAL */)
      {
#ifdef TCOS_WINDOWS
         return imp::ThreadWin32::Create(thread_name, stack_size, priority);
#else
         return imp::ThreadPthread::Create(thread_name, stack_size, priority);
#endif
      }

      ThreadPtr factory::CreateCommandExecutionThread(const std::string& thread_name,
         uint32_t stack_size/* =0 */, 
         Thread::ThreadPriority priority/* =Thread::PRIORITY_NORMAL */)
      {
         ThreadPtr thread = factory::CreateThread(thread_name, stack_size, priority);
         if (!thread->Start(ThreadObjectPtr(new CommandExecutionThreadObject)))
         {
            return ThreadPtr();
         }

         return thread;
      }

      MessageDispatcherPtr factory::CreateMessageDispatcher()
      {
         return MessageDispatcherPtr(new imp::MessageDispatcherImp());
      }

      ThreadPtr factory::GetCurrentThread()
      {
#ifdef TCOS_WINDOWS
         return imp::ThreadWin32::Self();
#else
         return imp::ThreadPthread::Self();
#endif
      }

      MutexPtr factory::CreateMutex(bool locked /* = false */)
      {
#ifdef TCOS_WINDOWS
         SharedPtr<imp::MutexWin32> mutex(new imp::MutexWin32);
#else
         SharedPtr<imp::MutexPthread> mutex(new imp::MutexPthread);
#endif
         if (!mutex->Init(locked))
         {
            return MutexPtr();
         }

         return mutex;
      }
      MutexPtr factory::CreateMutex(const std::string& shared_name,
          bool locked/* =false */, CreationMode mode /* = CRM_ALWAYS */)
      {
#ifdef TCOS_WINDOWS
         SharedPtr<imp::MutexWin32> mutex(new imp::MutexWin32);
#else
         SharedPtr<imp::MutexSharedPthread> mutex(new imp::MutexSharedPthread);
#endif
         if (!mutex->Init(shared_name, locked, mode))
         {
            return MutexPtr();
         }

         return mutex;
      }

      EventPtr factory::CreateEvent(bool manual_reset, bool initial_state)
      {
#ifdef TCOS_WINDOWS
         return EventPtr(new imp::EventWin32(manual_reset, initial_state));
#else
         return EventPtr(new imp::EventPthread(manual_reset, initial_state));
#endif
      }

      SemaphorePtr factory::CreateSemaphore(uint32_t initial_value /* = 1 */)
      {
#ifdef TCOS_WINDOWS
         SharedPtr<imp::SemaphoreWin32> semaphore(new imp::SemaphoreWin32);
#else
         SharedPtr<imp::SemaphorePthread> semaphore(new imp::SemaphorePthread);
#endif
         if (!semaphore->Init(initial_value))
         {
            return SemaphorePtr();
         }

         return semaphore;
      }

      SemaphorePtr factory::CreateSemaphore(const std::string& shared_name,
          uint32_t initial_value, CreationMode mode /* = CRM_ALWAYS */)
      {
#ifdef TCOS_WINDOWS
         SharedPtr<imp::SemaphoreWin32> semaphore(new imp::SemaphoreWin32);
#else
         SharedPtr<imp::SemaphorePthread> semaphore(new imp::SemaphorePthread);
#endif
         if (!semaphore->Init(shared_name, initial_value, mode))
         {
            return SemaphorePtr();
         }

         return semaphore;
      }

      ConditionPtr factory::CreateCondition()
      {
#ifdef TCOS_WINDOWS
         return ConditionPtr(new imp::ConditionWin32);
#else
         return ConditionPtr(new imp::ConditionPthread);
#endif
      }

      ConditionPtr factory::CreateCondition(MutexPtr mutex)
      {
#ifdef TCOS_WINDOWS
         return ConditionPtr(new imp::ConditionWin32(mutex));
#else
         return ConditionPtr(new imp::ConditionPthread(mutex));
#endif
      }

   } // namespace multi_threading
} // namespace tc
