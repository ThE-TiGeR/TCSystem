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

#ifndef _TC_MT_MUTEX_WIN32_H_
#define _TC_MT_MUTEX_WIN32_H_

#include "TCMTMutex.h"
#include "TCMTFactory.h"

#include <string>

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
    * Implementation class of Mutex for Win32
    */
   class TC_DLL_LOCAL MutexWin32: public Mutex
   {
   public:
      MutexWin32();
      ~MutexWin32();

      bool Init(bool locked);
      bool Init(const std::string& shared_name, bool locked, Factory::CreationMode mode);

      bool Lock();
      bool TryLock();
      bool TryLock(const Time& millisecs);
      bool UnLock();
   private:
      void* m_handle;

      friend class ConditionWin32;
   };

   /**
    * @}
    */

} // namespace Impl
} // namespace MT
} // namespace TC

#endif // _TC_MT_MUTEX_WIN32_H_
