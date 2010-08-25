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
//  $Id: TCAudioSoundDataWav.h 920 2009-03-25 13:16:41Z the_____tiger $
//*******************************************************************************
#ifndef _TC_AUDIO_SOUND_DATA_MP3_H_
#define _TC_AUDIO_SOUND_DATA_MP3_H_

#include "TCAudioSoundData.h"
#include "TCMTLockable.h"
#include "TCStream.h"

#include <mpg123.h>

namespace TC
{
   namespace Audio
   {
      class TC_DLL_LOCAL SoundDataMp3: public SoundData,
                                       protected MT::ObjectLevelLockable<SoundDataMp3>
      {
      public:
         SoundDataMp3(StreamPtr stream);
         virtual ~SoundDataMp3();

         virtual const SoundFormat& GetFormat() const;
         virtual uint32 GetData(uint32 num_bytes, uint8* buffer);
         virtual void SetToStart();

      private:
         static ssize_t Mp3Read(void* h, void *buf, size_t cnt);
         static off_t Mp3Seek(void* h, off_t of, int origin);
         static void Mp3Cleanup(void* h);

      private:
         SoundFormat m_sound_format;
         StreamPtr m_stream;
         mpg123_handle* m_mp3_handle;

         typedef MT::LockerPtr<const SoundDataMp3*> Locker;
         friend class MT::LockerPtr<const SoundDataMp3*>;
      };
   }
}

#endif // _TC_AUDIO_SOUND_DATA_WAV_H_

