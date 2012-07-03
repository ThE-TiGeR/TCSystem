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

#include "TCAudioStreamingThread.h"

#include "TCAssert.h"
#include "TCAudioOpenALHandler.h"
#include "TCAudioStreamingTask.h"
#include "TCAudioUtil.h"
#include "TCException.h"
#include "TCMTFactory.h"
#include "TCMTMessage.h"
#include "TCOutput.h"
#include "TCTime.h"
#include "TCUtil.h"

#include <limits>

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      const Time STREAMING_TIMEOUT(0, 50 * Time::ONE_MILLI_SECOND_AS_NANO_SECONDS);
      const Time RESPONSE_TIMEOUT(0, 200 * Time::ONE_MILLI_SECOND_AS_NANO_SECONDS);

      enum MessageIds
      {
         PLAY_MESSAGE = multi_threading::Message::MSG_ID_USER_START,
         PAUSE_MESSAGE,
         STOP_MESSAGE,

         RESPONSE_OK,
         RESPONSE_FAILED
      };

      StreamingThread::StreamingThread()
         :m_thread(),
         m_running(),
         m_message_dispatcher(),
         m_tasks(),
         m_streaming_task()
      {
         m_running            = multi_threading::factory::CreateEvent(true, false);
         m_message_dispatcher = multi_threading::factory::CreateMessageDispatcher();
         m_streaming_task     = SharedPtr<StreamingTask>(new StreamingTask);

         m_message_dispatcher->RegisterMessageCallback<SoundDataMessage>(PLAY_MESSAGE,
            m_streaming_task, &StreamingTask::OnPlayMessage);
         m_message_dispatcher->RegisterMessageCallback<SoundDataMessage>(PLAY_MESSAGE,
            m_streaming_task, &StreamingTask::OnPlayMessage);
         m_message_dispatcher->RegisterMessageCallback<SoundDataMessage>(PAUSE_MESSAGE, 
            m_streaming_task, &StreamingTask::OnPauseMessage);
         m_message_dispatcher->RegisterMessageCallback<SoundDataMessage>(STOP_MESSAGE, 
            m_streaming_task, &StreamingTask::OnStopMessage);

         AddTask(m_streaming_task);
      }

      StreamingThread::~StreamingThread()
      {
      }

      bool StreamingThread::Run()
      {
         m_thread = multi_threading::factory::GetCurrentThread();
         m_running->Set();

         try
         {
            bool run = true;
            while (run)
            {
               Time next_time_out;
               TaskPtr next_task = GetNextTask(next_time_out);
               multi_threading::MessagePtr message;
               multi_threading::Message::ReturnValue ret_val = multi_threading::Message::MSG_RECEIVE_TIMEOUT;
               if (next_time_out > Time::Zero())
               {
                  ret_val = m_thread->WaitThreadMessage(message, next_time_out);
               }

               switch(ret_val)
               {
               case multi_threading::Message::MSG_RECEIVE_TIMEOUT:
                  {
                     if (next_task->Execute() == Task::TASK_FINISHED)
                     {
                        RemoveTask(next_task);
                     }
                     next_task->m_last_processed = Time::Now();
                     next_task = GetNextTask(next_time_out);
                  }
                  break;

               case multi_threading::Message::MSG_RECEIVED:
                  if (!m_message_dispatcher->DispatchMessage(message))
                  {
                     message->GetSenderThread()->SendThreadMessage(
                        multi_threading::MessagePtr(new multi_threading::Message(RESPONSE_FAILED)));
                  }
                  else
                  {
                     message->GetSenderThread()->SendThreadMessage(
                        multi_threading::MessagePtr(new multi_threading::Message(RESPONSE_OK)));
                  }

                  message = multi_threading::MessagePtr();
                  break;

               case multi_threading::Message::MSG_QUIT_RECEIVED:
                  run = false;
                  break;

               case multi_threading::Message::MSG_RECEIVE_FAILED:
                  throw Exception("WaitThreadMessage failed");

               }
            }
         }

         catch (Exception& e)
         {
            TCERRORS("TCAUDIO", "failed with exception\n" << e.what());
            m_running->Reset();
            return false;
         }

         m_running->Reset();
         return true;
      }

      bool StreamingThread::AddTask(TaskPtr task)
      {
         m_tasks.push_back(task);

         return true;
      }

      bool StreamingThread::RemoveTask(TaskPtr task)
      {
         std::vector< TaskPtr >::iterator task_found = std::find(m_tasks.begin(), m_tasks.end(), task);
         if (task_found == m_tasks.end())
         {
            return false;
         }

         m_tasks.erase(task_found);

         return true;
      }

      StreamingThread::TaskPtr StreamingThread::GetNextTask(Time& next_time_out)
      {
         // find next timeout
         Time current_time = Time::Now();
         next_time_out = Time::Infinite();
         if (m_tasks.empty())
         {
            return TaskPtr();
         }

         std::vector< TaskPtr >::iterator next_task = m_tasks.begin();
         for (std::vector< TaskPtr >::iterator task=m_tasks.begin(); task != m_tasks.end(); task++)
         {
            Time diff_timeout = current_time - (*task)->m_last_processed;
            if (diff_timeout >= (*task)->m_timeout)
            {
               next_task = task;
               next_time_out = Time::Zero();
               break;
            }
            else if ((*task)->m_timeout - diff_timeout < next_time_out)
            {
               next_task = task;
               next_time_out  = (*task)->m_timeout - diff_timeout;
            }
         }

         return *next_task;
      }

      bool StreamingThread::PlaySoundData(SoundDataPtr sound_data)
      {
         multi_threading::MessagePtr smessage(new SoundDataMessage(PLAY_MESSAGE, sound_data));
         m_thread->SendThreadMessage(smessage);

         multi_threading::MessagePtr rmessage;
         smessage->GetSenderThread()->WaitThreadMessage(rmessage, RESPONSE_TIMEOUT);
         return rmessage && rmessage->GetMessageId() == RESPONSE_OK;
      }

      bool StreamingThread::PauseSoundData(SoundDataPtr sound_data)
      {
         multi_threading::MessagePtr smessage(new SoundDataMessage(PAUSE_MESSAGE, sound_data));
         m_thread->SendThreadMessage(smessage);

         multi_threading::MessagePtr rmessage;
         smessage->GetSenderThread()->WaitThreadMessage(rmessage, RESPONSE_TIMEOUT);
         return rmessage && rmessage->GetMessageId() == RESPONSE_OK;
      }

      bool StreamingThread::StopSoundData(SoundDataPtr sound_data)
      {
         multi_threading::MessagePtr smessage(new SoundDataMessage(STOP_MESSAGE, sound_data));
         m_thread->SendThreadMessage(smessage);

         multi_threading::MessagePtr rmessage;
         smessage->GetSenderThread()->WaitThreadMessage(rmessage, RESPONSE_TIMEOUT);
         return rmessage && rmessage->GetMessageId() == RESPONSE_OK;
      }

      void StreamingThread::Stop()
      {
         if (m_thread)
         {
            multi_threading::MessagePtr msg(new multi_threading::Message(multi_threading::Message::MSG_ID_QUIT));
            m_thread->SendThreadMessage(msg);
         }
      }

      bool StreamingThread::IsSoundDataPlaying(SoundDataCPtr sound_data) const
      {
         return m_streaming_task->IsSoundDataPlaying(sound_data);
      }

      bool StreamingThread::IsSoundDataPaused(SoundDataCPtr sound_data) const
      {
         return m_streaming_task->IsSoundDataPaused(sound_data);
      }

      bool StreamingThread::IsSoundDataStopped(SoundDataCPtr sound_data) const
      {
         return m_streaming_task->IsSoundDataStopped(sound_data);
      }
   }
}
