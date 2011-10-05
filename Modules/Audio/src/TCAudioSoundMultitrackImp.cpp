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

#include "TCAudioSoundMultitrackImp.h"

#include "TCOutput.h"
#include "TCTime.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Audio
   {
      SoundMultitrackImp::SoundMultitrackImp(SoundDataMultitrackPtr sound_data, 
         SoundPlayerPtr sound_player, SoundDataMapperPtr sound_data_mapper)
         :m_sound_data(sound_data),
         m_sound_player(sound_player),
         m_sound_data_mapper(sound_data_mapper)
      {
      }

      SoundMultitrackImp::~SoundMultitrackImp()
      {
      }

      bool SoundMultitrackImp::Play()
      {
         for (uint32 i=0; i<m_sound_data->GetNumSoundData(); i++)
         {
            if (!m_sound_player->PlaySoundData(m_sound_data->GetSoundData(i)))
            {
               return false;
            }
         }
         return true;
      }

      bool SoundMultitrackImp::Pause()
      {
         for (uint32 i=0; i<m_sound_data->GetNumSoundData(); i++)
         {
            if (!m_sound_player->PauseSoundData(m_sound_data->GetSoundData(i)))
            {
               return false;
            }
         }
         return true;
      }

      bool SoundMultitrackImp::Stop()
      {
         for (uint32 i=0; i<m_sound_data->GetNumSoundData(); i++)
         {
            if (!m_sound_player->StopSoundData(m_sound_data->GetSoundData(i)))
            {
               return false;
            }
         }
         return true;
      }

      void SoundMultitrackImp::SetLooping(bool looping)
      {
         m_sound_data->SetLoopData(looping);
      }

      bool SoundMultitrackImp::IsLooping() const
      {
         return m_sound_data->IsLooping();
      }

      bool SoundMultitrackImp::IsPlaying() const
      {
         for (uint32 i=0; i<m_sound_data->GetNumSoundData(); i++)
         {
            if (m_sound_player->IsSoundDataPlaying(m_sound_data->GetSoundData(i)))
            {
               return true;
            }
         }

         return false;
      }

      bool SoundMultitrackImp::IsPaused() const
      {
         for (uint32 i=0; i<m_sound_data->GetNumSoundData(); i++)
         {
            if (m_sound_player->IsSoundDataPaused(m_sound_data->GetSoundData(i)))
            {
               return true;
            }
         }

         return false;
      }

      bool SoundMultitrackImp::IsStopped() const
      {
         return !IsPlaying();
      }

      const SoundFormat& SoundMultitrackImp::GetFormat() const
      {
         return m_sound_data->GetFormat();
      }

      double SoundMultitrackImp::GetVolume() const
      {
         return m_sound_data->GetVolume();
      }

      double SoundMultitrackImp::SetVolume(double vol)
      {
         return m_sound_data->SetVolume(vol);

      }

      double SoundMultitrackImp::GetPitch() const
      {
         return m_sound_data->GetPitch();
      }

      double SoundMultitrackImp::SetPitch(double pitch)
      {
         return m_sound_data->SetPitch(pitch);
      }

      double SoundMultitrackImp::GetPan() const
      {
         return m_sound_data->GetPan();
      }

      double SoundMultitrackImp::SetPan(double pan)
      {
         return m_sound_data->SetPan(pan);
      }

      uint32 SoundMultitrackImp::GetNumSounds() const
      {
         return m_sound_data->GetNumSoundData();
      }

      SoundPtr SoundMultitrackImp::GetSound(uint32 idx)
      {
         SoundDataPtr data = m_sound_data->GetSoundData(idx);
         if (!data)
         {
            return SoundPtr();
         }
         return m_sound_data_mapper->Get(data);
      }

      bool SoundMultitrackImp::AddSound(SoundPtr sound)
      {
         SoundDataPtr sound_data = m_sound_data_mapper->Get(sound);
         if (!sound_data)
         {
            return false;
         }
         m_sound_data->AddSoundData(sound_data);
         return true;
      }

      bool SoundMultitrackImp::RemoveSound(SoundPtr sound)
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
