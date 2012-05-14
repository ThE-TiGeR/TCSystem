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

#ifndef _TC_MT_EVENT_H_
#define _TC_MT_EVENT_H_

#include "TCSharedPtr.h"
#include "TCTime.h"

namespace tc
{
namespace multi_threading
{

   /**
    * @addtogroup TC_MT
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of tc::multi_threading::Event
    * @author Thomas Goessler
    */

   /**
    * An object of class Event represents a synchronization object that allows one thread 
    * to notify another that an event has occurred. Events are useful when a thread needs 
    * to know when to perform its task or a task has finished
    *
    * Event objects have two types: manual and automatic. 
    * A manual Event object stays in the state set by Set or Reset until the other 
    * function is called. An automatic Event object automatically returns to a 
    * nonsignaled (unavailable) state after at least one thread is released.
    *
    * To use a Event object, construct the Event object when it is needed.
    * Call Set to signal (make available) the event object. In the other thread reat it out 
    * with Wait or TryWait.
    */
   class Event
   {
   public:
      /**
       * @brief Wait for ever until the event was set or an error happens
       * @return true if the event was set else error
       */
      virtual bool Wait() = 0;
      /**
       * @brief Wait for specified time or until the event was set or an error happens
       * @param millisecs Time in milliseconds to wait
       * @return true if the event was set else timeout or error
       */
      virtual bool TryWait(const Time& millisecs) = 0;
      /**
       * @brief Try if the event is already set
       * @return true if the event was set else error
       */
      virtual bool Try() = 0;
      /**
       * @brief Set the event
       * @return if successful set
       */
      virtual bool Set() = 0;
      /**
       * @brief Reset the event
       * @return if successful reset
       */
      virtual bool Reset() = 0;

      virtual ~Event() {}
   };

   /** @brief Smart pointer to an event object */
   typedef SharedPtr< Event > EventPtr;

   /**
    * @}
    */

} // namespace multi_threading
} // namespace tc

#endif // _TC_MT_EVENT_H_
