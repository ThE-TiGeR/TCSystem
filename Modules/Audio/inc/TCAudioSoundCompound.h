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
#ifndef _TC_AUDIO_SOUND_COMPOUND_H_
#define _TC_AUDIO_SOUND_COMPOUND_H_

#include "TCAudioSound.h"

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
      * @brief This file provides the definition of tc::audio::SoundCompound
      *
      * @author Thomas Goessler
      */

      /**
      * @brief This defines the interface for an compound sound object
      * 
      * This could be a sound sequence where one sound after the other is played
      */
      class SoundCompound: public Sound
      {
      public:
         /** @brief Get number of sounds included in this compound sound */
         virtual uint32 GetNumSounds() const = 0;
         /** @brief Get specified sound at index */
         virtual SoundPtr GetSound(uint32 idx) = 0;

         /** @brief Add a sound to this compound sound */
         virtual bool AddSound(SoundPtr sound) = 0;
         /** @brief Remove a sound from this compound sound */
         virtual bool RemoveSound(SoundPtr sound) = 0;
      };

      typedef SharedPtr<SoundCompound> SoundCompoundPtr;

      /**
      * @}
      */
   }
}
#endif // _TC_AUDIO_SOUND_COMPOUND_H_
