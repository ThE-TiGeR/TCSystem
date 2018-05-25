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
//  $Id: TCMTEventWin32.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TC_MT_EVENT_WIN32_H_
#define _TC_MT_EVENT_WIN32_H_

#include "TCMTEvent.h"

#include "TCMTOS.h"

namespace tc
{
namespace multi_threading
{
namespace imp
{
   /**
    * @addtogroup TC_MT_IMP
    * @{
    */

   /**
    * @brief The Event implementation class
    */
   class TC_DLL_LOCAL EventWin32: public Event
   {
   public:
      EventWin32(bool manual_reset, bool initial_state);
      ~EventWin32();

      bool Wait();
      bool TryWait(const Time& millisecs);
      bool Try();
      bool Set();
      bool Reset();

   private:
      /** Handle to the event object */
      HANDLE m_event;
   };

   /**
    * @}
    */

} // namespace imp
} // namespace multi_threading
} // namespace tc

#endif // _TC_MT_EVENT_WIN32_H_
