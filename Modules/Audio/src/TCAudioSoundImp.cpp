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

#include "TCAudioSoundImp.h"

#include "TCOutput.h"
#include "TCTime.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      class ScopedTimeMeassure
      {
      public:
         ScopedTimeMeassure(const char* trace_text)
            :m_start_time(Time::Now()),
            m_trace_text(trace_text)
         {
         }

         ~ScopedTimeMeassure()
         {
            TCTRACES("AUDIO", 3, m_trace_text << " " << Time::Since(m_start_time).ToMilliSeconds());
         }

      private:
         Time m_start_time;
         const char* m_trace_text;
      };

      SoundImp::SoundImp(SoundDataPtr sound_data, SoundPlayerPtr sound_player)
         :m_sound_data(sound_data),
         m_sound_player(sound_player)
      {

      }

      SoundImp::~SoundImp()
      {

      }

      bool SoundImp::Play()
      {
         ScopedTimeMeassure meassure("Time used for start play sound");
         return m_sound_player->PlaySoundData(m_sound_data);
      }

      bool SoundImp::Pause()
      {
         ScopedTimeMeassure meassure("Time used for pause play sound");
         return m_sound_player->PauseSoundData(m_sound_data);
      }

      bool SoundImp::Stop()
      {
         if (m_sound_player->IsSoundDataPlaying(m_sound_data))
         {
            ScopedTimeMeassure meassure("Time used for stop play sound");
            return m_sound_player->StopSoundData(m_sound_data);
         }
         return true;
      }

      void SoundImp::SetLooping(bool looping)
      {
         m_sound_data->SetLoopData(looping);
      }

      bool SoundImp::IsLooping() const
      {
         return m_sound_data->IsLooping();
      }

      bool SoundImp::IsPlaying() const
      {
         return m_sound_player->IsSoundDataPlaying(m_sound_data);
      }

      bool SoundImp::IsPaused() const
      {
         return m_sound_player->IsSoundDataStopped(m_sound_data);
      }

      bool SoundImp::IsStopped() const
      {
         return m_sound_player->IsSoundDataStopped(m_sound_data);
      }

      const SoundFormat& SoundImp::GetFormat() const
      {
         return m_sound_data->GetFormat();
      }

      double SoundImp::GetVolume() const
      {
         return m_sound_data->GetVolume();
      }

      double SoundImp::SetVolume(double vol)
      {
         return m_sound_data->SetVolume(vol);

      }

      double SoundImp::GetPitch() const
      {
         return m_sound_data->GetPitch();
      }

      double SoundImp::SetPitch(double pitch)
      {
         return m_sound_data->SetPitch(pitch);
      }

      double SoundImp::GetPan() const
      {
         return m_sound_data->GetPan();
      }

      double SoundImp::SetPan(double pan)
      {
         return m_sound_data->SetPan(pan);
      }
   }
}
