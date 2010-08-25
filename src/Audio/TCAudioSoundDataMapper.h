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
//  $Id: TCAudioSoundDataMapper.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifndef _TC_AUDIO_SOUND_DATA_MAPPER_H_
#define _TC_AUDIO_SOUND_DATA_MAPPER_H_

#include "TCAudioSoundData.h"

#include <map>

namespace TC
{
   namespace Audio
   {
      class TC_DLL_LOCAL SoundDataMapper
      {
      public:
         virtual ~SoundDataMapper() {}

         virtual void Add(SoundPtr sound, SoundDataPtr sound_data)
         {
            m_sound_data_map[sound] = sound_data;
            m_data_sound_map[sound_data] = sound;
         }
         virtual void Remove(SoundPtr sound)
         {
            std::map<SoundPtr, SoundDataPtr>::iterator it_data = m_sound_data_map.find(sound);
            if (it_data == m_sound_data_map.end())
            {
               return;
            }

            SoundDataPtr sound_data = it_data->second;
            std::map<SoundDataPtr, SoundPtr>::iterator it_sound = m_data_sound_map.find(sound_data);
            if (it_sound != m_data_sound_map.end())
            {
               m_data_sound_map.erase(it_sound);
            }

            m_sound_data_map.erase(it_data);
         }

         SoundDataPtr Get(SoundPtr sound)
         {
            return m_sound_data_map[sound];
         }

         SoundPtr Get(SoundDataPtr sound_data)
         {
            return m_data_sound_map[sound_data];
         }
      private:
         std::map<SoundPtr, SoundDataPtr> m_sound_data_map;
         std::map<SoundDataPtr, SoundPtr> m_data_sound_map;
      };

      typedef SharedPtr<SoundDataMapper> SoundDataMapperPtr;
   }
}

#endif // _TC_AUDIO_SOUND_DATA_MAPPER_H_

