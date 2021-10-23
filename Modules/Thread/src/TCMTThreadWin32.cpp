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
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCMTThreadWin32.cpp,v 060ee5db7c99 2015/02/08 20:38:30 thomas $
//*******************************************************************************
#include "TCDefines.h"

#ifdef TCOS_WINDOWS

#include "TCMTThreadWin32.h"

#include "TCMTMessageQueue.h"
#include "TCMTMutex.h"
#include "TCMTOS.h"
#include "TCOutput.h"
#include "TCString.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {
         ThreadPtr ThreadWin32::Create(const std::string& thread_name,
            uint32_t stack_size,
            Priority priority)
         {
            TCTRACES("TCMT", 1, thread_name);

            ThreadWin32* thread = new ThreadWin32(thread_name, stack_size, priority);
            if (!thread)
            {
               TCERRORS("TCMT", thread_name << " failed.");
               return ThreadPtr();
            }

            ThreadPtr thread_ptr(thread);
            // add me to the list of created threads
            if (!m_threads_mutex)
            {
               m_threads_mutex = factory::CreateMutex();
            }

            m_threads_mutex->Lock();
            m_threads.push_back(thread_ptr);
            m_threads_mutex->UnLock();

            if (!thread->Init())
            {
               TCERRORS("TCMT", thread_name << " Init failed.");
               return ThreadPtr();
            }

            TCTRACES("TCMT", 1, thread_name << " done.");
            return thread_ptr;
         }

         ThreadWin32::ThreadWin32(const std::string& thread_name,
            uint32_t stack_size, Thread::Priority priority)
            :ThreadBase(thread_name, stack_size, priority),
            m_handle(0),
            m_thread_id(0)
         {
         }

         ThreadWin32::ThreadWin32(const std::string& thread_name, void* handle, unsigned long id)
            :ThreadBase(thread_name, 0, Priority::NORMAL),
            m_handle(handle),
            m_thread_id(id)
         {
         }

         ThreadWin32::~ThreadWin32()
         {
            TCTRACES("TCMT", 1, m_name);

            if (m_handle)
            {
               ::CloseHandle(m_handle);
               m_handle = 0;
            }
         }

         bool ThreadWin32::CreateOSThread(InitStruct* init_data)
         {
            m_handle = ::CreateThread(0, m_stack_size, Wrapper, init_data, 0, &m_thread_id);
            if (m_handle == 0)
            {
               TCERRORS("TCMT", m_name << " failed.");
               return false;
            }

            TCTRACES("TCMT", 1, m_name << " done.");
            return true;
         }

         unsigned long ThreadWin32::Wrapper(void *ptr)
         {
            InitStruct* init_data = reinterpret_cast<InitStruct*>(ptr);
            ThreadWin32* thread = static_cast<ThreadWin32*>(init_data->thread);

            return thread->ThreadRunner(init_data);
         }

         bool ThreadWin32::JoinOS()
         {
            TCTRACES("TCMT", 5, m_name);

            if (::WaitForSingleObject(m_handle, INFINITE)!= WAIT_OBJECT_0)
            {
               return false;
            }

            return true;
         }

         bool ThreadWin32::SetPriorityOS(Priority priority_in)
         {
            if (!m_handle)
            {
               TCERRORS("TCMT", m_name << " failed.");
               return false;
            }

            if (!::SetThreadPriority(m_handle, GetPriority(priority_in)))
            {
               TCERRORS("TCMT", m_name << " failed.");
               return false;
            }
            return true;
         }

         ThreadPtr ThreadWin32::Self()
         {
            TCTRACES("TCMT", 10, "");

            DWORD id = ::GetCurrentThreadId();
            MutexLocker lock(m_threads_mutex);
            for (uint32_t i=0; i<m_threads.size(); i++)
            {
               ThreadPtr thread = m_threads[i];
               SharedPtr< ThreadWin32 > thread_imp;
               thread_imp = SharedPtr< ThreadWin32 >::DynamicCast(thread);
               if (!thread_imp) continue;

               if (thread_imp->m_thread_id == id)
               {
                  return m_threads[i];
               }
            }

            ThreadPtr thread_ptr(new ThreadWin32(string::ToString(uint64_t(id)), ::GetCurrentThread(), id));
            m_threads.push_back(thread_ptr);

            return thread_ptr;
         }

         void ThreadWin32::SwitchContext()
         {
            TCTRACES("TCMT", 10, "");
            ::Sleep(0);
         }

         int32_t ThreadWin32::GetPriority(Priority pri)
         {
            switch (pri)
            {
            case Priority::LOWEST:
               return THREAD_PRIORITY_LOWEST;

            case Priority::LOW:
               return THREAD_PRIORITY_BELOW_NORMAL;

            case Priority::NORMAL:
            default:
               return THREAD_PRIORITY_NORMAL;

            case Priority::HIGH:
               return THREAD_PRIORITY_ABOVE_NORMAL;

            case Priority::HIGHEST:
               return THREAD_PRIORITY_HIGHEST;
            }
         }

      } // namespace imp
   } // namespace multi_threading
} // namespace tc

#endif // TCOS_WINDOWS
