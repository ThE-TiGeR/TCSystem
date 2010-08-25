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
//  $Id: TCMTConditionPthread.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TC_MT_CONDITION_PTHREAD_H_
#define _TC_MT_CONDITION_PTHREAD_H_

#include "TCMTCondition.h"
#include "TCMTMutexPthread.h"

namespace TC
{
namespace MT
{
namespace Impl
{
   /**
    * @addtogroup TC_MT_IMP
    * @{
    */

   /**
    * Implementation class of Condition for pthread
    */
  class TC_DLL_LOCAL ConditionPthread: public Condition
   {
   public:
     ConditionPthread();
     ConditionPthread(MutexPtr mutex);
     virtual ~ConditionPthread();
     
     MutexPtr GetMutex();

     bool Wait();
     bool TryWait(const Time& timeout);
     bool Signal();
     bool Broadcast();
     
   private:
     SharedPtr<MutexPthread> m_mutex;
     ::pthread_cond_t m_condition;
   };

   /**
    * @}
    */

} // namespace Impl
} // namespace MT
} // namespace TC

#endif // _TC_MT_CONDITION_PTHREAD_H_
