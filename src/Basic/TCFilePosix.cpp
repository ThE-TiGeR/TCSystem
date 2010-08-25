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
//  $Id: TCFilePosix.cpp 987 2010-05-17 08:28:04Z the_____tiger $
//*******************************************************************************

#if TCOS_POSIX
#include "TCFile.h"

#include "TCFileName.h"
#include "TCOutput.h"
#include "TCSystem.h"
#include "TCUtil.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>

#include "TCNewEnable.h"

namespace TC
{
   bool File::ChangeDirectory(const std::string& directory)
   {
      if (directory.empty()) 
      {
         return false;
      }
      return ::chdir(directory.c_str()) == 0;
   }

   std::string File::GetDirectory()
   {
      char buffer[512] = "";
      if (!::getcwd(buffer, Util::ArraySize(buffer)))
      {
         buffer[0] = 0;
      }

      return buffer;
   }

   bool File::Exists(const std::string & file)
   {
      struct stat buf;
      return stat(file.c_str(), &buf) == 0;
   }

   // Check whether its a directory
   bool File::IsDirectory(const std::string & file)
   {
      struct stat info;
      return stat(file.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
   }

   // Check whether its a file
   bool File::IsFile(const std::string & file)
   {
      struct stat info;
      return stat(file.c_str(), &info) == 0 && S_ISREG(info.st_mode);
   }

   // Return 1 if file is readable
   bool File::IsReadable(const std::string &file)
   {
      return access(file.c_str(), R_OK) == 0;
   }

   // Return 1 if file is writeable
   bool File::IsWriteable(const std::string &file)
   {
      return access(file.c_str(), W_OK) == 0;
   }

   // Return 1 if file is executable
   bool File::IsExecutable(const std::string &file)
   {
      return access(file.c_str(), X_OK) == 0;
   }

   // Change the mode flags for this file
   bool File::SetFileAttr(const std::string &file, uint32 attr)
   {
      sint32 a = 0;
      if (attr && FILEATTR_WRITE == FILEATTR_WRITE) {
         attr = FILEATTR_OWNER_WRITE | FILEATTR_GROUP_WRITE | FILEATTR_EVERYONE_WRITE;
      }
      if (attr && FILEATTR_READONLY == FILEATTR_READONLY) {
         attr = FILEATTR_OWNER_READ | FILEATTR_GROUP_READ | FILEATTR_EVERYONE_READ;
      }
      if (attr && FILEATTR_EXECUTE == FILEATTR_EXECUTE) {
         attr |= FILEATTR_OWNER_EXEC | FILEATTR_GROUP_EXEC | FILEATTR_EVERYONE_EXEC;
      }
      if (attr && FILEATTR_OWNER_WRITE == FILEATTR_OWNER_WRITE) a |= S_IWUSR;
      if (attr && FILEATTR_GROUP_WRITE == FILEATTR_GROUP_WRITE) a |= S_IWGRP;
      if (attr && FILEATTR_EVERYONE_WRITE == FILEATTR_EVERYONE_WRITE) a |= S_IWOTH;

      if (attr && FILEATTR_OWNER_READ == FILEATTR_OWNER_READ) a |= S_IRUSR;
      if (attr && FILEATTR_GROUP_READ == FILEATTR_GROUP_READ) a |= S_IRGRP;
      if (attr && FILEATTR_EVERYONE_READ == FILEATTR_EVERYONE_READ) a |= S_IROTH;

      if (attr && FILEATTR_OWNER_EXEC == FILEATTR_OWNER_EXEC) a |= S_IXUSR;
      if (attr && FILEATTR_GROUP_EXEC == FILEATTR_GROUP_EXEC) a |= S_IXGRP;
      if (attr && FILEATTR_EVERYONE_EXEC == FILEATTR_EVERYONE_EXEC) a |= S_IXOTH;

      return file!="" && chmod(file.c_str(), a)==0;
   }

   bool File::Remove(const std::string & file)
   {
      if (!Exists(file))
      {
         return false;
      }

      if (IsDirectory(file))
      {
         return ::rmdir(file.c_str()) == 0;
      }
      else
      {
         return ::remove(file.c_str()) == 0;
      }
   }

   bool File::Copy(const std::string& source, const std::string& dest, SharedPtr<Progress> copy_status)
   {
      if (!IsFile(source))
      {
         return false;
      }

      uint32 buf_sz = 32 * 1024;
      std::vector<char> buf(buf_sz);
      int infile=0, outfile=0;
      struct stat from_stat;

      if ((::stat(source.c_str(), &from_stat) != 0) ||
          (infile  = ::open(source.c_str(), O_RDONLY)) < 0 ||
          (outfile = ::open(dest.c_str(), O_WRONLY | O_CREAT | O_EXCL, from_stat.st_mode)) < 0)
      {
         if (infile >= 0) ::close(infile);
         return false;
      }

      int sz_read=1;
      while (sz_read > 0
         && (sz_read = ::read(infile, &buf.front(), buf_sz)) > 0)
      {
         int sz_write = 0, sz;
         do
         {
            if ((sz = ::write(outfile, &buf.front(), sz_read - sz_write)) < 0)
            { 
               sz_read = sz;
               break;
            }
            sz_write += sz;
         } while (sz_write < sz_read);
      }

      if (::close(infile) < 0) sz_read = -1;
      if (::close(outfile) < 0) sz_read = -1;

      if (sz_read < 0) return false;

      return true;
   }

   bool File::Move(const std::string& source, const std::string& dest)
   {
      if (!IsFile(source))
      {
         return false;
      }

      return ::rename(source.c_str(), dest.c_str()) == 0;
   }

   bool File::CreateDirectory(const std::string& path)
   {
      if (path.empty()) 
      {
         return false;
      }

      return ::mkdir(path.c_str(), 0777) == 0;
   }

   // Return time file was last modified
   uint64 File::GetModificationTime(const std::string &file)
   {
      struct stat status;
      return stat(file.c_str(), &status) == 0 ? (uint64)status.st_mtime : 0;
   }

   // Return time file was last accessed
   uint64 File::GetLastAccessTime(const std::string &file)
   {
      struct stat status;
      return stat(file.c_str(), &status) == 0 ? (uint64)status.st_atime : 0;
   }

   // Return time when created
   uint64 File::GetCreationTime(const std::string &file)
   {
      struct stat status;
      return stat(file.c_str(), &status) == 0 ? (uint64)status.st_ctime : 0;
   }

   // Get file size
   uint64 File::GetFileSize(const std::string &file)
   {
      struct stat status;
      return stat(file.c_str(), &status) == 0 ? (uint64)status.st_size : 0;
   }

   // get name of file user
   std::string File::GetFileUser(const std::string &file)
   {
      struct stat status;
      uint32 user_id = stat(file.c_str(), &status) == 0 ? status.st_uid : 0;
      struct passwd *pwd = ::getpwuid(user_id);
      if (!pwd)
      {
         TCERROR("TCBASE", System::GetLastErrorMessage().c_str());
         return "";
      }      
      return pwd->pw_name;
   }

   // get name of file group
   std::string File::GetFileGroup(const std::string &file)
   {
      struct stat status;
      uint32 group_id = stat(file.c_str(), &status) == 0 ? status.st_gid : 0;
      struct group *grp = ::getgrgid(group_id);
      if (!grp)
      {
         TCERROR("TCBASE", System::GetLastErrorMessage().c_str());
         return "";
      }      
      return grp->gr_name;
   }

   std::vector < std::string >
   File::GetFileListOfDirectory(const std::string & searchDirectory,
                                const std::string & searchExtension)
   {
      std::vector < std::string > fileList;
      std::string file;

      DIR *dirp = ::opendir(searchDirectory.c_str());
      if (!dirp) return fileList;

      struct dirent *de;
      while ((de = ::readdir(dirp)) != 0)
      {
         file = de->d_name;

         // skip subdirname ".":
         if (FileName::GetName(file) == "." || FileName::GetName(file) == "..")
         {
            continue;
         }

         std::string ext = FileName::GetExtension(file);
         if (searchExtension.size() && ext != searchExtension)
         {
            continue;
         }

         fileList.push_back(file);
      }
      ::closedir(dirp);
      return fileList;
   }

   void File::GetFileInfosOfDirectory(std::vector < FileInfo >& file_infos,
      const std::string & searchDirectory,
      const std::string& searchExtension)
   {
      DIR *dirp = ::opendir(searchDirectory.c_str());
      if (!dirp) 
      {
         return;
      }

      struct dirent *de;
      while ((de = ::readdir(dirp)) != 0)
      {
         FileInfo file_info;
         file_info.name = de->d_name;

         // skip subdirname ".":
         if (FileName::GetName(file_info.name) == "." || FileName::GetName(file_info.name) == "..")
         {
            continue;
         }

         std::string ext = FileName::GetExtension(file_info.name);
         if (searchExtension.size() && ext != searchExtension)
         {
            continue;
         }

         std::string full_file = FileName::AddFileNameAndPath(file_info.name, searchDirectory);

         struct stat info;
         if (stat(full_file.c_str(), &info) == 0)
         {
            file_info.is_directory  = S_ISDIR(info.st_mode) ? true : false;
            file_info.last_modified = info.st_mtime;
            file_info.file_size     = info.st_size;
            file_infos.push_back(file_info);
         }
      }
      ::closedir(dirp);
   }
}
#endif
