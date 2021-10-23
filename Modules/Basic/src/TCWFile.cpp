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
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCFile.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCWFile.h"

#include "TCWFileName.h"
#include "TCOutput.h"
#include "TCUtil.h"

namespace tc::wfile
{
   bool CreateDirRecursive(const std::wstring& path)
   {
      if (path.empty())
      {
         return false;
      }

      std::vector<std::wstring> directories(wfile_name::GetDirectoriesOfPath(path));

      std::wstring current_dir;
      if (path[0] == wfile_name::GetPathSeparator()[0])
      {
         current_dir = wfile_name::GetPathSeparator();
      }

      for (const auto& directory : directories)
      {
         current_dir = wfile_name::AddPaths(current_dir, directory);
         if (!IsDirectory(current_dir))
         {
            if (Exists(current_dir))
            {
               return false;
            }

            if (!CreateDir(current_dir))
            {
               return false;
            }
         }
      }

      return true;
   }

   bool RemoveDirRecursive(const std::wstring& path)
   {
      if (path.empty()) return false;

      FileInfos file_infos;
      GetFileInfosOfDirectory(file_infos, path);

      for (const auto& file_info : file_infos)
      {
         if (file_info.is_directory)
         {
            std::wstring dir = wfile_name::AddPaths(path, file_info.name);
            if (!RemoveDirRecursive(dir))
            {
               return false;
            }
         }
         else
         {
            std::wstring file = wfile_name::AddFileNameAndPath(file_info.name, path);
            if (!Remove(file))
            {
               return false;
            }
         }
      }
      return RemoveDir(path);
   }

   // Return time when touched
   uint64_t GetTouchedTime(const std::wstring& file)
   {
      return util::Max(GetModificationTime(file),
         GetLastAccessTime(file),
         GetCreationTime(file));
   }

   bool Create(const std::wstring& file, bool remove_old)
   {
      // check for remove or if already exists
      if (remove_old)
      {
         Remove(file);
      }
      else if (IsFile(file))
      {
         return true;
      }

      ::FILE* fd = ::_wfopen(file.c_str(), L"w");
      if (fd)
      {
         ::fclose(fd);
         return true;
      }
      return false;
   }

   FileInfos FindFilesRecursive(const std::wstring& search_dir, const std::wstring& search_ext)
   {
      FileInfos all_files;
      FileInfos file_infos_of_dir;
      GetFileInfosOfDirectory(file_infos_of_dir, search_dir);

      for (auto& file_info : file_infos_of_dir)
      {
         file_info.name = wfile_name::AddFileNameAndPath(file_info.name, search_dir);
         if (file_info.is_directory)
         {
            auto files = FindFilesRecursive(file_info.name, search_ext);
            all_files.insert(all_files.end(), files.begin(), files.end());
         }
         else if (!search_ext.empty())
         {
            std::wstring ext = wfile_name::GetExtension(file_info.name);
            if (ext == search_ext)
            {
               all_files.push_back(file_info);
            }
         }
         else
         {
            all_files.push_back(file_info);
         }
      }

      return all_files;
   }
}
