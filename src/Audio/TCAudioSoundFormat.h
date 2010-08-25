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
//  $Id: TCAudioSoundFormat.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifndef _TC_AUDIO_SOUND_FORMAT_H_
#define _TC_AUDIO_SOUND_FORMAT_H_

#include "TCSharedPtr.h"

namespace TC
{
   namespace Audio
   {
      /**
      * @addtogroup TC_AUDIO
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of TC::Audio::SoundFormat
      *
      * @author Thomas Goessler
      */

      /** class holding audio format information */
      class SoundFormat
      {
      public:
         uint16 audio_format;       ///< 1 = Uncompressed (PCM), > 1: Compressed
         uint16 num_channels;       ///< Number of channels, can be any number
         uint32 samples_per_second; ///< Samples per second (Hertz)
         uint32 bytes_per_second;   ///< bytes per second 
         uint16 bytes_per_sample;   ///< channels * bits/sample / 8
         uint16 bits_per_sample;    ///< 8, 16 or 24
      };

      /**
      * @}
      */
   }
}
#endif // _TC_AUDIO_SOUND_FORMAT_H_
