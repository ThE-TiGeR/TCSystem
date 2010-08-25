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
//  $Id: TCAudioChangePanTask.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TC_AUDIO_CHANGE_PAN_TASK_H_
#define _TC_AUDIO_CHANGE_PAN_TASK_H_

#include "TCAudioChangeSoundValueTask.h"

namespace TC
{
   namespace Audio
   {
      /**
      * @brief Implements a task for changing the panning of an sound object
      */
      class TC_DLL_LOCAL ChangePanTask: public ChangeSoundValueTask
      {
      public:
         /**
         * @param sound_data Sound data for which the pan should be changed
         * @param start_pan Start pan at the beginning when starting this task
         * @param end_pan End pan when the task is finished
         * @param duration Time in milliseconds who long it should take to reach the end pan
         */
         ChangePanTask(SoundDataPtr sound_data,
            double start_pan, double end_pan, const Time& duration);

      protected:
         virtual void ApplyNewValueOnSoundData(SoundDataPtr, double new_value);
      };

   } // end namespace Audio
} // end namespace TC

#endif // _TC_AUDIO_CHANGE_PAN_TASK_H_
