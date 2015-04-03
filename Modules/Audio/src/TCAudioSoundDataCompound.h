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
//  $Id: TCAudioSoundDataCompound.h,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************
#ifndef _TC_AUDIO_SOUND_DATA_COMPOUND_H_
#define _TC_AUDIO_SOUND_DATA_COMPOUND_H_

#include "TCAudioSoundData.h"
#include "TCMTLockable.h"

#include <vector>

namespace tc
{
   namespace audio
   {
      class TC_DLL_LOCAL SoundDataCompound: public SoundData,
                                            protected multi_threading::ObjectLevelLockable<SoundDataCompound>
      {
      public:
         SoundDataCompound();
         virtual ~SoundDataCompound();

         virtual const SoundFormat& GetFormat() const;

         uint32_t GetNumSoundData() const;
         SoundDataPtr GetSoundData(uint32_t idx);
         SoundDataCPtr GetSoundData(uint32_t idx) const;
         bool AddSoundData(SoundDataPtr sound_data);
         bool RemoveSoundData(SoundDataPtr sound_data);

      protected:
         std::vector<SoundDataPtr> m_sound_data;

         typedef multi_threading::LockerPtr<const SoundDataCompound*> Locker;
         friend class multi_threading::LockerPtr<const SoundDataCompound*>;
      };

      typedef SharedPtr<SoundDataCompound> SoundDataCompoundPtr;
   }
}

#endif // _TC_AUDIO_SOUND_DATA_COMPOUND_H_

