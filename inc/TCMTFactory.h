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
//  $Id: TCMTFactory.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TC_MT_FACTORY_H_
#define _TC_MT_FACTORY_H_

#include "TCMTCondition.h"
#include "TCMTEvent.h"
#include "TCMTMutex.h"
#include "TCMTSemaphore.h"
#include "TCMTThread.h"
#include "TCMTMessageDispatcher.h"

namespace TC
{
namespace MT
{
   /**
    * @addtogroup TC_MT
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TC::MT::Factory
    * @author Thomas Goessler
    */

   /**
    * @brief Factory for creating TC::MT objects
    */
   namespace Factory
   {
       /** Creation mode for shared objects */
       enum CreationMode
       {
           CRM_ALWAYS,
           CRM_WHEN_NOT_EXISTS,
           CRM_WHEN_EXISTS
       };

      /**
       * @brief Create a thread object
       *
       * When returning the thread is already in the running state
       * @param thread_name Name of the thread
       * @param stack_size Stack size of the thread 0 = system default stack size
       * @param priority Priority of the thread @see ThreadPriority
       */
      TCMT_API ThreadPtr CreateThread(const std::string& thread_name,
                                      uint32 stack_size=0,
                                      Thread::ThreadPriority priority=Thread::PRIORITY_NORMAL);

      /**
       * @brief Create a message dispatcher object
       * @see TC::MT::MessageDispatcher
       */
      TCMT_API MessageDispatcherPtr CreateMessageDispatcher();

      /**
       * @brief Get The current thread object in which this command is executed
       */
      TCMT_API ThreadPtr GetCurrentThread();

      /**
       * @brief Create a Mutex object
       *
       * For detailed description @see TCMT::Mutex
       * @param locked true if the mutex should be already owned(locked)
       * @return A shared pointer of the created mutex
       */
      TCMT_API MutexPtr CreateMutex(bool locked=false);
      /**
      * @brief Create a shared Mutex object with specified name
      *
      * For detailed description @see TCMT::Mutex
      * @param shared_name the name of the mutex so that it can be created inside other processes
      * @param locked true if the mutex should be already owned(locked)
      * @return A shared pointer of the created mutex
      */
      TCMT_API MutexPtr CreateMutex(const std::string& shared_name, bool locked=false, CreationMode mode=CRM_ALWAYS);

      /** 
       * @brief Construct an event object
       * @param manual_reset True if the event hast to be reset manual after a wait
       * @param initial_state True to set the event at the beginning
       */
      TCMT_API EventPtr CreateEvent(bool manual_reset=false, bool initial_state=false);

      /** 
       * @brief Construct an semaphore object
       * @param initial_value The initial value of the semaphore
       */
      TCMT_API SemaphorePtr CreateSemaphore(uint32 initial_value);
      /** 
       * @brief Construct an shared semaphore object
       * @param shared_name the name of the mutex so that it can be created inside other processes
       * @param initial_value The initial value of the semaphore
       */
      TCMT_API SemaphorePtr CreateSemaphore(const std::string& shared_name, uint32 initial_value, CreationMode mode=CRM_ALWAYS);

      /**
       * @brief construct a Condition variable
       *
       * The condition variable is linked a the mutex, so that there must be an
       * implicit UnLock and Lock around Wait() and Try(). The Mutex can be 
       * obtained with GetMutex
       */
      TCMT_API ConditionPtr CreateCondition();

      /**
       * @brief construct a Condition variable with an existing mutex
       */
      TCMT_API ConditionPtr CreateCondition(MutexPtr mutex);
   };

   /**
    * @}
    */

} // namespace MT
} // namespace TC


#endif //_TC_MT_FACTORY_H_
