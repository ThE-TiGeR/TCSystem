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
#include "TCDefines.h"

#if TCOS_POSIX
#include "TCMemoryMappedFilePosix.h"

#include "TCWString.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "TCNewEnable.h"

namespace tc
{
   MemoryMappedFilePosix::MemoryMappedFilePosix()
      :m_size(0),
      m_data(0),
      m_file(0),
      m_file_map(0),
      m_readonly(true)
   {
   }

   MemoryMappedFilePosix::~MemoryMappedFilePosix()
   {
      UnmapFromMemory();
   }

   uint32_t MemoryMappedFilePosix::GetSize() const
   {
      return m_size;
   }

   uint8_t* MemoryMappedFilePosix::GetData()
   {
      if (m_readonly)
      {
         return 0;
      }
      else
      {
         return static_cast<uint8_t*>(m_data);
      }
   }


   const uint8_t* MemoryMappedFilePosix::GetReadOnlyData() const
   {
      return static_cast<const uint8_t*>(m_data);
   }

   bool MemoryMappedFilePosix::MapToMemory(const std::string&  file_name, bool readonly, uint32_t size)
   {
      int file_handle = ::open(file_name.c_str(), readonly ? O_RDONLY : O_RDWR);
      if (file_handle == -1)
      {
         return false;
      }
      m_file = file_handle;

      uint32_t num_bytes = size;
      if (num_bytes == 0)
      {
         struct stat status;
         num_bytes = stat(file_name.c_str(), &status) == 0 ? status.st_size : 0;
      }
      m_file_map = ::mmap(0, num_bytes, readonly ? PROT_READ : PROT_READ|PROT_WRITE,
         MAP_PRIVATE, file_handle, 0);
      if (m_file_map == MAP_FAILED)
      {
         m_file_map = 0;
         return false;
      }

      m_data     = m_file_map;
      m_size     = num_bytes;
      m_readonly = readonly;

      return true;
   }

   bool MemoryMappedFilePosix::MapToMemory(const std::wstring& fileName, bool readonly, uint32_t size)
   {
      return MapToMemory(wstring::ToString(fileName.c_str()), readonly, size);
   }

   void MemoryMappedFilePosix::UnmapFromMemory()
   {
      m_data = 0;

      if (m_file_map)
      {
         ::munmap(m_file_map, m_size);
         m_file_map = 0;
      }

      if (m_file)
      {
         ::close(m_file);
         m_file = 0;
      }

   }
}
#endif
