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

#ifndef _TC_MT_SEMAPHORE_H_
#define _TC_MT_SEMAPHORE_H_

#include "TCNonCopyable.h"
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
    * @brief This file provides the definition of tc::multi_threading::Semaphore
    * @author Thomas Goessler
    */

   /**
    * counted semaphore
    */
   class Semaphore: protected NonCopyAble
   {
   public:
      /**
       * if semaphore value is > 0 then decrement it and carry on. If it's
       * already 0 then block until > 0.
       */
      virtual bool Wait() = 0;

      /**
       * if semaphore value is > 0 then decrement it and return 1 (true).
       * If it's already 0 then return 0 (false).
       */
      virtual bool Try() = 0;

      /**
       * if semaphore value is > 0 then decrement it and return 1 (true).
       * If it's already 0 then block for maximum specified time until > 0
       */
      virtual bool TryWait(const Time& millisecs) = 0;

      /**
       * if any threads are blocked in wait(), wake one of them up. Otherwise
       * increment the value of the semaphore.
       */
      virtual bool Post() = 0;

      virtual ~Semaphore() {}

   };

   /** @brief Smart pointer to an semaphore object */
   typedef SharedPtr< Semaphore > SemaphorePtr;

   /** @brief Locking object for Semaphore objects */
   class SemaphoreLocker: protected NonCopyAble
   {
   public:
      SemaphoreLocker(SemaphorePtr s) 
         :m_semaphore(s)
      {
         m_semaphore->Wait(); 
      }
      ~SemaphoreLocker() 
      { 
         m_semaphore->Post(); 
      }

   private:
      SemaphorePtr m_semaphore;
   };

   /**
    * @}
    */

} // namespace multi_threading
} // namespace tc


#endif // _TC_MT_SEMAPHORE_H_
