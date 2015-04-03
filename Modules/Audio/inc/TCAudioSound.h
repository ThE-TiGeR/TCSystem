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
//  $Id: TCAudioSound.h,v a33a555a5a40 2012/02/01 22:48:18 Thomas $
//*******************************************************************************
#ifndef _TC_AUDIO_SOUND_H_
#define _TC_AUDIO_SOUND_H_

#include "TCSharedPtr.h"
#include "TCAudioSoundFormat.h"

namespace tc
{
   namespace audio
   {
      /**
      * @addtogroup TC_AUDIO
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of tc::audio::Sound
      *
      * @author Thomas Goessler
      */

      /**
      * @brief This defines the interface for an sound object
      *
      * It provides functionality to play, stop or pause a sound
      */
      class Sound
      {
      public:
         /** @brief Get If the sound is playing */
         virtual bool IsPlaying() const = 0;
         /** @brief Get If the sound is pause */
         virtual bool IsPaused() const = 0;
         /** @brief Get If the sound is stopped */
         virtual bool IsStopped() const = 0;

         /** 
          * @brief Start playing the sound at current position of the sound
          * @return true if success
          */
         virtual bool Play() = 0;
         /**
          * @brief Pause playing the sound
          * @return true if success
          */
         virtual bool Pause() = 0;
         /** 
          * @brief Stop playing the sound
          * The sound position for next play is set to the beginning
          * @return true if success
          */
         virtual bool Stop() = 0;


         /** @brief returns true if the sound should loop when playing */
         virtual bool IsLooping() const = 0;
         /**
          * @brief Enable or disable looping of the sound
          * @parameter looping true for enable looping else disable looping
          */
         virtual void SetLooping(bool looping) = 0;

         /** @brief Get The format of this sound object */
         virtual const SoundFormat& GetFormat() const = 0;

         /**
         * @brief Get the volume amplification which is applied to the sound
         *
         * Range:  ]0.0-  ]
         * Default: 1.0
         *
         * A value of 1.0 means un-attenuated/unchanged.
         * Each division by 2 equals an attenuation of -6dB.
         * Each multiplication with 2 equals an amplification of +6dB.
         * A value of 0.0 is meaningless with respect to a logarithmic
         *  scale; it is interpreted as zero volume - the channel
         *  is effectively disabled.
         */
         virtual double GetVolume() const = 0;
         /**
         * @brief Set the volume amplification which should be applied to the sound
         * @see audio::IfSound::GetVolume
         * @return the previous volume
         */
         virtual double SetVolume(double vol) = 0;

         /**
         * @brief Get the pitch which is applied to the sound
         *
         * Range:   [0.5-2.0]
         * Default: 1.0
         */
         virtual double GetPitch() const = 0;
         /**
         * @brief Set the pitch which is applied to the sound
         * @see audio::IfSound::GetPitch
         * @return the previous pitch
         */
         virtual double SetPitch(double pitch) = 0;

         /**
         * @brief Get the pan which is applied to the sound
         *
         * Range:   [-1.0(left) - 1.0(right)]
         * Default: 0.0(center)
         */
         virtual double GetPan() const = 0;
         /**
         * @brief Set the pan which is applied to the sound
         * @see audio::IfSound::GetPan
         * @return the previous pan
         */
         virtual double SetPan(double pan) = 0;

         virtual ~Sound() {}
      };

      typedef SharedPtr<Sound> SoundPtr;

      /**
      * @}
      */
   }
}
#endif // _TC_AUDIO_SOUND_H_
