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
//  $Id: TCAudioChangePanTask.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCAudioChangePanTask.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Audio
   {
      ChangePanTask::ChangePanTask(SoundDataPtr sound_data, 
         double start_pan, double end_pan, const Time& duration)
         :ChangeSoundValueTask(sound_data, start_pan, end_pan, duration)
      {
         ApplyNewValueOnSoundData(sound_data, start_pan);
      }

      void ChangePanTask::ApplyNewValueOnSoundData(SoundDataPtr sound_data, double new_pan)
      {
         sound_data->SetPan(new_pan);
      }
   } // end namespace Audio
} // end namespace TC
