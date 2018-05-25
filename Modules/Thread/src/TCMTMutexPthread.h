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
//  $Id: TCMTMutexPthread.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TC_MT_MUTEX_PTHREAD_H_
#define _TC_MT_MUTEX_PTHREAD_H_

#include "TCMTMutex.h"

#include "TCMTOS.h"
#include "TCMTSemaphorePthread.h"

#include <string>

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
    * Implementation class of Mutex for pthread
    */
   class TC_DLL_LOCAL MutexPthread: public Mutex
   {
   public:
      MutexPthread();
      ~MutexPthread();

      bool Init(bool locked);

      bool Lock();
      bool TryLock();
      bool TryLock(const Time& millisecs);
      bool UnLock();

   private:
      ::pthread_mutex_t m_mutex;

      friend class ConditionPthread;
   };

   class TC_DLL_LOCAL MutexSharedPthread: public Mutex
   {
   public:
      MutexSharedPthread();
      ~MutexSharedPthread();

      bool Init(const std::string& shared_name, bool locked, factory::CreationMode mode);

      bool Lock();
      bool TryLock();
      bool TryLock(const Time& millisecs);
      bool UnLock();

   private:
      bool TryAndGetIfOwner();
      void SetOwnerShip();
      bool UnGetIfOwner();

   private:
      uint32_t m_nesting_level;
      ::pthread_t m_owner_process;
      SemaphorePtr m_semaphore;
   };

   /**
    * @}
    */

} // namespace imp
} // namespace multi_threading
} // namespace tc

#endif // _TC_MT_MUTEX_IMP_H_
