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

#include "TCMTThreadBase.h"

#include "TCMTMessageQueue.h"
#include "TCMTMutex.h"
#include "TCMTOS.h"
#include "TCOutput.h"

#include <algorithm>

#include "TCNewEnable.h"

namespace TC
{
   namespace MT
   {
      namespace Impl
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
            uint32 stack_size,
            ThreadPriority priority)
            :m_name(thread_name),
            m_priority(priority),
            m_stack_size(stack_size),
            m_state(STATE_NEW)
         {
            TCTRACE2("TCMT", 1, "%s(%d)", m_name.c_str(), m_threads.size());
         }

         ThreadBase::~ThreadBase()
         {
            TCTRACE1("TCMT", 1, "%s", m_name.c_str());
         }

         bool ThreadBase::Init()
         {
            TCTRACE1("TCMT", 2,"%s ...", m_name.c_str());

            InitStruct init_data;
            init_data.thread = this;
            init_data.event = Factory::CreateEvent();

            if (!CreateOSThread(&init_data))
            {
               TCERROR1("TCMT", "%s error creating thread.", m_name.c_str());
               return false;
            }

            if (!init_data.event->Wait())
            {
               TCERROR1("TCMT", "%s error waitng thread startup.", m_name.c_str());
               return false;
            }

            if (!SetPriority(m_priority))
            {
               TCERROR1("TCMT", "%s failed.", m_name.c_str());
               return false;
            }

            TCTRACE1("TCMT", 2, "%s done.", m_name.c_str());
            return true;
         }

         uint32 ThreadBase::ThreadRunner(InitStruct* init_data)
         {
            TCTRACE1("TCMT", 3,"%s enter", m_name.c_str());

            m_state = STATE_RUNNING;
            init_data->event->Set();

            MessagePtr message;

            while (m_state == STATE_RUNNING &&
               WaitThreadMessage(message) != Message::MSG_RECEIVE_FAILED)
            {
               TCTRACE2("TCMT", 50, "%s Received message %d",
                  m_name.c_str(), message->GetMessageId());
               switch(message->GetMessageId())
               {
               case StopThreadMessage::MESSAGE_ID:
                  {
                     TCTRACE1("TCMT", 4,"%s Stop.", m_name.c_str());
                     m_state = STATE_TERMINATED;
                  }
                  break;

               case StartThreadMessage::MESSAGE_ID:
                  {
                     TCTRACE1("TCMT", 4,"%s Run ...", m_name.c_str());
                     SharedPtr<StartThreadMessage>::DynamicCast(message)->m_object_to_start->Run();
                     TCTRACE1("TCMT", 4,"%s Run done.", m_name.c_str());
                     break;
                  }

               default:
                  TCERROR2("TCMT", "%s Received unknown message %d",
                     m_name.c_str(), message->GetMessageId());
                  break;
               }

               message = MessagePtr();
            }

            m_state = STATE_TERMINATED;

            // remove me from the list of created threads
            m_threads_mutex->Lock();
            std::vector<ThreadPtr>::iterator thread_it;
            thread_it = std::find(m_threads.begin(), m_threads.end(), Factory::GetCurrentThread());
            m_threads.erase(thread_it);

            // if the last thread we free the array
            uint64 num_threads = m_threads.size();
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

            TCTRACE2("TCMT", 3,"%s leave. (still running %" TC_UINT64_FORMAT ")", m_name.c_str(), num_threads);

            return 0;
         }


         bool ThreadBase::Start(ThreadObjectPtr object_to_start)
         {
            TCTRACE1("TCMT", 5,"%s", m_name.c_str());

            return SendThreadMessage(MessagePtr(new StartThreadMessage(object_to_start)));
         }

         bool ThreadBase::Join()
         {
            TCTRACE1("TCMT", 5,"%s", m_name.c_str());

            if (Factory::GetCurrentThread() == this)
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
            TCTRACE1("TCMT", 5,"%s", m_name.c_str());

            if (!SetPriorityOS(priority_in))
            {
               return false;
            }

            Locker lock(this);
            m_priority = priority_in;

            TCTRACE1("TCMT", 5,"%s done.", m_name.c_str());
            return true;
         }

         ThreadBase::ThreadState ThreadBase::GetState()const
         {
            TCTRACE1("TCMT", 10,"%s ...", m_name.c_str());
            Locker lock(const_cast<ThreadBase*>(this));

            return m_state;
         }

         ThreadBase::ThreadPriority ThreadBase::GetPriority()const
         {
            TCTRACE1("TCMT", 10,"%s ...", m_name.c_str());
            Locker lock(const_cast<ThreadBase*>(this));

            return m_priority;
         }

         Message::ReturnValue ThreadBase::WaitThreadMessage(MessagePtr &message, const Time& timeout)
         {
            TCTRACE2("TCMT", 100, "%s %" TC_UINT64_FORMAT " ...", m_name.c_str(), timeout.ToMilliSeconds());
            Message::ReturnValue ret;
            if (timeout == Time::Zero())
            {
               ret = m_message_queue.TryGetMessage(uint32(Message::MSG_ID_UNKNOWN),
                  uint32(Message::MSG_ID_UNKNOWN),
                  message, true);
            }
            else
            {
               ret = m_message_queue.GetMessage(uint32(Message::MSG_ID_UNKNOWN),
                  uint32(Message::MSG_ID_UNKNOWN),
                  message, timeout);
            }

            return ret;
         }

         Message::ReturnValue ThreadBase::WaitThreadMessage(MessagePtr &message)
         {
            TCTRACE1("TCMT", 50, "%s ...", m_name.c_str());
            Message::ReturnValue ret = m_message_queue.GetMessage(uint32(Message::MSG_ID_UNKNOWN),
               uint32(Message::MSG_ID_UNKNOWN),
               message);

            if (ret == Message::MSG_RECEIVE_FAILED)
            {
               TCERROR1("TCMT", "%s failed.", m_name.c_str());
               return ret;
            }

            TCTRACE1("TCMT", 50, "%s done.", m_name.c_str());
            return ret;
         }

         bool ThreadBase::SendThreadMessage(MessagePtr message)
         {
            TCTRACE1("TCMT", 50, "%s ...", m_name.c_str());
            // check if already terminated because then it is not possible to send a message
            if (IsTerminated())
            {
               TCERROR1("TCMT", "%s failed.", m_name.c_str());
               return false;
            }

            if (!m_message_queue.AddMessage(message))
            {
               TCERROR1("TCMT", "%s failed.", m_name.c_str());
               return false;
            }

            return true;
         }

         Message::ReturnValue ThreadBase::SendSyncThreadMessage(MessagePtr message)
         {
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

            return ret;
         }

         bool ThreadBase::SendReplyThreadMessage(MessagePtr message)
         {
            return SendThreadMessage(message);
         }

      } // namespace Impl
   } // namespace MT
} // namespace TC
