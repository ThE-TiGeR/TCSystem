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
//  $Id: TCAudioManager.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifndef _TC_AUDIO_MANAGER_H_
#define _TC_AUDIO_MANAGER_H_

#include "TCAudioApi.h"
#include "TCAudioSoundCompound.h"
#include "TCStream.h"

namespace TC
{
   namespace Audio
   {
      class Manager;
      typedef SharedPtr<Manager> ManagerPtr;

      class Manager
      {
      public:
         virtual ~Manager() {}

         virtual SoundPtr CreateSound(const std::string& file_name) = 0;
         virtual SoundPtr CreateSound(const std::string& name, StreamPtr stream) = 0;
         virtual SoundCompoundPtr CreateSoundSequence(const std::string& name) = 0;
         virtual SoundCompoundPtr CreateSoundMultitrack(const std::string& name) = 0;

         virtual void DeleteSound(SoundPtr& sound) = 0;
         virtual void DeleteSound(SoundCompoundPtr& sound) = 0;

         static TCAUDIO_API ManagerPtr Create();
      };
   }
}

#endif // _TC_AUDIO_MANAGER_H_
