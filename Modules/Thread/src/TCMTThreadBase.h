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
//  $Id$
//*******************************************************************************

#ifndef _TC_MT_THREAD_BASE_H_
#define _TC_MT_THREAD_BASE_H_

#include "TCMTThread.h"
#include "TCMTLockable.h"
#include "TCMTMessageQueue.h"

#include <string>
#include <vector>

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {

         class MessageQueue;

         /**
         * @addtogroup TC_MT_IMP
         * @{
         */

         /**
         * @brief Class for creating a thread which then can start an ThreadObject
         */
         class TC_DLL_LOCAL ThreadBase: public Thread,
            public ObjectLevelLockable<ThreadBase>
         {
         public:
            ThreadBase(const std::string& thread_name,
               uint32_t stack_size, Thread::ThreadPriority priority);
            virtual ~ThreadBase();

            bool Start(ThreadObjectPtr object_to_start);
            bool Join();

            bool IsRunning() const;
            bool IsTerminated() const;

            bool SetPriority(ThreadPriority priority);
            ThreadPriority GetPriority() const;

            bool SendThreadMessage(MessagePtr message);
            Message::ReturnValue SendSyncThreadMessage(MessagePtr message);
            bool SendReplyThreadMessage(MessagePtr message);
            Message::ReturnValue WaitThreadMessage(MessagePtr &message);
            Message::ReturnValue WaitThreadMessage(MessagePtr &message, const Time& timeout);

         protected:
            struct InitStruct
            {
               ThreadBase* thread;
               EventPtr event;
            };

            /** methode which is executed by the thread and waits for start and stop messages */
            uint32_t ThreadRunner(InitStruct* init_data);

            /** create the os dependent thread */
            virtual bool CreateOSThread(InitStruct* init_data) = 0;
            virtual bool JoinOS() = 0;
            virtual bool SetPriorityOS(ThreadPriority priority) = 0;

         protected:
            /**
            * @brief Creates the thread
            * @return true if sucessfully created
            */
            bool Init();

            /** @return thread state (invalid, new, running or terminated). */
            ThreadState GetState() const;

            /** Holds the name of the thread */
            std::string m_name;
            /** The current priority of this thread */
            ThreadPriority m_priority;
            /** The current stack size of this thread */
            uint32_t m_stack_size;
            /** The current state of the thread */
            ThreadState m_state;
            /** pointer to the message queue of the thread */
            MessageQueue m_message_queue;

            /** Array of created threads */
            static std::vector<ThreadPtr> m_threads;
            static MutexPtr m_threads_mutex;
         };

         /**
         * @}
         */

      } // namespace imp
   } // namespace multi_threading
} // namespace tc

#endif // _TC_MT_THREAD_BASE_H_
