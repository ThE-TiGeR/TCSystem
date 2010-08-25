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
//  $Id: TCMTFactory.cpp 957 2010-01-28 23:17:00Z the_____tiger $
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

#include "TCNewEnable.h"

namespace TC
{
   namespace MT
   {

      ThreadPtr Factory::CreateThread(const std::string& thread_name,
         uint32 stack_size/* =0 */, 
         Thread::ThreadPriority priority/* =Thread::PRIORITY_NORMAL */)
      {
#ifdef TCOS_WINDOWS
         return Impl::ThreadWin32::Create(thread_name, stack_size, priority);
#else
         return Impl::ThreadPthread::Create(thread_name, stack_size, priority);
#endif
      }

      MessageDispatcherPtr Factory::CreateMessageDispatcher()
      {
         return MessageDispatcherPtr(new Impl::MessageDispatcherImp());
      }

      ThreadPtr Factory::GetCurrentThread()
      {
#ifdef TCOS_WINDOWS
         return Impl::ThreadWin32::Self();
#else
         return Impl::ThreadPthread::Self();
#endif
      }

      MutexPtr Factory::CreateMutex(bool locked /* = false */)
      {
#ifdef TCOS_WINDOWS
         return MutexPtr(new Impl::MutexWin32(locked));
#else
         return MutexPtr(new Impl::MutexPthread(locked));
#endif
      }
      MutexPtr Factory::CreateMutex(const std::string& shared_name,
          bool locked/* =false */, CreationMode /* mode = CRM_ALWAYS */)
      {
#ifdef TCOS_WINDOWS
         return MutexPtr(new Impl::MutexWin32(shared_name, locked));
#else
         return MutexPtr(new Impl::MutexSharedPthread(shared_name, locked));
#endif
      }

      EventPtr Factory::CreateEvent(bool manual_reset, bool initial_state)
      {
#ifdef TCOS_WINDOWS
         return EventPtr(new Impl::EventWin32(manual_reset, initial_state));
#else
         return EventPtr(new Impl::EventPthread(manual_reset, initial_state));
#endif
      }

      SemaphorePtr Factory::CreateSemaphore(uint32 initial_value /* = 1 */)
      {
#ifdef TCOS_WINDOWS
         return SemaphorePtr(new Impl::SemaphoreWin32(initial_value));
#else
         return SemaphorePtr(new Impl::SemaphorePthread(initial_value));
#endif
      }

      SemaphorePtr Factory::CreateSemaphore(const std::string& shared_name,
          uint32 initial_value, CreationMode /* mode = CRM_ALWAYS */)
      {
#ifdef TCOS_WINDOWS
         return SemaphorePtr(new Impl::SemaphoreWin32(shared_name, initial_value));
#else
         return SemaphorePtr(new Impl::SemaphorePthread(shared_name, initial_value));
#endif
      }

      ConditionPtr Factory::CreateCondition()
      {
#ifdef TCOS_WINDOWS
         return ConditionPtr(new Impl::ConditionWin32);
#else
         return ConditionPtr(new Impl::ConditionPthread);
#endif
      }

      ConditionPtr Factory::CreateCondition(MutexPtr mutex)
      {
#ifdef TCOS_WINDOWS
         return ConditionPtr(new Impl::ConditionWin32(mutex));
#else
         return ConditionPtr(new Impl::ConditionPthread(mutex));
#endif
      }

   } // namespace MT
} // namespace TC
