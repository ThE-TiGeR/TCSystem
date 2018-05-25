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
//  $Id: TCFile.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCFile.h"

#include "TCFileName.h"
#include "TCOutput.h"
#include "TCSystem.h"
#include "TCUtil.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace file
   {
      bool CreateDirRecursive(const std::string& path)
      {
         if (path.empty()) return false;

         std::vector< std::string > directories;
         file_name::GetDirectoriesOfPath(path, directories);

         std::string current_dir;
         if (path[0] == file_name::GetPathSeparator()[0]) current_dir = file_name::GetPathSeparator();

         std::vector< std::string >::const_iterator dir;
         for (dir=directories.begin(); dir != directories.end(); dir++)
         {
            current_dir = file_name::AddPaths(current_dir, *dir);
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

      bool RemoveDirRecursive(const std::string& path)
      {
         if (path.empty()) return false;

         FileInfos file_infos;
         GetFileInfosOfDirectory(file_infos, path);

         for (FileInfos::const_iterator it=file_infos.begin(); it!=file_infos.end(); ++it)
         {
            if (it->is_directory)
            {
               std::string dir = file_name::AddPaths(path, it->name);
               if (!RemoveDirRecursive(dir))
               {
                  return false;
               }
            }
            else
            {
               std::string file = file_name::AddFileNameAndPath(it->name, path);
               if (!Remove(file))
               {
                  return false;
               }
            }
         }
         return RemoveDir(path);
      }

      // Return time when touched
      uint64_t GetTouchedTime(const std::string &file)
      {
         return util::Max(GetModificationTime(file), 
            GetLastAccessTime(file),
            GetCreationTime(file));
      }

      bool Create(const std::string &file, bool remove_old)
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

         ::FILE *fd=::fopen(file.c_str(), "w");
         if(fd)
         {
            ::fclose(fd);
            return true;
         }
         return false;
      }

      void FindFilesRecursive(FileInfos& files, const std::string& search_dir, 
         const std::string& search_ext)
      {
         FileInfos file_infos_of_dir;
         GetFileInfosOfDirectory(file_infos_of_dir, search_dir);

         FileInfos::const_iterator file;
         for (file=file_infos_of_dir.begin(); file!=file_infos_of_dir.end(); file++)
         {
            FileInfo file_info(*file);
            file_info.name = file_name::AddFileNameAndPath(file->name, search_dir);
            if (file_info.is_directory)
            {
               FindFilesRecursive(files, file_info.name, search_ext);
            }
            else if (!search_ext.empty())
            {
               std::string ext = file_name::GetExtension(file_info.name);
               if (ext == search_ext)
               {
                  files.push_back(file_info);
               }
            }
            else
            {
               files.push_back(file_info);
            }
         }
      }
   }
}
