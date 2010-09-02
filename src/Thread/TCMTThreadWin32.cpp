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
#ifdef TCOS_WINDOWS

#include "TCMTThreadWin32.h"

#include "TCMTMessageQueue.h"
#include "TCMTMutex.h"
#include "TCMTOS.h"
#include "TCOutput.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace MT
   {
      namespace Impl
      {
         static ThreadWin32 s_main_thread("main", ::GetCurrentThread(), ::GetCurrentThreadId());
         static ThreadPtr s_main_thread_ptr(&s_main_thread, TC::NoDelete());

         ThreadPtr ThreadWin32::Create(const std::string& thread_name,
            uint32 stack_size,
            ThreadPriority priority)
         {
            TCTRACE1("TCMT", 1, "%s", thread_name.c_str());

            ThreadWin32* thread = new ThreadWin32(thread_name, stack_size, priority);
            if (!thread)
            {
               TCERROR1("TCMT", "%s failed.", thread_name.c_str());
               return ThreadPtr();
            }

            ThreadPtr thread_ptr(thread);
            // add me to the list of created threads
            if (!m_threads_mutex)
            {
               m_threads_mutex = Factory::CreateMutex();
            }

            m_threads_mutex->Lock();
            m_threads.push_back(thread_ptr);
            m_threads_mutex->UnLock();

            if (!thread->Init())
            {
               TCERROR1("TCMT", "%s failed.", thread_name.c_str());
               return ThreadPtr();
            }

            TCTRACE1("TCMT", 1, "%s done.", thread_name.c_str());
            return thread_ptr;
         }

         ThreadWin32::ThreadWin32(const std::string& thread_name,
            uint32 stack_size, Thread::ThreadPriority priority)
            :ThreadBase(thread_name, stack_size, priority),
            m_handle(0),
            m_thread_id(0)
         {
         }

         ThreadWin32::ThreadWin32(const std::string& thread_name, void* handle, unsigned long id)
            :ThreadBase(thread_name, 0, PRIORITY_NORMAL),
            m_handle(handle),
            m_thread_id(id)
         {
         }

         ThreadWin32::~ThreadWin32()
         {
            TCTRACE1("TCMT", 1, "%s", m_name.c_str());

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
               TCERROR1("TCMT", "%s failed.", m_name.c_str());
               return false;
            }

            TCTRACE1("TCMT", 2, "%s done.", m_name.c_str());
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
            TCTRACE1("TCMT", 5,"%s", m_name.c_str());

            if (::WaitForSingleObject(m_handle, INFINITE)!= WAIT_OBJECT_0)
            {
               return false;
            }

            return true;
         }

         bool ThreadWin32::SetPriorityOS(ThreadPriority priority_in)
         {
            if (!m_handle)
            {
               TCTRACE1("TCMT", -1, "%s failed.", m_name.c_str());
               return false;
            }

            if (!::SetThreadPriority(m_handle, GetPriority(priority_in)))
            {
               TCTRACE1("TCMT", -1, "%s failed.", m_name.c_str());
               return false;
            }
            return true;
         }

         ThreadPtr ThreadWin32::Self()
         {
            TCTRACEF("TCMT", 10);

            DWORD id = ::GetCurrentThreadId();
            MutexLocker lock(m_threads_mutex);
            for (uint32 i=0; i<m_threads.size(); i++)
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

            return s_main_thread_ptr;
         }

         void ThreadWin32::SwitchContext()
         {
            TCTRACEF("TCMT", 10);
            ::Sleep(0);
         }

         sint32 ThreadWin32::GetPriority(ThreadPriority pri)
         {
            switch (pri)
            {
            case PRIORITY_LOWEST:
               return THREAD_PRIORITY_LOWEST;

            case PRIORITY_LOW:
               return THREAD_PRIORITY_BELOW_NORMAL;

            case PRIORITY_NORMAL:
            default:
               return THREAD_PRIORITY_NORMAL;

            case PRIORITY_HIGH:
               return THREAD_PRIORITY_ABOVE_NORMAL;

            case PRIORITY_HIGHEST:
               return THREAD_PRIORITY_HIGHEST;
            }
         }

      } // namespace Impl
   } // namespace MT
} // namespace TC

#endif // TCOS_WINDOWS
