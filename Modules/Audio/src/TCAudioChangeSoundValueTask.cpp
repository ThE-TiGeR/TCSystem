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
//  $Id: TCAudioChangeSoundValueTask.cpp,v a33a555a5a40 2012/02/01 22:48:18 Thomas $
//*******************************************************************************

#include "TCAudioChangeSoundValueTask.h"

#include "TCTime.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      const Time ChangeSoundValueTask::CHANGE_VALUE_TIMEOUT(0, 100 * Time::ONE_MILLI_SECOND_AS_NANO_SECONDS);

      ChangeSoundValueTask::ChangeSoundValueTask(SoundDataPtr sound_data, 
         double start_value, double end_value, const Time& duration)
         :StreamingThread::Task(CHANGE_VALUE_TIMEOUT, tc::Time::Now()),
         m_sound_data(sound_data),
         m_start_value(start_value),
         m_end_value(end_value),
         m_change_value_duration(duration)
      {
      }

      ChangeSoundValueTask::~ChangeSoundValueTask()
      {
      }

      ChangeSoundValueTask::ReturnType ChangeSoundValueTask::Execute(const Time& current_time)
      {
         Time diff_time = current_time - GetLastExecutedTime();
         if (m_execution_time + diff_time >= m_change_value_duration)
         {
            m_change_value_duration = Time::Zero();
            ApplyNewValueOnSoundData(m_sound_data, m_end_value);
            return TASK_FINISHED;
         }

         m_execution_time += diff_time;
         ApplyNewValueOnSoundData(m_sound_data, GetCurrentValue());

         return TASK_OK;
      }

      double ChangeSoundValueTask::GetCurrentValue()
      {
         double diff_value = m_end_value - m_start_value;
         double value = m_start_value + diff_value * GetCurrentExecutionPosition();

         return value;
      }

      double ChangeSoundValueTask::GetCurrentExecutionPosition()
      {
         return static_cast<double>(m_execution_time.ToNanoSeconds()) / 
            static_cast<double>(m_change_value_duration.ToNanoSeconds());
      }

   } // end namespace audio
} // end namespace tc
