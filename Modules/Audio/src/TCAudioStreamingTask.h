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
#ifndef _TC_AUDIO_STREAMING_TASK_H_
#define _TC_AUDIO_STREAMING_TASK_H_

#include "TCAudioStreamingThread.h"

namespace TC
{
   namespace Audio
   {
      class TC_DLL_LOCAL StreamingTask: public StreamingThread::Task
      {
      public:
         StreamingTask();
         virtual ~StreamingTask();

         virtual ReturnType Execute();

         bool IsSoundDataPlaying(SoundDataCPtr sound_data) const;
         bool IsSoundDataPaused(SoundDataCPtr sound_data) const;
         bool IsSoundDataStopped(SoundDataCPtr sound_data) const;

         bool OnPlayMessage(SharedPtr<SoundDataMessage> message_in);
         bool OnPauseMessage(SharedPtr<SoundDataMessage> message_in);
         bool OnStopMessage(SharedPtr<SoundDataMessage> message_in);

      private:
         struct StreamingSource
         {
            SoundDataPtr m_sound_data;
            ALuint m_buffer[4];
            ALuint m_source;
         };
         typedef SharedPtr<StreamingSource> StreamingSourcePtr;
         class CompareSoundData2StreamingSource;

         StreamingSourcePtr CreateStreamingSource(SoundDataPtr sound_data);
         StreamingSourcePtr FindStreamingSource(SoundDataPtr sound_data);
         void ReleaseStreamingSource(StreamingSourcePtr& source);
         std::vector<StreamingSourcePtr>::iterator ReleaseStreamingSource(
                        std::vector<StreamingSourcePtr>::iterator streaming_source);

         void ProcessStreamingSources();
         void ProcessStreamingSource(StreamingSourcePtr source);
         void UpdateStreamingSourceSettings(StreamingSourcePtr streaming_source);
         bool CheckSourceStatus(StreamingSourcePtr streaming_source);
         void QueueBuffer(StreamingSourcePtr source, ALuint buffer_to_queue);

         void UpdateBufferSize(const SoundFormat& format);
      private:
         std::vector<StreamingSourcePtr> m_streaming_sources;

         uint8* m_streaming_buffer;
         uint32 m_streaming_buffer_size;

         uint32 m_total_num_processed_buffers;
      };

      typedef SharedPtr<StreamingThread> StreamingThreadPtr;
   }
}

#endif // _TC_AUDIO_STREAMING_TASK_H_

