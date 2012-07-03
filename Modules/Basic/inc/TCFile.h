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
// Copyright (C) 2003 - 2012 Thomas Goessler. All Rights Reserved. 
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

#ifndef _TC_FILE_H_
#define _TC_FILE_H_

#include "TCSharedPtr.h"
#include "TCStlTypes.h"

#include <string>
#include <vector>

namespace tc
{
   /**
    * @addtogroup TC_BASE_IO
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of tc::file
    *
    * @author Thomas Goessler
    */

   /**
    * @brief Provides functions to get/create/set file information
    */
   namespace file
   {
      enum FileAttributes
      {
         FILEATTR_OWNER_WRITE          = 0x00000001,
         FILEATTR_OWNER_READ           = 0x00000002,
         FILEATTR_OWNER_EXEC           = 0x00000004,
         FILEATTR_GROUP_WRITE          = 0x00000008,
         FILEATTR_GROUP_READ           = 0x00000010,
         FILEATTR_GROUP_EXEC           = 0x00000020,
         FILEATTR_EVERYONE_WRITE       = 0x00000040,
         FILEATTR_EVERYONE_READ        = 0x00000080,
         FILEATTR_EVERYONE_EXEC        = 0x00000100,

         FILEATTR_READONLY             = 0x00000200,
         FILEATTR_ARCHIVE              = 0x00000400,
         FILEATTR_SYSTEM               = 0x00000800,
         FILEATTR_HIDDEN               = 0x00001000,

         FILEATTR_WRITE                = 0x00002000,
         FILEATTR_EXECUTE              = 0x00004000

      };

      /// @brief  return if file or directory exists
      TCBASE_API bool Exists(const std::string &file);
      /// @brief  return if its a file
      TCBASE_API bool IsFile(const std::string &file);
      /// @brief  return if file is a directory
      TCBASE_API bool IsDirectory(const std::string &file);
      /// @brief  return if the files/directory readable
      TCBASE_API bool IsReadable(const std::string &file);
      /// @brief  return if the files/directory writeable
      TCBASE_API bool IsWriteable(const std::string &file);
      /// @brief  return if the files/directory executable
      TCBASE_API bool IsExecutable(const std::string &file);

      /// @brief  Change the mode flags for this file
      TCBASE_API bool SetFileAttr(const std::string &file, uint32_t attr);
      /// @brief  Get file size
      TCBASE_API uint64_t GetFileSize(const std::string &file);

      /// @brief  returns time file was last modified   
      TCBASE_API uint64_t GetModificationTime(const std::string &file);
      /// @brief  returns time file was last accessed
      TCBASE_API uint64_t GetLastAccessTime(const std::string &file);
      /// @brief  returns time when created
      TCBASE_API uint64_t GetCreationTime(const std::string &file);
      /// @brief  returns time when touched
      TCBASE_API uint64_t GetTouchedTime(const std::string &file);

      /// @brief  get name of file user
      TCBASE_API std::string GetFileUser(const std::string &file);
      /// @brief  get name of file group
      TCBASE_API std::string GetFileGroup(const std::string &file);

      /// @brief  change to specified directory
      TCBASE_API bool ChangeDirectory(const std::string &directoryIn);
      /// @brief  returns current directory
      TCBASE_API std::string GetDirectory();
      /// @brief  returns all files with specified extension
      TCBASE_API StringVector GetFileListOfDirectory(const std::string &search_dir, const std::string &search_ext="");
      struct FileInfo
      {
         std::string name;
         uint64_t last_modified;
         bool is_directory;
         uint64_t file_size;
      };
      typedef std::vector < FileInfo > FileInfos;

      TCBASE_API void GetFileInfosOfDirectory(FileInfos& file_infos,
         const std::string& search_dir, 
         const std::string& search_ext="");
      TCBASE_API void FindFilesRecursive(FileInfos& files, const std::string& search_dir, 
         const std::string& search_ext="");


      /// @brief  create a empty file
      TCBASE_API bool Create(const std::string &file, bool removeOld=true);
      /// @brief  remove specified file
      TCBASE_API bool Remove(const std::string &file);

      class Progress
      {
      public:
         virtual void OnCurrentStatus(uint32_t percent_done) = 0;

         virtual ~Progress() {}
      };

      /// @brief  copy source file to destination file
      TCBASE_API bool Copy(const std::string& source, const std::string& dest,
         SharedPtr<Progress> copy_status=SharedPtr<Progress>());
      /// @brief  move source file to destination file
      TCBASE_API bool Move(const std::string& source, const std::string& dest);
      /// @brief  create a directory
      TCBASE_API bool CreateDir(const std::string &path);
      /// @brief  create a directory also if one of the parent directories does not exist
      TCBASE_API bool CreateDirRecursive(const std::string& path);
      /// @brief  remove a directory
      TCBASE_API bool RemoveDir(const std::string &path);
      /// @brief  remove a directory also if not empty
      TCBASE_API bool RemoveDirRecursive(const std::string &path);

   } // namespace file

   /**
    * @}
    */

} // namespace tc

#endif // _TC_FILE_H_
