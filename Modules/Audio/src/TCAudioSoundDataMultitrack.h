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
#ifndef _TC_AUDIO_SOUND_DATA_MULTITRACK_H_
#define _TC_AUDIO_SOUND_DATA_MULTITRACK_H_

#include "TCAudioSoundDataCompound.h"

#include <vector>

namespace tc
{
   namespace audio
   {
      class TC_DLL_LOCAL SoundDataMultitrack: public SoundDataCompound
      {
      public:
         SoundDataMultitrack();
         virtual ~SoundDataMultitrack();

         virtual uint64_t GetData(uint64_t num_bytes, uint8_t* buffer);
         virtual void SetToStart();

         virtual void SetLoopData(bool loop);

      private:
         typedef multi_threading::LockerPtr<const SoundDataMultitrack*> Locker;
         friend class multi_threading::LockerPtr<const SoundDataMultitrack*>;
      };

      typedef SharedPtr<SoundDataMultitrack> SoundDataMultitrackPtr;
   }
}

#endif // _TC_AUDIO_SOUND_DATA_MULTITRACK_H_

