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
// Copyright (C) 2003 - 2014 Thomas Goessler. All Rights Reserved. 
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

#ifndef _TC_MT_MUTEX_H_
#define _TC_MT_MUTEX_H_

#include "TCMTLock.h"
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
    * @brief This file provides the definition of tc::multi_threading::Mutex
    * @author Thomas Goessler
    */

   /**
    * Class for protecting variables or something else which is used in more than one thread
    */
   class Mutex
   {
   public:
      /** Lock the mutex, if already locked wait until it get unlocked */
      virtual bool Lock() = 0;
      /**
       * @brief Try to look the mutex if not locked lock it
       * @return true if locked else false
       */
      virtual bool TryLock() = 0;
      /** Try to look the mutex within specified time */
      virtual bool TryLock(const Time& millisecs) = 0;
      /** UnLock the mutex */
      virtual bool UnLock() = 0;

      virtual ~Mutex() {};
   };

   /** @brief Smart pointer to a mutex object */
   typedef SharedPtr< Mutex > MutexPtr;

   /** @short Provides a typedef for locking this object */
   typedef LockerPtr< MutexPtr > MutexLocker;

   /**
    * @}
    */

} // namespace multi_threading
} // namespace tc

#endif // _TC_MT_MUTEX_H_
