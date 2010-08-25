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
//  $Id: TCAudioSoundDataSequence.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCAudioSoundDataSequence.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Audio
   {
      SoundDataSequence::SoundDataSequence()
      {
      }

      SoundDataSequence::~SoundDataSequence()
      {
      }

      uint32 SoundDataSequence::GetData(uint32 num_bytes, uint8* buffer)
      {
         Locker lock(this);

         uint32 n_read = 0;
         while(num_bytes > 0 && 
               m_current_sound_data < m_sound_data.size())
         {
            uint32 c_read = m_sound_data[m_current_sound_data]->GetData(num_bytes, buffer);
            if (c_read < num_bytes)
            {
               m_sound_data[m_current_sound_data]->SetToStart();
               m_current_sound_data++;
               if (IsLooping() && 
                   m_current_sound_data >= m_sound_data.size())
               {
                  m_current_sound_data = 0;
               }
            }
            n_read    += c_read;
            num_bytes -= c_read;
         };

         return n_read;
     }

      void SoundDataSequence::SetToStart()
      {
         Locker lock(this);
         m_current_sound_data = 0;
      }
   }
}
