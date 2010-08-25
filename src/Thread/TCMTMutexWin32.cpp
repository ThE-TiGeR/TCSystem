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
//  $Id: TCMTMutexWin32.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifdef TCOS_WINDOWS

#include "TCMTMutexWin32.h"

#include "TCSystem.h"
#include "TCMTOS.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace MT
   {
      namespace Impl
      {

         MutexWin32::MutexWin32(bool locked)
         {
            m_handle = ::CreateMutexA(0, FALSE, 0);

            // lock it if it should be initially locked
            if (locked) Lock();
         }

         MutexWin32::MutexWin32(const std::string& shared_name, bool locked)
         {
            m_handle = ::CreateMutexA(0, FALSE, shared_name.c_str());

            // lock it if it should be initially locked
            if (locked) Lock();
         }

         MutexWin32::~MutexWin32()
         {
            if (m_handle)
            {
               ::CloseHandle(m_handle);
               m_handle = 0;
            }
         }

         bool MutexWin32::Lock()
         {
            return ::WaitForSingleObject(m_handle, INFINITE) == WAIT_OBJECT_0;
         }

         bool MutexWin32::TryLock()
         {
            return ::WaitForSingleObject(m_handle, 0) == WAIT_OBJECT_0;
         }

         bool MutexWin32::TryLock(const Time& millisecs)
         {
            return ::WaitForSingleObject(m_handle, static_cast<DWORD>(millisecs.ToMilliSeconds())) == WAIT_OBJECT_0;
         }

         bool MutexWin32::UnLock()
         {
            return ::ReleaseMutex(m_handle) != FALSE;
         }

      } // namespace Impl
   } // namespace MT
} // namespace TC

#endif // TCOS_WINDOWS
