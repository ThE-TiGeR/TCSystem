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
#ifndef _TC_AUDIO_SOUND_IMP_H_
#define _TC_AUDIO_SOUND_IMP_H_

#include "TCAudioSound.h"
#include "TCAudioSoundData.h"
#include "TCAudioSoundPlayer.h"

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
      * @brief This file provides the definition of TC::Audio::SoundImp
      *
      * @author Thomas Goessler
      */

      /**
      * @brief Implementation of the Sound interface for a single sound
      */
      class TC_DLL_LOCAL SoundImp: public Sound
      {  
      public:
         SoundImp(SoundDataPtr sound_data, SoundPlayerPtr sound_player);
         virtual ~SoundImp();

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
      private:
         SoundDataPtr m_sound_data;
         SoundPlayerPtr m_sound_player;
      };
   }
}
#endif // _TC_AUDIO_SOUND_IMP_H_
