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

#include "TCAudioSoundDataCompound.h"

#include <cstring>

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      SoundDataCompound::SoundDataCompound()
         :m_sound_data()
      {
      }

      SoundDataCompound::~SoundDataCompound()
      {
      }

      const SoundFormat& SoundDataCompound::GetFormat() const
      {
         Locker lock(this);

         static SoundFormat s_sound_format;
         if (m_sound_data.size() == 0)
         {
            return s_sound_format;
         }

         return m_sound_data[0]->GetFormat();
      }

      uint32_t SoundDataCompound::GetNumSoundData() const
      {
         Locker lock(this);
         return uint32_t(m_sound_data.size());
      }

      SoundDataPtr SoundDataCompound::GetSoundData(uint32_t idx)
      {
         Locker lock(this);
         if (idx >= m_sound_data.size())
         {
            return SoundDataPtr();
         }

         return m_sound_data[idx];
      }

      SoundDataCPtr SoundDataCompound::GetSoundData(uint32_t idx) const
      {
         Locker lock(this);
         if (idx >= m_sound_data.size())
         {
            return SoundDataCPtr();
         }

         return m_sound_data[idx];
      }

      bool SoundDataCompound::AddSoundData(SoundDataPtr sound_data)
      {
         Locker lock(this);
         if (m_sound_data.size() > 0)
         {
            const SoundFormat& format1 = m_sound_data[0]->GetFormat();
            const SoundFormat& format2 = sound_data->GetFormat();
            if (memcmp(&format1, &format2, sizeof(SoundFormat)) != 0)
            {
               return false;
            }
         }
         m_sound_data.push_back(sound_data);
         return true;
      }

      bool SoundDataCompound::RemoveSoundData(SoundDataPtr sound_data)
      {
         Locker lock(this);
         std::vector<SoundDataPtr>::iterator it_found =
            std::find(m_sound_data.begin(), m_sound_data.end(), sound_data);
         if (it_found == m_sound_data.end())
         {
            return false;
         }

         m_sound_data.erase(it_found);
         return true;
      }
   }
}
