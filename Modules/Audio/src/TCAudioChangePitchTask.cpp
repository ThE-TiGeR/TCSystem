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
//  $Id: TCAudioChangePitchTask.cpp,v a33a555a5a40 2012/02/01 22:48:18 Thomas $
//*******************************************************************************

#include "TCAudioChangePitchTask.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      ChangePitchTask::ChangePitchTask(SoundDataPtr sound_data, 
         double start_pitch, double end_pitch, const Time& duration)
         :ChangeSoundValueTask(sound_data, start_pitch, end_pitch, duration)
      {
         ApplyNewValueOnSoundData(sound_data, start_pitch);
      }

      void ChangePitchTask::ApplyNewValueOnSoundData(SoundDataPtr sound_data, double new_pitch)
      {
         sound_data->SetPitch(new_pitch);
      }
   } // end namespace audio
} // end namespace tc
