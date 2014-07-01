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

#ifndef _TC_HEAP_MUTEX_H_
#define _TC_HEAP_MUTEX_H_

#include "TCHeap.h"

/** @addtogroup TC_BASE_DEBUG */
//@{
/**
* @file
* This header file provides the definition of the class tc::Heap::Mutex
*/
/** @} */

#ifdef TCOS_WINDOWS
#   include <windows.h>

namespace tc
{
   class Heap::Mutex
   {
   public:
      Mutex()
      {
#pragma warning(push)
#pragma warning(suppress: 28125)
         ::InitializeCriticalSection(&m_critical);
#pragma warning(pop)
      }
      ~Mutex()
      {
         ::DeleteCriticalSection(&m_critical);
      }
      void Lock()
      {
         ::EnterCriticalSection(&m_critical);
      }
      void UnLock()
      {
         ::LeaveCriticalSection(&m_critical);
      }
   private:
      CRITICAL_SECTION m_critical;
   };
}
#elif defined TCOS_POSIX
#   include <pthread.h>
#   include <cstdio>

namespace tc
{
class Heap::Mutex
{
public:
   Mutex()
   {
      ::pthread_mutexattr_t attr;
      if (::pthread_mutexattr_init(&attr) != 0)
      {
         std::perror("pthread_mutexattr_init failed");
         return;
      }

      if (::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
      {
         ::pthread_mutexattr_destroy(&attr);
         std::perror("pthread_mutexattr_settype failed");
         return;
      }

      if (::pthread_mutex_init(&m_mutex, &attr) != 0)
      {
         std::perror("pthread_mutex_init failed");
      }

      ::pthread_mutexattr_destroy(&attr);
   }
   ~Mutex()
   {
      ::pthread_mutex_destroy(&m_mutex);
   }
   void Lock()
   {
      if (::pthread_mutex_lock(&m_mutex) != 0)
      {
         std::perror("pthread_mutex_lock failed");
      }
   }
   void UnLock()
   {
      if (::pthread_mutex_unlock(&m_mutex) != 0)
      {
         std::perror("pthread_mutex_unlock failed");
      }
   }
private:
   ::pthread_mutex_t m_mutex;
};
}
#endif

#endif // _TC_HEAP_MUTEX_H_
