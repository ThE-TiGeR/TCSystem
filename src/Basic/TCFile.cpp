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
//  $Id: TCFile.cpp 962 2010-03-29 22:22:38Z the_____tiger $
//*******************************************************************************

#include "TCFile.h"

#include "TCFileName.h"
#include "TCOutput.h"
#include "TCSystem.h"
#include "TCUtil.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace File
   {
      bool CreateDirectoryRecursive(const std::string& path)
      {
         if (path.empty()) return false;

         std::vector< std::string > directories;
         FileName::GetDirectoriesOfPath(path, directories);

         std::string current_dir;
         if (path[0] == FileName::GetPathSeparator()[0]) current_dir = FileName::GetPathSeparator();

         std::vector< std::string >::const_iterator dir;
         for (dir=directories.begin(); dir != directories.end(); dir++)
         {
            current_dir = FileName::AddPaths(current_dir, *dir);
            if (!IsDirectory(current_dir))
            {
               if (Exists(current_dir))
               {
                  return false;
               }

               if (!CreateDirectory(current_dir))
               {
                  return false;
               }
            }
         }

         return true;
      }

      // Return time when touched
      uint64 GetTouchedTime(const std::string &file)
      {
         return Util::Max(GetModificationTime(file), 
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
            file_info.name = FileName::AddFileNameAndPath(file->name, search_dir);
            if (file_info.is_directory)
            {
               FindFilesRecursive(files, file_info.name, search_ext);
            }
            else if (!search_ext.empty())
            {
               std::string ext = FileName::GetExtension(file_info.name);
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
