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
#ifndef _TC_AUDIO_SOUND_DATA_H_
#define _TC_AUDIO_SOUND_DATA_H_

#include "TCAudioSound.h"

namespace tc
{
   namespace audio
   {
      class TC_DLL_LOCAL SoundData
      {
      public:
         SoundData()
            :m_loop_data(false),
            m_volume(1.0),
            m_pitch(1.0),
            m_pan(0.0)
         {
         }

         virtual const SoundFormat& GetFormat() const = 0;
         virtual uint64 GetData(uint64 num_bytes, uint8* buffer) = 0;
         virtual void SetToStart() = 0;

         virtual bool IsLooping() const { return m_loop_data; }
         virtual void SetLoopData(bool loop) { m_loop_data = loop; }

         double GetVolume() const
         {
            return m_volume;
         }

         double SetVolume(double vol)
         {
            std::swap(m_volume, vol);
            return vol;
         }

         double GetPitch() const
         {
            return m_pitch;
         }

         double SetPitch(double pitch)
         {
            std::swap(m_pitch, pitch);
            return pitch;
         }

         double GetPan() const
         {
            return m_pan;
         }

         double SetPan(double pan)
         {
            std::swap(m_pan, pan);
            return pan;
         }

         virtual ~SoundData() {}
      private:
         bool m_loop_data;
         double m_volume;
         double m_pitch;
         double m_pan;
      };

      typedef SharedPtr<SoundData> SoundDataPtr;
      typedef SharedPtr<const SoundData> SoundDataCPtr;
   }
}

#endif // _TC_AUDIO_SOUND_DATA_H_

