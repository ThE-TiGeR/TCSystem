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
//  $Id: TCAudioSoundSequenceImp.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCAudioSoundSequenceImp.h"

#include "TCOutput.h"
#include "TCTime.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Audio
   {
      SoundSequenceImp::SoundSequenceImp(SoundDataSequencePtr sound_data, 
         SoundPlayerPtr sound_player, SoundDataMapperPtr sound_data_mapper)
         :m_sound_data(sound_data),
         m_sound_player(sound_player),
         m_sound_data_mapper(sound_data_mapper)
      {
      }

      SoundSequenceImp::~SoundSequenceImp()
      {
      }

      bool SoundSequenceImp::IsPlaying() const
      {
         return m_sound_player->IsSoundDataPlaying(m_sound_data);
      }

      bool SoundSequenceImp::IsPaused() const
      {
         return m_sound_player->IsSoundDataStopped(m_sound_data);
      }

      bool SoundSequenceImp::IsStopped() const
      {
         return m_sound_player->IsSoundDataStopped(m_sound_data);
      }

      bool SoundSequenceImp::Play()
      {
         return m_sound_player->PlaySoundData(m_sound_data);
      }

      bool SoundSequenceImp::Pause()
      {
         return m_sound_player->PauseSoundData(m_sound_data);
      }

      bool SoundSequenceImp::Stop()
      {
         return m_sound_player->StopSoundData(m_sound_data);
      }

      void SoundSequenceImp::SetLooping(bool looping)
      {
         m_sound_data->SetLoopData(looping);
      }

      bool SoundSequenceImp::IsLooping() const
      {
         return m_sound_data->IsLooping();
      }

      const SoundFormat& SoundSequenceImp::GetFormat() const
      {
         return m_sound_data->GetFormat();
      }

      double SoundSequenceImp::GetVolume() const
      {
         return m_sound_data->GetVolume();
      }

      double SoundSequenceImp::SetVolume(double vol)
      {
         return m_sound_data->SetVolume(vol);

      }

      double SoundSequenceImp::GetPitch() const
      {
         return m_sound_data->GetPitch();
      }

      double SoundSequenceImp::SetPitch(double pitch)
      {
         return m_sound_data->SetPitch(pitch);
      }

      double SoundSequenceImp::GetPan() const
      {
         return m_sound_data->GetPan();
      }

      double SoundSequenceImp::SetPan(double pan)
      {
         return m_sound_data->SetPan(pan);
      }

      uint32 SoundSequenceImp::GetNumSounds() const
      {
         return m_sound_data->GetNumSoundData();
      }

      SoundPtr SoundSequenceImp::GetSound(uint32 idx)
      {
         SoundDataPtr data = m_sound_data->GetSoundData(idx);
         if (!data)
         {
            return SoundPtr();
         }
         return m_sound_data_mapper->Get(data);
      }

      bool SoundSequenceImp::AddSound(SoundPtr sound)
      {
         SoundDataPtr sound_data = m_sound_data_mapper->Get(sound);
         if (!sound_data)
         {
            return false;
         }
         m_sound_data->AddSoundData(sound_data);
         return true;
      }

      bool SoundSequenceImp::RemoveSound(SoundPtr sound)
      {
         SoundDataPtr sound_data = m_sound_data_mapper->Get(sound);
         if (!sound_data)
         {
            return false;
         }
         m_sound_data->RemoveSoundData(sound_data);
         return true;
      }
   }
}
