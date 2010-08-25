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
//  $Id: TCAudioSoundMultitrackImp.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifndef _TC_AUDIO_SOUND_MULTITRACK_IMP_H_
#define _TC_AUDIO_SOUND_MULTITRACK_IMP_H_

#include "TCAudioSoundCompound.h"
#include "TCAudioSoundDataMultitrack.h"
#include "TCAudioSoundDataMapper.h"
#include "TCAudioSoundPlayer.h"

#include <vector>

namespace TC
{
   namespace Audio
   {
      /**
      * @addtogroup TC_AUDIO_IMP
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of TC::Audio::SoundMultitrackImp
      *
      * @author Thomas Goessler
      */

      /**
      * @brief Implementation of the Sound interface for a sound sequence
      */
      class TC_DLL_LOCAL SoundMultitrackImp: public SoundCompound
      {  
      public:
         SoundMultitrackImp(SoundDataMultitrackPtr sound_data, 
                          SoundPlayerPtr sound_player,
                          SoundDataMapperPtr sound_data_mapper);
         virtual ~SoundMultitrackImp();

         virtual bool IsPlaying() const;
         virtual bool IsPaused() const;
         virtual bool IsStopped() const;

         virtual bool Play();
         virtual bool Pause();
         virtual bool Stop();

         virtual bool IsLooping() const;
         virtual void SetLooping(bool looping);

         virtual const SoundFormat& GetFormat() const;

         virtual double GetVolume() const;
         virtual double SetVolume(double vol);

         virtual double GetPitch() const;
         virtual double SetPitch(double pitch);

         virtual double GetPan() const;
         virtual double SetPan(double pan);      

         virtual uint32 GetNumSounds() const;
         virtual SoundPtr GetSound(uint32 idx);

         virtual bool AddSound(SoundPtr sound);
         virtual bool RemoveSound(SoundPtr sound);

      private:
        SoundDataMultitrackPtr m_sound_data;
        SoundPlayerPtr m_sound_player;
        SoundDataMapperPtr m_sound_data_mapper;
      };
   }
}
#endif // _TC_AUDIO_SOUND_MULTITRACK_IMP_H_
