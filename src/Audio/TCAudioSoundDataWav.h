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
#ifndef _TC_AUDIO_SOUND_DATA_WAV_H_
#define _TC_AUDIO_SOUND_DATA_WAV_H_

#include "TCAudioSoundData.h"
#include "TCMTLockable.h"
#include "TCStream.h"

namespace TC
{
   namespace Audio
   {
      class TC_DLL_LOCAL SoundDataWav: public SoundData,
                                       protected MT::ObjectLevelLockable<SoundDataWav>
      {
      public:
         SoundDataWav(StreamPtr stream);
         virtual ~SoundDataWav();

         virtual const SoundFormat& GetFormat() const;
         virtual uint64 GetData(uint64 num_bytes, uint8* buffer);
         virtual void SetToStart();

      private:
         void ProcessStream();
         void ProcessFormat(uint32 chunk_length);
         void ProcessData(uint32 chunk_length);

      private:
         SoundFormat m_sound_format;
         StreamPtr m_stream;
         uint64 m_start_of_sound_data;

         typedef MT::LockerPtr<const SoundDataWav*> Locker;
         friend class MT::LockerPtr<const SoundDataWav*>;
      };
   }
}

#endif // _TC_AUDIO_SOUND_DATA_WAV_H_

