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
//  $Id: TCMTSemaphoreWin32.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#include "TCDefines.h"

#ifdef TCOS_WINDOWS

#include "TCMTSemaphoreWin32.h"

#include "TCMTOS.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {

         SemaphoreWin32::SemaphoreWin32() 
            :m_handle(0)
         {
         }

         bool SemaphoreWin32::Init(uint32_t initial) 
         {
            m_handle = ::CreateSemaphoreA(0, initial, 0xffffff, 0);
            if (m_handle == 0)
            {
               return false;
            }

            return true;
         }

         bool SemaphoreWin32::Init(const std::string& shared_name, uint32_t initial, factory::CreationMode mode) 
         {
            m_handle = ::CreateSemaphoreA(0, initial, 0xffffff, shared_name.c_str());
            DWORD error = ::GetLastError();
            if (m_handle == 0)
            {
               return false;
            }

            switch(mode)
            {
            case factory::CRM_ALWAYS:
               return true;
            case factory::CRM_WHEN_EXISTS:
               return error == ERROR_ALREADY_EXISTS;
            case factory::CRM_WHEN_NOT_EXISTS:
               return error != ERROR_ALREADY_EXISTS;
            }

            return false;
         }

         SemaphoreWin32::~SemaphoreWin32()
         {
            if (m_handle)
            {
               ::CloseHandle(m_handle);
               m_handle = 0;
            }
         }

         bool SemaphoreWin32::Wait()
         {
            return ::WaitForSingleObject(m_handle, INFINITE) == WAIT_OBJECT_0;
         }

         bool SemaphoreWin32::Try()
         {
            return ::WaitForSingleObject(m_handle, 0) == WAIT_OBJECT_0;
         }

         bool SemaphoreWin32::TryWait(const Time& timeout)
         {
            return ::WaitForSingleObject(m_handle, static_cast<DWORD>(timeout.ToMilliSeconds())) == WAIT_OBJECT_0;
         }

         bool SemaphoreWin32::Post()
         {
            return ::ReleaseSemaphore(m_handle, 1, 0) == TRUE;
         }

      } // namespace imp
   } // namespace multi_threading
} // namespace tc

#endif // TCOS_WINDOWS
