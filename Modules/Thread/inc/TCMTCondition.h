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

#ifndef _TC_MT_CONDITION_H_
#define _TC_MT_CONDITION_H_

#include "TCMTMutex.h"
#include "TCSharedPtr.h"

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
    * @brief This file provides the definition of tc::multi_threading::Condition
    * @author Thomas Goessler
    */

   /**
    * Condition variable
    *
    * The condition variable is linked to the mutex, so that there is an
    * implicit UnLock and Lock around Wait() and TryWait(). The Mutex can be 
    * obtained with GetMutex
    */
   class Condition
   {
   public:
     /** @brief Get the mutex used with this condition */
     virtual MutexPtr GetMutex() = 0;

     /**
      * @brief Wait for the condition variable to be signaled.
      *
      * The mutex is implicitly released before waiting and locked again after waking up.
      * If Wait() is called by multiple threads, a signal may wake up more
      * than one thread.  See POSIX threads documentation for details.
      * @return (true) if successfully signaled else (false)
      */
     virtual bool Wait() = 0;
     
     /**
      * @brief Wait for the condition variable to be signaled for specified time.
      * 
      * @param timeout Time to wait in milliseconds
      * @return (true) if successfully signaled, (false) if time expired.
      */
     virtual bool TryWait(const Time& timeout) = 0;
     
     /**
      * @brief If one or more threads have called Wait() Signal wakes up at least
      * one of them, possibly more.
      * See POSIX threads documentation for details.
      * @return (true) if successfully signaled otherwise (false).
      */
     virtual bool Signal() = 0;
     
     /**
      * @brief Broadcast is like Signal but wakes all threads which have called Wait().
      * @return (true) if successfully signaled otherwise (false).
      */
     virtual bool Broadcast() = 0;

     virtual ~Condition() {}
   };

   /** @brief Smart pointer to a condition object */
   typedef SharedPtr< Condition > ConditionPtr;

  /**
    * @}
    */

} // namespace multi_threading
} // namespace tc

#endif // _TC_MT_CONDITION_H_
