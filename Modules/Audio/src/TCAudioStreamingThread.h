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
#ifndef _TC_AUDIO_STREAMING_THREAD_H_
#define _TC_AUDIO_STREAMING_THREAD_H_

#include "TCAudioOpenAL.h"
#include "TCAudioSoundPlayer.h"
#include "TCMTThread.h"
#include "TCMTEvent.h"
#include "TCMTMessageDispatcher.h"
#include "TCMTLockable.h"

namespace TC
{
   namespace Audio
   {
      class StreamingTask;

      class TC_DLL_LOCAL SoundDataMessage: public MT::Message
      {
      public:
         SoundDataMessage(uint32 message_id, SoundDataPtr sound_data)
            :Message(message_id),
            m_sound_data(sound_data)
         {
         }
         SoundDataPtr m_sound_data;
      };

      class TC_DLL_LOCAL StreamingThread: public MT::ThreadObject,
                                          public SoundPlayer
      {
      public:
         class TC_DLL_LOCAL Task
         {
         public:
            Task(const Time& time_out, const Time& last_processed=Time::Zero())
               :m_timeout(time_out),
               m_last_processed(last_processed)
            {
            }

            virtual ~Task() {}

            enum ReturnType
            {
               TASK_OK,
               TASK_FINISHED,
               TASK_EXECUTION_ERROR
            };

            /**
            * @brief Method which is called to execute the task
            * @return Should return if successful or not
            */
            virtual ReturnType Execute() = 0;

            const Time& GetLastExecutedTime() const { return m_last_processed;}

         private:
            /** timeout until next execution */
            Time m_timeout;
            /** time when the task was last executed */
            Time m_last_processed;

            friend class StreamingThread;
         };

         typedef SharedPtr<Task> TaskPtr;

      public:
         StreamingThread();
         virtual ~StreamingThread();

         virtual bool Run();

         bool IsRunning()
         {
            return m_running->Try();
         }

         bool WaitUntilRunning()
         {
            return m_running->Wait();
         }

         void Stop();

         virtual bool IsSoundDataPlaying(SoundDataCPtr sound_data) const;
         virtual bool IsSoundDataPaused(SoundDataCPtr sound_data) const;
         virtual bool IsSoundDataStopped(SoundDataCPtr sound_data) const;
 
         virtual bool PlaySoundData(SoundDataPtr sound_data);
         virtual bool PauseSoundData(SoundDataPtr sound_data);
         virtual bool StopSoundData(SoundDataPtr sound_data);

      private:
         TaskPtr GetNextTask(Time& next_time_out);
         bool AddTask(TaskPtr task);
         bool RemoveTask(TaskPtr task);

      private:
         MT::ThreadPtr m_thread;
         MT::EventPtr m_running;
         MT::MessageDispatcherPtr m_message_dispatcher;
         std::vector<TaskPtr> m_tasks;
         SharedPtr<StreamingTask> m_streaming_task;
      };

      typedef SharedPtr<StreamingThread> StreamingThreadPtr;
   }
}

#endif // _TC_AUDIO_STREAMING_THREAD_H_

