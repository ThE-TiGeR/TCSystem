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
//  $Id$
//*******************************************************************************
#include "TCDefines.h"

#ifdef TCOS_POSIX

#include "TCMTEventPthread.h"

#include "TCMTFactory.h"
#include "TCOutput.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace MT
   {
      namespace Impl
      {

         EventPthread::EventPthread(bool manual_reset, bool initial_state)
         {
            m_event.manual_reset = manual_reset;
            m_event.state        = initial_state;
            m_event.condition    = Factory::CreateCondition();
         }

         EventPthread::~EventPthread()
         {
            m_event.condition = ConditionPtr();
         }

         bool EventPthread::Wait()
         {
            TCTRACE("TCMT", 100, "...");

            MutexLocker lock(m_event.condition->GetMutex());

            // wait until event is set
            if (!m_event.state) m_event.condition->Wait();

            // reset if manual reset is set
            if (!m_event.manual_reset) m_event.state = false;

            TCTRACE("TCMT", 100, "done.");
            return true;
         }

         bool EventPthread::Try()
         {
            TCTRACE("TCMT", 100, "...");

            MutexLocker lock(m_event.condition->GetMutex());
            if (!m_event.state)
            {
               TCTRACE("TCMT", 100, "not set.");
               return false;
            }

            if (!m_event.manual_reset) m_event.state = false;

            TCTRACE("TCMT", 100, "was set.");
            return true;
         }

         bool EventPthread::TryWait(const Time& millisecs)
         {
            // for 0 we just try if set
            if (millisecs == Time::Zero())
            {
               return Try();
            }
            // for infinite we wait endless
            else if (millisecs == Time::Infinite())
            {
               return Wait();
            }

            TCTRACE1("TCMT", 100, "%d ms ...", millisecs.ToMilliSeconds());

            MutexLocker lock(m_event.condition->GetMutex());
            if (!m_event.state)
            {
               if (!m_event.condition->TryWait(millisecs))
               {
                  TCTRACE("TCMT", 100, "timeout.");
                  return false;
               }
            }

            if (!m_event.state)
            {
               TCERROR("TCMT", "not set.");
               return false;
            }

            if (!m_event.manual_reset) m_event.state = false;

            TCTRACE("TCMT", 100, "was set.");
            return true;
         }

         bool EventPthread::Set()
         {
            MutexLocker lock(m_event.condition->GetMutex());

            m_event.state = true;
            if (m_event.manual_reset)
               return m_event.condition->Broadcast();
            else
               return m_event.condition->Signal();
         }

         bool EventPthread::Reset()
         {
            MutexLocker lock(m_event.condition->GetMutex());
            m_event.state = false;

            return true;
         }

      } // namespace Impl
   } // namespace MT
} // namespace TC

#endif // TCOS_POSIX