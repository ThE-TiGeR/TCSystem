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
// see https://bitbucket.org/the____tiger/tcsystem for details.
// Copyright (C) 2003 - 2018 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCClock.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCClock.h"

#include "TCTime.h"

#include "TCNewEnable.h"

namespace tc
{
Clock::Clock()
{
   Reset();
}

Clock::~Clock()
{
}

void Clock::Reset()
{
   m_running = false;

   m_current_time     = Time::Now().ToMilliSeconds();
   m_last_time        = m_current_time;
   m_system_time      = 0;
   m_stop_start_time  = 0;

   m_frame_count      = 0;
   m_frame_start_time = 0;
   m_frame_end_time   = 0;

   m_simulation_time        = 0;
   m_simulation_offset_time = 0;
}

void Clock::Start()
{
   if (!m_running)
   {
      Update();
      m_simulation_offset_time += (m_system_time - m_stop_start_time);
      m_running = true;
   }
}

void Clock::Stop()
{
   if (m_running)
   {
      Update();
      m_stop_start_time = m_system_time;

      m_running = false;
   }
}

void Clock::Update()
{
   // get windows' idea of current time
   uint64_t elapsed = 0;

   m_last_time    = m_current_time;
   m_current_time = Time::Now().ToMilliSeconds();

   // convert to elapsed time
   // also handle rollover, which happens every 2^32 milliseconds
   if (m_current_time < m_last_time)
   {
      elapsed = m_last_time - m_current_time;
   }
   else
   {
      elapsed = m_current_time - m_last_time;
   }
   // system time is real time and never pauses
   m_system_time += elapsed;
}

void Clock::BeginFrame()
{
   m_frame_count++;

   Update(); // update system time

   if (m_running)
   {
      m_frame_start_time = m_frame_end_time; // start of this frame = end of last frame
      m_frame_end_time = m_system_time - m_simulation_offset_time; // end of this frame = current time
      m_simulation_time = m_frame_start_time; // set simulation time to start of frame
   }
}

}

