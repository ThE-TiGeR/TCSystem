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
#include "TCDefines.h"

#if TCOS_WINDOWS
#include "TCMemoryMappedFileWin32.h"

#include "TCWString.h"

#include "TCNewEnable.h"

namespace TC
{
   MemoryMappedFileWin32::MemoryMappedFileWin32()
      :m_size(0),
      m_data(0),
      m_file(0),
      m_file_map(0),
      m_readonly(true)
   {
   }

   MemoryMappedFileWin32::~MemoryMappedFileWin32()
   {
      UnmapFromMemory();
   }

   uint32 MemoryMappedFileWin32::GetSize() const
   {
      return m_size;
   }

   uint8* MemoryMappedFileWin32::GetData()
   {
      if (m_readonly)
      {
         return 0;
      }
      else
      {
         return static_cast<uint8*>(m_data);
      }
   }


   const uint8* MemoryMappedFileWin32::GetReadOnlyData() const
   {
      return static_cast<const uint8*>(m_data);
   }

   bool MemoryMappedFileWin32::MapToMemory(const std::string& fileName, bool readonly, uint32 size)
   {
      return MapToMemory(WString::ToString(fileName), readonly, size);
   }

   bool MemoryMappedFileWin32::MapToMemory(const std::wstring& fileName, bool readonly, uint32 size)
   {
      m_file = ::CreateFileW(fileName.c_str(),
         readonly ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE),
         readonly ? FILE_SHARE_READ : 0, 
         0, readonly ? OPEN_EXISTING : OPEN_ALWAYS,
         FILE_ATTRIBUTE_NORMAL, 0);

      if (m_file == INVALID_HANDLE_VALUE)
      {
         return false;
      }

      m_file_map = ::CreateFileMapping(m_file, 
         0, (readonly ? PAGE_READONLY : PAGE_READWRITE),
         0, size, 0);           

      if (m_file_map == 0)
      {
         return false;
      }

      m_data = ::MapViewOfFile(
         m_file_map,
         (readonly ? FILE_MAP_READ : FILE_MAP_WRITE),
         0,         
         0,         
         0);        

      if (m_data == 0)
      {
         return false;
      }

      m_size = ::GetFileSize(m_file, 0);
      m_readonly = readonly;

      return true;
   }

   void MemoryMappedFileWin32::UnmapFromMemory()
   {
      if (m_data)
      {
         ::UnmapViewOfFile(m_data);
         m_data = 0;
      }
      if (m_file_map)
      {
         ::CloseHandle(m_file_map);
         m_file_map = 0;
      }
      if (m_file)
      {
         ::CloseHandle(m_file);
         m_file = 0;
      }
   }
}
#endif
