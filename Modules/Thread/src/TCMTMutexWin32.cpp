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
//  $Id: TCMTMutexWin32.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#include "TCDefines.h"

#ifdef TCOS_WINDOWS

#include "TCMTMutexWin32.h"

#include "TCSystem.h"
#include "TCMTOS.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {

         MutexWin32::MutexWin32()
            :m_handle(0)
         {
         }

         bool MutexWin32::Init(bool locked)
         {
            m_handle = ::CreateMutexA(0, FALSE, 0);
            if (m_handle == 0)
            {
               return false;
            }

            // lock it if it should be initially locked
            if (locked) 
            {
               return Lock();
            }

            return true;
         }

         bool MutexWin32::Init(const std::string& shared_name, bool locked, factory::CreationMode mode)
         {
            bool status = false;
            m_handle = ::CreateMutexA(0, FALSE, shared_name.c_str());
            DWORD error = ::GetLastError();
            if (m_handle == 0)
            {
               return status;
            }

            switch(mode)
            {
            case factory::CreationMode::ALWAYS:
               status = true;
               break;
            case factory::CreationMode::WHEN_EXISTS:
               status = error == ERROR_ALREADY_EXISTS;
               break;
            case factory::CreationMode::WHEN_NOT_EXISTS:
               status = error != ERROR_ALREADY_EXISTS;
               break;
            }

            // lock it if it should be initially locked
            if (status && locked) 
            {
               return Lock();
            }

            return status;
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

      } // namespace imp
   } // namespace multi_threading
} // namespace tc

#endif // TCOS_WINDOWS
