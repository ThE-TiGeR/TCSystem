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

#include "TCMTThreadBase.h"

#include "TCMTMessageQueue.h"
#include "TCMTMutex.h"
#include "TCMTOS.h"
#include "TCOutput.h"

#include <algorithm>

#include "TCNewEnable.h"

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {
         /** Array of created threads */
         std::vector<ThreadPtr> ThreadBase::m_threads;
         MutexPtr ThreadBase::m_threads_mutex;

         class StopThreadMessage: public Message
         {
         public:
            enum { MESSAGE_ID = Message::MSG_ID_START };

            StopThreadMessage()
               :Message(MESSAGE_ID){}
         };

         class StartThreadMessage: public Message
         {
         public:
            enum { MESSAGE_ID = Message::MSG_ID_START + 1 };

            StartThreadMessage(ThreadObjectPtr object_to_start)
               :Message(MESSAGE_ID),
               m_object_to_start(object_to_start)
            {}

            ThreadObjectPtr m_object_to_start;
         };

         ThreadBase::ThreadBase(const std::string& thread_name,
            uint32_t stack_size,
            ThreadPriority priority)
            :m_name(thread_name),
            m_priority(priority),
            m_stack_size(stack_size),
            m_state(STATE_NEW)
         {
            TCTRACES("TCMT", 1, m_name << "(" << m_threads.size() << ")");
         }

         ThreadBase::~ThreadBase()
         {
            TCTRACES("TCMT", 1, m_name);
         }

         bool ThreadBase::Init()
         {
            TCTRACES("TCMT", 2, m_name << " ...");

            InitStruct init_data;
            init_data.thread = this;
            init_data.event = factory::CreateEvent();

            if (!CreateOSThread(&init_data))
            {
               TCERRORS("TCMT", m_name << ", Error creating thread.");
               return false;
            }

            if (!init_data.event->Wait())
            {
               TCERRORS("TCMT", m_name << ", Error waiting thread startup.");
               return false;
            }

            if (!SetPriority(m_priority))
            {
               TCERRORS("TCMT", m_name << ", Error setting priority.");
               return false;
            }

            TCTRACES("TCMT", 2, m_name << " done.");
            return true;
         }

         uint32_t ThreadBase::ThreadRunner(InitStruct* init_data)
         {
            TCTRACES("TCMT", 3, m_name << " ...");

            m_state = STATE_RUNNING;
            init_data->event->Set();

            MessagePtr message;

            while (m_state == STATE_RUNNING &&
               WaitThreadMessage(message) != Message::MSG_RECEIVE_FAILED)
            {
               TCTRACES("TCMT", 50, m_name << " Received message " << message->GetMessageId());
               switch(message->GetMessageId())
               {
               case StopThreadMessage::MESSAGE_ID:
                  {
                     TCTRACES("TCMT", 4, m_name << "Stop.");
                     m_state = STATE_TERMINATED;
                  }
                  break;

               case StartThreadMessage::MESSAGE_ID:
                  {
                     TCTRACES("TCMT", 4, m_name << "Run ...");
                     SharedPtr<StartThreadMessage>::DynamicCast(message)->m_object_to_start->Run();
                     TCTRACES("TCMT", 4, m_name << "Run done.");
                     break;
                  }

               case Message::MSG_ID_QUIT:
                  break;

               default:
                  TCERRORS("TCMT", m_name << " Received unknown message " << message->GetMessageId());
                  break;
               }

               message = MessagePtr();
            }

            m_state = STATE_TERMINATED;

            // remove me from the list of created threads
            m_threads_mutex->Lock();
            std::vector<ThreadPtr>::iterator thread_it;
            thread_it = std::find(m_threads.begin(), m_threads.end(), factory::GetCurrentThread());
            m_threads.erase(thread_it);

            // if the last thread we free the array
            uint64_t num_threads = m_threads.size();
            if (num_threads== 0)
            {
               m_threads_mutex->UnLock();
               std::vector<ThreadPtr> tmp;
               m_threads.swap(tmp);
               m_threads_mutex = MutexPtr();
            }
            else
            {
               m_threads_mutex->UnLock();
            }

            TCTRACES("TCMT", 3, m_name << " leave. (still running " << num_threads << ")");

            return 0;
         }


         bool ThreadBase::Start(ThreadObjectPtr object_to_start)
         {
            TCTRACES("TCMT", 5, m_name << " ...");

            return SendThreadMessage(MessagePtr(new StartThreadMessage(object_to_start)));
         }

         bool ThreadBase::Join()
         {
            TCTRACES("TCMT", 5, m_name << " ...");

            if (factory::GetCurrentThread() == this)
            {
               return false;
            }

            // check if allready terminated
            if (IsTerminated())
            {
               return true;
            }

            if (!SendThreadMessage(MessagePtr(new StopThreadMessage())))
            {
               return false;
            }

            return JoinOS();
         }

         bool ThreadBase::IsRunning() const
         { 
            return GetState() == STATE_RUNNING;
         }

         bool ThreadBase::IsTerminated() const
         { 
            return GetState() == STATE_TERMINATED;
         }

         bool ThreadBase::SetPriority(ThreadPriority priority_in)
         {
            TCTRACES("TCMT", 5, m_name << " ...");

            if (!SetPriorityOS(priority_in))
            {
               return false;
            }

            Locker lock(this);
            m_priority = priority_in;

            TCTRACES("TCMT", 5, m_name << " done.");
            return true;
         }

         ThreadBase::ThreadState ThreadBase::GetState()const
         {
            TCTRACES("TCMT", 10, m_name << " ...");
            Locker lock(const_cast<ThreadBase*>(this));

            return m_state;
         }

         ThreadBase::ThreadPriority ThreadBase::GetPriority()const
         {
            TCTRACES("TCMT", 10, m_name << " ...");
            Locker lock(const_cast<ThreadBase*>(this));

            return m_priority;
         }

         Message::ReturnValue ThreadBase::WaitThreadMessage(MessagePtr &message, const Time& timeout)
         {
            TCTRACES("TCMT", 500, m_name << " " << timeout.ToMilliSeconds() << "ms ...");
            Message::ReturnValue ret;
            if (timeout == Time::Zero())
            {
               ret = m_message_queue.TryGetMessage(uint32_t(Message::MSG_ID_UNKNOWN),
                  uint32_t(Message::MSG_ID_UNKNOWN),
                  message, true);
            }
            else
            {
               ret = m_message_queue.GetMessage(uint32_t(Message::MSG_ID_UNKNOWN),
                  uint32_t(Message::MSG_ID_UNKNOWN),
                  message, timeout);
            }

            return ret;
         }

         Message::ReturnValue ThreadBase::WaitThreadMessage(MessagePtr &message)
         {
            TCTRACES("TCMT", 50, m_name << " ...");
            Message::ReturnValue ret = m_message_queue.GetMessage(uint32_t(Message::MSG_ID_UNKNOWN),
               uint32_t(Message::MSG_ID_UNKNOWN),
               message);

            if (ret == Message::MSG_RECEIVE_FAILED)
            {
               TCERRORS("TCMT", m_name << " failed.");
               return ret;
            }

            TCTRACES("TCMT", 50, m_name << " done.");
            return ret;
         }

         bool ThreadBase::SendThreadMessage(MessagePtr message)
         {
            TCTRACES("TCMT", 50, m_name << " ...");
            // check if already terminated because then it is not possible to send a message
            if (IsTerminated())
            {
               TCERRORS("TCMT", m_name << " failed.");
               return false;
            }

            if (!m_message_queue.AddMessage(message))
            {
               TCERRORS("TCMT", m_name << " failed.");
               return false;
            }

            TCTRACES("TCMT", 50, m_name << " done.");
            return true;
         }

         Message::ReturnValue ThreadBase::SendSyncThreadMessage(MessagePtr message)
         {
            TCTRACES("TCMT", 50, m_name << " ...");
            Message::ReturnValue ret = Message::MSG_RECEIVE_FAILED;
            if (SendThreadMessage(message))
            {
               SharedPtr<ThreadBase> thread = SharedPtr<ThreadBase>::StaticCast(message->GetSenderThread());

               MessagePtr ret_message;
               ret = thread->m_message_queue.GetMessage(message->GetMessageId(),
                  message->GetMessageId(), ret_message);
               if (ret == Message::MSG_RECEIVED)
               {
                  ret = ret_message->GetReplyId() == message->GetReplyId() ? Message::MSG_RECEIVED : Message::MSG_RECEIVE_FAILED;
               }
            }

            TCTRACES("TCMT", 50, m_name << " done(" << ret << ").");
            return ret;
         }

         bool ThreadBase::SendReplyThreadMessage(MessagePtr message)
         {
            return SendThreadMessage(message);
         }

      } // namespace imp
   } // namespace multi_threading
} // namespace tc
