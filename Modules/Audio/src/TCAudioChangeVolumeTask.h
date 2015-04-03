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
//  $Id: TCAudioChangeVolumeTask.h,v a33a555a5a40 2012/02/01 22:48:18 Thomas $
//*******************************************************************************

#ifndef _TC_AUDIO_CHANGE_VOLUME_TASK_H_
#define _TC_AUDIO_CHANGE_VOLUME_TASK_H_

#include "TCAudioChangeSoundValueTask.h"

namespace tc
{
   namespace audio
   {
      /**
      * @brief Implements a task for changing the volume of an sound object
      */
      class TC_DLL_LOCAL ChangeVolumeTask: public ChangeSoundValueTask
      {
      public:
         /**
         * @param sound_data Sound data for which the volume should be changed
         * @param start_volume Start volume at the beginning when starting this task
         * @param end_volume End volume when the task is finished
         * @param duration Time in milliseconds who long it should take to reach the end volume
         */
         ChangeVolumeTask(SoundDataPtr sound_data,
            double start_volume, double end_volume, const Time& duration);

      protected:
         virtual void ApplyNewValueOnSoundData(SoundDataPtr, double new_value);
      };

   } // end namespace audio
} // end namespace tc

#endif // _TC_AUDIO_CHANGE_VOLUME_TASK_H_
