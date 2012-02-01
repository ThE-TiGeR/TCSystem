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

#include "TCAudioStreamingTask.h"

#include "TCAssert.h"
#include "TCAudioOpenALHandler.h"
#include "TCAudioUtil.h"
#include "TCException.h"
#include "TCMTFactory.h"
#include "TCMTMessage.h"
#include "TCOutput.h"
#include "TCTime.h"
#include "TCUtil.h"

#include <cstring>

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      const Time STREAMING_TIMEOUT(0, 50 * Time::ONE_MILLI_SECOND_AS_NANO_SECONDS);
      const Time RESPONSE_TIMEOUT(0, 200 * Time::ONE_MILLI_SECOND_AS_NANO_SECONDS);

      StreamingTask::StreamingTask()
         :Task(STREAMING_TIMEOUT),
         m_streaming_sources(),
         m_streaming_buffer(0),
         m_streaming_buffer_size(0),
         m_total_num_processed_buffers(0)
      {
      }

      StreamingTask::~StreamingTask()
      {
         delete []m_streaming_buffer;
      }

      StreamingThread::Task::ReturnType StreamingTask::Execute()
      {
#ifdef TCTRACE_ENABLE
         Time start_time = Time::Now();
         uint32 old_buffers = m_total_num_processed_buffers;
#endif
         ProcessStreamingSources();
#ifdef TCTRACE_ENABLE
         uint64 ellapsed = (Time::Now() - start_time).ToMilliSeconds();
         TCTRACE3("AUDIO", 10, "Sounds = %ld, Buffers = %3ld, Time = %3" TC_UINT64_FORMAT,
            m_streaming_sources.size(), m_total_num_processed_buffers-old_buffers, ellapsed);
#endif

         return TASK_OK;
      }

      bool StreamingTask::OnPlayMessage(SharedPtr<SoundDataMessage> message)
      {
         StreamingSourcePtr streaming_source = FindStreamingSource(message->m_sound_data);
         if (!streaming_source)
         {
            streaming_source = CreateStreamingSource(message->m_sound_data);
            if (!streaming_source->m_sound_data)
            {
               return false;
            }

            streaming_source->m_sound_data->SetToStart();

            // update buffer size for this format
            UpdateBufferSize(streaming_source->m_sound_data->GetFormat());
            // Fill all the Buffers with audio data from the wavefile
            for (uint32 iLoop = 0; iLoop < tc::util::ArraySize(streaming_source->m_buffer); iLoop++)
            {
               QueueBuffer(streaming_source, streaming_source->m_buffer[iLoop]);
            }

            m_streaming_sources.push_back(streaming_source);
         }

         UpdateStreamingSourceSettings(streaming_source);

         // Start playing source
         ::alSourcePlay(streaming_source->m_source);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourcePlay");

         return true;
      }

      bool StreamingTask::OnPauseMessage(SharedPtr<SoundDataMessage> message)
      {
         StreamingSourcePtr streaming_source = FindStreamingSource(message->m_sound_data);
         if (!streaming_source)
         {
            return false;
         }

         ::alSourcePause(streaming_source->m_source);
         OpenALHandler::GetInstance()->GetInstance()->CheckErrorAndThrowException("alGetSourcei");

         return true;
      }

      bool StreamingTask::OnStopMessage(SharedPtr<SoundDataMessage> message)
      {
         StreamingSourcePtr streaming_source = FindStreamingSource(message->m_sound_data);
         if (!streaming_source)
         {
            return false;
         }

         ReleaseStreamingSource(streaming_source);

         return true;
      }

      void StreamingTask::ProcessStreamingSources()
      {
         for (std::vector<StreamingSourcePtr>::iterator source=m_streaming_sources.begin();
            source != m_streaming_sources.end();)
         {
            ProcessStreamingSource(*source);
            if (!CheckSourceStatus(*source))
            {
               source = ReleaseStreamingSource(source);
            }
            else
            {
               source++;
            }
         }
      }

      void StreamingTask::ProcessStreamingSource(StreamingSourcePtr streaming_source)
      {
         UpdateStreamingSourceSettings(streaming_source);

         // Request the number of OpenAL Buffers have been processed (played) on the Source
         ALint num_buffers_processed = 0;
         ::alGetSourcei(streaming_source->m_source, AL_BUFFERS_PROCESSED, &num_buffers_processed);
         OpenALHandler::GetInstance()->GetInstance()->CheckErrorAndThrowException("alGetSourcei");

         // Keep a running count of number of buffers processed (for logging purposes only)
         m_total_num_processed_buffers += num_buffers_processed;

         // For each processed buffer, remove it from the Source Queue, read next chunk of audio
         // data from disk, fill buffer with new data, and add it to the Source Queue
         while (num_buffers_processed)
         {
            // Remove the Buffer from the Queue.  (unqueued_buffer contains the Buffer ID for the unqueued Buffer)
            ALuint unqueued_buffer = 0;
            ::alSourceUnqueueBuffers(streaming_source->m_source, 1, &unqueued_buffer);
            OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourceUnqueueBuffers");

            QueueBuffer(streaming_source, unqueued_buffer);
            num_buffers_processed--;
         }
      }

      void StreamingTask::UpdateStreamingSourceSettings(StreamingSourcePtr streaming_source)
      {
         // update volume
         ::alSourcef(streaming_source->m_source,AL_GAIN, 
            static_cast<ALfloat>(streaming_source->m_sound_data->GetVolume()));
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourcePlay");

         // update pitch
         ::alSourcef(streaming_source->m_source,AL_PITCH, 
            static_cast<ALfloat>(streaming_source->m_sound_data->GetPitch()));
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourcePlay");

         // update position (pan)
         ::alSource3f(streaming_source->m_source,AL_POSITION, 
            static_cast<ALfloat>(streaming_source->m_sound_data->GetPan()), 0, 0);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourcePlay");
      }

      bool StreamingTask::CheckSourceStatus(StreamingSourcePtr streaming_source)
      {
         // Check the status of the Source.  If it is not playing, then playback was completed,
         // or the Source was starved of audio data, and needs to be restarted.
         ALint state;
         ::alGetSourcei(streaming_source->m_source, AL_SOURCE_STATE, &state);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alGetSourcei");
         if (state != AL_PLAYING && state != AL_PAUSED)
         {
            // If there are Buffers in the Source Queue then the Source was starved of audio
            // data, so needs to be restarted (because there is more audio data to play)
            ALint num_queued_buffers;
            ::alGetSourcei(streaming_source->m_source, AL_BUFFERS_QUEUED, &num_queued_buffers);
            OpenALHandler::GetInstance()->CheckErrorAndThrowException("alGetSourcei");
            if (num_queued_buffers)
            {
               ::alSourcePlay(streaming_source->m_source);
               OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourcePlay");

               return true;
            }
            else
            {
               return false;
            }
         }

         return true;
      }

      void StreamingTask::QueueBuffer(StreamingSourcePtr streaming_source, ALuint buffer_to_queue)
      {
         const SoundFormat& format(streaming_source->m_sound_data->GetFormat());

         // Read more audio data (if there is any)
         uint64 bytes_in_buffer = 
            streaming_source->m_sound_data->GetData(m_streaming_buffer_size, m_streaming_buffer);
         // end if sound data
         if (bytes_in_buffer == 0)
         {
            return;
         }

         // if bytes in buffer are not a multiple of for fill them with 0
         // this can only happen at the end of a sound data
         if (bytes_in_buffer % 4 != 0)
         {
            ::memset(m_streaming_buffer+bytes_in_buffer, 0, bytes_in_buffer % 4);
            bytes_in_buffer += bytes_in_buffer % 4;
         }

         ::alBufferData(buffer_to_queue, util::SoundFormat2BufferFormat(format),
            m_streaming_buffer, ALsizei(bytes_in_buffer), format.samples_per_second);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alBufferData");

         ::alSourceQueueBuffers(streaming_source->m_source, 1, &buffer_to_queue);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourceQueueBuffers");
      }

      StreamingTask::StreamingSourcePtr StreamingTask::CreateStreamingSource(SoundDataPtr sound_data)
      {
         StreamingSourcePtr streaming_source(new StreamingSource);
         streaming_source->m_source = OpenALHandler::GetInstance()->GetUnusedSource();
         for (uint32 i=0; i<tc::util::ArraySize(streaming_source->m_buffer); i++)
         {
            streaming_source->m_buffer[i] = OpenALHandler::GetInstance()->GetUnusedBuffer();
         }

         streaming_source->m_sound_data = sound_data;
         return streaming_source;
      }

      void StreamingTask::ReleaseStreamingSource(StreamingSourcePtr& streaming_source)
      {
         std::vector<StreamingSourcePtr>::iterator it = 
            std::find(m_streaming_sources.begin(), m_streaming_sources.end(), streaming_source);
         if (it != m_streaming_sources.end())
         {
            ReleaseStreamingSource(it);
            streaming_source = StreamingSourcePtr();
         }
      }

      std::vector<StreamingTask::StreamingSourcePtr>::iterator 
         StreamingTask::ReleaseStreamingSource(
         std::vector<StreamingSourcePtr>::iterator streaming_source)
      {
         // Finished playing
         // Stop the Source and clear the Queue
         ::alSourceStop((*streaming_source)->m_source);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourceStop");

         ::alSourcei((*streaming_source)->m_source, AL_BUFFER, 0);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alSourcei");

         OpenALHandler::GetInstance()->ReleaseSource((*streaming_source)->m_source);
         for (uint32 i=0; i<tc::util::ArraySize((*streaming_source)->m_buffer); i++)
         {
            OpenALHandler::GetInstance()->ReleaseBuffer((*streaming_source)->m_buffer[i]);
         }

         return m_streaming_sources.erase(streaming_source);
      }

      StreamingTask::StreamingSourcePtr StreamingTask::FindStreamingSource(SoundDataPtr sound_data)
      {
         for (std::vector<StreamingSourcePtr>::iterator it=m_streaming_sources.begin();
            it != m_streaming_sources.end(); it++)
         {
            if ((*it)->m_sound_data == sound_data)
            {
               return *it;
            }
         }

         return StreamingSourcePtr();
      }

      void StreamingTask::UpdateBufferSize(const SoundFormat& format)
      {
         //uint32 buffer_size = format.bytes_per_second >> 4;
         uint32 buffer_size = format.bytes_per_second >> 2;
         // we always force a multiple of 4
         // IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
         buffer_size -= (buffer_size % tc::util::Max(format.bytes_per_sample, uint16(4)));

         //tcout << double(buffer_size)/format.bytes_per_second << "block time" << endl;

         if (m_streaming_buffer_size < buffer_size)
         {
            delete []m_streaming_buffer;
            m_streaming_buffer = new uint8[buffer_size];
            if (!m_streaming_buffer)
            {
               throw Exception("StreamingTask::UpdateBufferSize allocation failed");
            }
            m_streaming_buffer_size = buffer_size;
         }
      }

      class StreamingTask::CompareSoundData2StreamingSource
      {
      public:
         explicit CompareSoundData2StreamingSource(SoundDataCPtr sound_data)
            :m_sound_data(sound_data)
         {

         }
         bool operator()(StreamingTask::StreamingSourcePtr streaming_source)
         {
            return streaming_source->m_sound_data == m_sound_data;
         }

      private:
         SoundDataCPtr m_sound_data;
      };

      bool StreamingTask::IsSoundDataPlaying(SoundDataCPtr sound_data) const
      {
         std::vector<StreamingSourcePtr>::const_iterator found =
            std::find_if(m_streaming_sources.begin(), m_streaming_sources.end(), CompareSoundData2StreamingSource(sound_data));
         if (found == m_streaming_sources.end())
         {
            return false;
         }

         ALint state;
         ::alGetSourcei((*found)->m_source, AL_SOURCE_STATE, &state);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alGetSourcei");
         return state == AL_PLAYING;
      }

      bool StreamingTask::IsSoundDataPaused(SoundDataCPtr sound_data) const
      {
         std::vector<StreamingSourcePtr>::const_iterator found =
            std::find_if(m_streaming_sources.begin(), m_streaming_sources.end(), CompareSoundData2StreamingSource(sound_data));
         if (found == m_streaming_sources.end())
         {
            return false;
         }

         ALint state;
         ::alGetSourcei((*found)->m_source, AL_SOURCE_STATE, &state);
         OpenALHandler::GetInstance()->CheckErrorAndThrowException("alGetSourcei");
         return state == AL_PAUSED;
      }

      bool StreamingTask::IsSoundDataStopped(SoundDataCPtr sound_data) const
      {
         std::vector<StreamingSourcePtr>::const_iterator found =
            std::find_if(m_streaming_sources.begin(), m_streaming_sources.end(), CompareSoundData2StreamingSource(sound_data));
         if (found == m_streaming_sources.end())
         {
            return true;
         }

         return false;
      }
   }
}
