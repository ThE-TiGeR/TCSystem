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
#ifndef _TC_AUDIO_SOUND_FORMAT_H_
#define _TC_AUDIO_SOUND_FORMAT_H_

#include "TCSharedPtr.h"

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
      * @brief This file provides the definition of tc::audio::SoundFormat
      *
      * @author Thomas Goessler
      */

      /** class holding audio format information */
      class SoundFormat
      {
      public:
         SoundFormat()
            :m_num_channels(0),
            m_samples_per_second(0),
            m_bits_per_sample(0)
         {
         }

         bool operator==(const SoundFormat& other) const
         {
            return (
                  (m_num_channels == other.m_num_channels)
               && (m_samples_per_second == other.m_samples_per_second)
               && (m_bits_per_sample == other.m_bits_per_sample)
               );
         }

         bool operator!=(const SoundFormat& other) const
         {
            return (!this->operator==(other));
         }

         bool IsValid()
         {
            return ( m_num_channels
               && m_samples_per_second
               && m_bits_per_sample);
         }

         void SetNumChannels(uint16_t num_channels)
         {
            m_num_channels = num_channels;
         }
         uint16_t GetNumChannels() const
         {
            return m_num_channels;
         }

         void  SetSamplesPerSecond(uint32_t samples_per_second)
         {
            m_samples_per_second = samples_per_second;
         }
         uint32_t GetSamplesPerSecond() const
         {
            return m_samples_per_second;
         }

         uint32_t GetBytesPerSecond() const
         {
            return (m_samples_per_second * m_num_channels *  m_bits_per_sample / 8);
         }

         uint16_t GetBytesPerSample() const
         {
            return (m_num_channels * m_bits_per_sample / 8);
         }

         void SetBitsPerSample(uint16_t bits_per_sample)
         {
            m_bits_per_sample = bits_per_sample;
         }

         uint16_t GetBitsPerSample() const
         {
            return m_bits_per_sample;
         }

      private:
         uint16_t m_num_channels;       ///< Number of channels, can be any number
         uint32_t m_samples_per_second; ///< Samples per second (Hertz)
         uint16_t m_bits_per_sample;    ///< 8, 16 or 24

      }; // end of class SoundFormat   

      /**
      * @}
      */
   }
}
#endif // _TC_AUDIO_SOUND_FORMAT_H_
