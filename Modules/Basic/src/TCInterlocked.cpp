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
//  $Id: TCInterlocked.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCInterlocked.h"

#ifdef TCOS_WINDOWS
#   include <windows.h>
#else
#   include <pthread.h>
#endif

#include "TCNewEnable.h"

namespace tc
{
   namespace interlocked
   {

#ifdef TCOS_WINDOWS
#else
      class Locked
      {
      public:
         Locked()
         {
            Init();
            ::pthread_mutex_lock(&s_interlocked_handle);
         }
         ~Locked()
         {
            ::pthread_mutex_unlock(&s_interlocked_handle);
         }
      private:
         static void Init()
         {
            static bool init(false);
            if (!init)
            {
               ::pthread_mutex_init(&s_interlocked_handle, 0);
               init = true;
            }
         }
         static pthread_mutex_t s_interlocked_handle;
      };
      pthread_mutex_t Locked::s_interlocked_handle;
#endif

      Type Increment(Type& target)
      {
#ifdef TCOS_WINDOWS
         return ::InterlockedIncrement(&target);
#else
         Locked lock;
         return ++target;
#endif
      }

      Type Decrement(Type& target)
      {
#ifdef TCOS_WINDOWS
         return ::InterlockedDecrement(&target);
#else
         Locked lock;
         return --target;
#endif
      }

      Type Exchange(Type& target, int32_t new_value)
      {
#ifdef TCOS_WINDOWS
         return ::InterlockedExchange(&target, new_value);
#else
         Locked lock;

         Type tmp = new_value;
         new_value = target;
         return tmp;
#endif
      }

      Type Add(Type& target, int32_t value_to_add)
      {
#ifdef TCOS_WINDOWS
         return ::InterlockedExchangeAdd(&target, value_to_add);
#else
         Locked lock;

         int32_t old_value = target;
         target += value_to_add;

         return old_value;
#endif
      }
   }
}

