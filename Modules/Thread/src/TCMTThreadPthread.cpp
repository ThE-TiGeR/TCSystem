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
//  $Id: TCMTThreadPthread.cpp,v 4eef1ecc751f 2015/03/25 22:34:01 thomas $
//*******************************************************************************
#include "TCDefines.h"

#ifdef TCOS_POSIX

#include "TCMTThreadPthread.h"

#include "TCMTMessageQueue.h"
#include "TCMTMutex.h"
#include "TCMTOS.h"
#include "TCOutput.h"
#include "TCString.h"

#include <algorithm>
#include <cstring>

#include "TCNewEnable.h"

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {
         ThreadPtr ThreadPthread::Create(const std::string& thread_name,
            uint32_t stack_size,
            ThreadPriority priority)
         {
            TCTRACES("TCMT", 1, thread_name);

            ThreadPthread* thread = new ThreadPthread(thread_name, stack_size, priority);
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
               TCERRORS("TCMT", thread_name << " failed.");
               return ThreadPtr();
            }

            TCTRACES("TCMT", 5, thread_name << " done.");
            return thread_ptr;
         }

         ThreadPthread::ThreadPthread(const std::string& thread_name,
            uint32_t stack_size, ThreadPriority priority)
            :ThreadBase(thread_name, stack_size, priority),
            m_handle(0)
         {
         }

         ThreadPthread::ThreadPthread(const std::string& thread_name, pthread_t handle)
            :ThreadBase(thread_name, 0, PRIORITY_NORMAL),
            m_handle(handle)
         {
         }

         ThreadPthread::~ThreadPthread()
         {
            TCTRACES("TCMT", 1, m_name);
         }

         bool ThreadPthread::CreateOSThread(InitStruct* init_data)
         {
            pthread_attr_t attr;
            pthread_attr_init(&attr);

            if (m_stack_size)
            {
               if (::pthread_attr_setstacksize(&attr, (size_t)m_stack_size) != 0)
               {
                  TCERRORS("TCMT", m_name << " failed.");
                  return false;
               }
            }

            if (::pthread_create(&m_handle, &attr, Wrapper, init_data)!= 0)
            {
               ::pthread_attr_destroy(&attr);
               TCERRORS("TCMT", m_name << " failed.");
               return false;
            }
            ::pthread_attr_destroy(&attr);

            TCTRACES("TCMT", 5, m_name << " done.");
            return true;
         }

         void* ThreadPthread::Wrapper(void* ptr)
         {
            InitStruct* init_data = reinterpret_cast<InitStruct*>(ptr);
            ThreadPthread* thread = static_cast<ThreadPthread*>(init_data->thread);

            return thread->ThreadRunner(init_data) ? (void*)(0x1) : (void*)(0x0);
         }

         bool ThreadPthread::JoinOS()
         {
            TCTRACES("TCMT", 5, m_name);

            if (::pthread_join(m_handle, 0) != 0)
            {
               return false;
            }

            return true;
         }

         bool ThreadPthread::SetPriorityOS(ThreadPriority priority_in)
         {
            TCTRACES("TCMT", 5, m_name);

            if (!m_handle)
            {
               TCERRORS("TCMT", m_name << " failed.");
               return false;
            }

            struct sched_param sparam;
            int32_t policy = 0;
            int32_t error = pthread_getschedparam(m_handle, &policy, &sparam);
            if (error != 0)
            {
               TCERRORS("TCMT", m_name << "(error=" << error << " " << std::strerror(error) << " failed.");
               return false;
            }

            sparam.sched_priority = GetPriority(priority_in);
            error = ::pthread_setschedparam(m_handle, policy, &sparam);
            if (error != 0)
            {
               TCERRORS("TCMT", m_name << "(error=" << error << " " << std::strerror(error) << " failed.");
               return false;
            }

            return true;
         }

         ThreadPtr ThreadPthread::Self()
         {
            TCTRACES("TCMT", 10, "");

            pthread_t handle = ::pthread_self();
            MutexLocker lock(m_threads_mutex);
            for (uint32_t i=0; i<m_threads.size(); i++)
            {
               ThreadPtr thread = m_threads[i];
               SharedPtr< ThreadPthread > thread_imp;
               thread_imp = SharedPtr< ThreadPthread >::DynamicCast(thread);
               if (!thread_imp) continue;

               if (thread_imp->m_handle == handle)
               {
                  return m_threads[i];
               }
            }

            ThreadPtr thread_ptr(new ThreadPthread(string::ToString(uint64_t(handle)), handle));
            m_threads.push_back(thread_ptr);

            return thread_ptr;
         }

         void ThreadPthread::SwitchContext()
         {
            TCTRACES("TCMT", 10, "");
            ::sched_yield();
         }

         int32_t ThreadPthread::GetPriority(ThreadPriority pri)
         {
            static bool priorities_setup = false;
            static int32_t lowest_priority = 0;
            static int32_t normal_priority = 0;
            static int32_t highest_priority = 0;

            if (!priorities_setup)
            {
               lowest_priority  = ::sched_get_priority_min(SCHED_OTHER);
               highest_priority = ::sched_get_priority_max(SCHED_OTHER);

               switch (highest_priority - lowest_priority)
               {
               case 0:
               case 1:
                  normal_priority = lowest_priority;
                  break;

               default:
                  normal_priority = (lowest_priority + highest_priority)/2;
                  break;
               }

               priorities_setup = true;
            }

            switch (pri)
            {
            case PRIORITY_LOWEST:
               return lowest_priority;
            case PRIORITY_LOW:
               return (normal_priority + lowest_priority)/2;
            case PRIORITY_HIGH:
               return (normal_priority + highest_priority)/2;
            case PRIORITY_HIGHEST:
               return highest_priority;
            case PRIORITY_NORMAL:
            default:
               return normal_priority;
            }
         }
      } // namespace imp
   } // namespace multi_threading
} // namespace tc

#endif // TCOS_POSIX
