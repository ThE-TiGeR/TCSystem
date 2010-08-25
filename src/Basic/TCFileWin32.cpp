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
//  $Id: TCFileWin32.cpp 959 2010-03-16 20:53:52Z the_____tiger $
//*******************************************************************************
#if TCOS_WINDOWS
#include "TCFile.h"

#include "TCFileName.h"
#include "TCOutput.h"
#include "TCSystem.h"
#include "TCUtil.h"

#include <windows.h>
#include <winbase.h>
#include <accctrl.h>
#include <aclapi.h>

#undef CreateDirectory
#undef GetUserName
#undef CreateFile
#undef GetClassName

#include "TCNewEnable.h"

namespace TC
{
   bool File::ChangeDirectory(const std::string& directory)
   {
      if (directory.empty())
      {
         return false;
      }
      return ::SetCurrentDirectoryA(directory.c_str()) == TRUE;
   }

   std::string File::GetDirectory()
   {
      char buffer[512] = "";
      if (!::GetCurrentDirectoryA(static_cast<DWORD>(Util::ArraySize(buffer)), buffer))
      {
         return "";
      }

      return buffer;
   }

   bool File::Exists(const std::string & file)
   {
      DWORD atts = ::GetFileAttributesA(file.c_str());

      return atts != 0xFFFFFFFF;
   }

   // Check whether its a directory
   bool File::IsDirectory(const std::string & file)
   {
      DWORD atts = ::GetFileAttributesA(file.c_str());

      return (atts != 0xFFFFFFFF) && (atts & FILE_ATTRIBUTE_DIRECTORY);
   }

   // Check whether its a file
   bool File::IsFile(const std::string & file)
   {
      DWORD atts = ::GetFileAttributesA(file.c_str());

      return (atts != 0xFFFFFFFF) && !(atts & FILE_ATTRIBUTE_DIRECTORY);
   }

   // Return 1 if file is readable
   bool File::IsReadable(const std::string &file)
   {
      DWORD atts = ::GetFileAttributesA(file.c_str());

      return (atts != 0xFFFFFFFF) && !(atts & FILE_WRITE_ATTRIBUTES);
   }

   // Return 1 if file is writeable
   bool File::IsWriteable(const std::string &file)
   {
      DWORD atts = ::GetFileAttributesA(file.c_str());

      return (atts != 0xFFFFFFFF) && (atts & FILE_WRITE_ATTRIBUTES);
   }

   // Return 1 if file is executable
   bool File::IsExecutable(const std::string &file)
   {
      DWORD atts = ::GetFileAttributesA(file.c_str());

      return (atts != 0xFFFFFFFF) && (atts & FILE_EXECUTE);
   }

   // Change the mode flags for this file
   bool File::SetFileAttr(const std::string &file, uint32 attr)
   {
      attr &= (FILEATTR_READONLY | FILEATTR_ARCHIVE | FILEATTR_SYSTEM | FILEATTR_HIDDEN) ;
      return SetFileAttributesA(file.c_str(), attr) == TRUE ? true : false;
   }

   bool File::Remove(const std::string & file)
   {
      if (!Exists(file))
      {
         return false;
      }

      if (IsDirectory(file))
      {
         return ::RemoveDirectoryA(file.c_str()) == TRUE;
      }
      else
      {
         return ::DeleteFileA(file.c_str()) == TRUE;
      }
   }

   DWORD WINAPI CopyProgress(
      LARGE_INTEGER TotalFileSize,
      LARGE_INTEGER TotalBytesTransferred,
      LARGE_INTEGER /*StreamSize*/,
      LARGE_INTEGER /*StreamBytesTransferred*/,
      DWORD /*dwStreamNumber*/,
      DWORD /*dwCallbackReason*/,
      HANDLE /*hSourceFile*/,
      HANDLE /*hDestinationFile*/,
      LPVOID lpData
      )
   {
      SharedPtr<File::Progress> progress(*(SharedPtr<File::Progress>*)lpData);
      double val = (double)(TotalFileSize.QuadPart/TotalBytesTransferred.QuadPart) * 100;

      progress->OnCurrentStatus(uint32(val));

      return 0;
   }

   bool File::Copy(const std::string& source, const std::string& dest,
      SharedPtr<Progress> progress)
   {
      if (!IsFile(source))
      {
         return false;
      }

      if (progress)
      {
         if (::CopyFileExA(source.c_str(), dest.c_str(), 
            &CopyProgress, &progress, 0, 0) != 0)
         {
            progress->OnCurrentStatus(100);
            return true;
         }
         else
         {
            return false;
         }

      }
      else
      {
         return ::CopyFileExA(source.c_str(), dest.c_str(), 0, 0, 0, 0) != 0;
      }
   }

   bool File::Move(const std::string& source, const std::string& dest)
   {
      if (!IsFile(source))
      {
         return false;
      }

      return ::MoveFileExA(source.c_str(), dest.c_str(), 
         MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH) == TRUE;
   }

   bool File::CreateDirectory(const std::string& path)
   {
      if (path.empty()) return false;

      return ::CreateDirectoryA(path.c_str(), 0) == TRUE;
   }

   // Return time file was last modified
   uint64 File::GetModificationTime(const std::string &file)
   {
      HANDLE f = ::CreateFileA(file.c_str(),
         FILE_READ_ATTRIBUTES,
         FILE_SHARE_READ,
         0,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         0);
      if (f == INVALID_HANDLE_VALUE)
      {
         return 0;
      }

      FILETIME ftime;
      if (!::GetFileTime(f, 0, 0, &ftime))
      {
         ::CloseHandle(f);
         return 0;
      }
      ::CloseHandle(f);

      ULARGE_INTEGER ltime;
      ltime.LowPart  = ftime.dwLowDateTime;
      ltime.HighPart = ftime.dwHighDateTime;
      return ltime.QuadPart;
   }

   // Return time file was last accessed
   uint64 File::GetLastAccessTime(const std::string &file)
   {
      HANDLE f = ::CreateFileA(file.c_str(),
         FILE_READ_ATTRIBUTES,
         FILE_SHARE_READ,
         0,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         0);
      if (f == INVALID_HANDLE_VALUE)
      {
         return 0;
      }

      FILETIME ftime;
      if (!::GetFileTime(f, 0, &ftime, 0))
      {
         ::CloseHandle(f);
         return 0;
      }
      ::CloseHandle(f);

      ULARGE_INTEGER ltime;
      ltime.LowPart  = ftime.dwLowDateTime;
      ltime.HighPart = ftime.dwHighDateTime;
      return ltime.QuadPart;
   }

   // Return time when created
   uint64 File::GetCreationTime(const std::string &file)
   {
      HANDLE f = ::CreateFileA(file.c_str(),
         FILE_READ_ATTRIBUTES,
         FILE_SHARE_READ,
         0,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         0);
      if (f == INVALID_HANDLE_VALUE)
      {
         return 0;
      }

      FILETIME ftime;
      if (!::GetFileTime(f, &ftime, 0, 0))
      {
         ::CloseHandle(f);
         return 0;
      }
      ::CloseHandle(f);

      ULARGE_INTEGER ltime;
      ltime.LowPart  = ftime.dwLowDateTime;
      ltime.HighPart = ftime.dwHighDateTime;
      return ltime.QuadPart;
   }

   // Get file size
   uint64 File::GetFileSize(const std::string &file)
   {
      HANDLE f = ::CreateFileA(file.c_str(),
         FILE_READ_ATTRIBUTES,
         FILE_SHARE_READ,
         0,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         0);
      if (f == INVALID_HANDLE_VALUE)
      {
         return 0;
      }

      DWORD high_size = 0;
      DWORD low_size = ::GetFileSize(f, &high_size);
      if (low_size == INVALID_FILE_SIZE)
      {
         return 0;
      }

      LARGE_INTEGER ltime;
      ltime.LowPart  = low_size;
      ltime.HighPart = high_size;
      return ltime.QuadPart;
   }

   static std::string Win32FileInformation(const std::string &file, SECURITY_INFORMATION info_type)
   {
      // Get the handle of the file object.
      HANDLE hFile = CreateFileA(
         file.c_str(),
         FILE_READ_ATTRIBUTES,
         FILE_SHARE_READ,
         0,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         0);

      // Check GetLastError for CreateFile error code.
      if (hFile == INVALID_HANDLE_VALUE)
      {
         TCERROR("TCBASE", System::GetLastErrorMessage().c_str());
         return "";
      }

      // Allocate memory for the SID structure.
      PSID pSidOwner = (PSID)GlobalAlloc(GMEM_FIXED, sizeof(PSID));

      // Allocate memory for the security descriptor structure.
      PSECURITY_DESCRIPTOR pSD = 0;
      pSD = (PSECURITY_DESCRIPTOR)::GlobalAlloc(GMEM_FIXED,sizeof(PSECURITY_DESCRIPTOR));

      // Get the owner SID of the file.
      // Check GetLastError for GetSecurityInfo error condition.
      if (GetSecurityInfo(hFile, SE_FILE_OBJECT, info_type,
         &pSidOwner, 0, 0, 0, &pSD) != ERROR_SUCCESS)
      {
         TCERROR("TCBASE", System::GetLastErrorMessage().c_str());
         return "";
      }
      ::CloseHandle(hFile);
      hFile = INVALID_HANDLE_VALUE;
      ::GlobalFree(pSD);

      SID_NAME_USE eUse = SidTypeUnknown;
      char AcctName[256];
      char DomainName[256];
      DWORD dwAcctName = static_cast<DWORD>(Util::ArraySize(AcctName));
      DWORD dwDomainName = static_cast<DWORD>(Util::ArraySize(DomainName));
      // call to LookupAccountSid to get the account name.
      if (!LookupAccountSidA(
         0,                          // name of local or remote computer
         pSidOwner,                     // security identifier
         AcctName,                      // account name buffer
         &dwAcctName,                   // size of account name buffer 
         DomainName,                    // domain name
         &dwDomainName,                 // size of domain name buffer
         &eUse))                        // SID type
      {
         TCERROR("TCBASE", System::GetLastErrorMessage().c_str());
         return "";
      }
      ::GlobalFree(pSidOwner);

      return std::string(DomainName) + "\\" + AcctName;
   }

   // get name of file user
   std::string File::GetFileUser(const std::string &file)
   {
      return Win32FileInformation(file, OWNER_SECURITY_INFORMATION);
   }

   // get name of file group
   std::string File::GetFileGroup(const std::string &file)
   {
      return Win32FileInformation(file, GROUP_SECURITY_INFORMATION);
   }

   std::vector < std::string >
   File::GetFileListOfDirectory(const std::string & searchDirectory,
                                const std::string & searchExtension)
   {
      std::vector < std::string > fileList;
      std::string file;

      HANDLE findFile;
      WIN32_FIND_DATAA findData;
      std::string search_dir;
      if (searchExtension.empty())
      {
         search_dir = FileName::AddFileNameAndPath("*", searchDirectory);
      }
      else
      {
         search_dir = FileName::AddFileNameAndPath("*." + searchExtension, searchDirectory);
      }
      findFile = ::FindFirstFileA(search_dir.c_str(), &findData);
      if (findFile == INVALID_HANDLE_VALUE) return fileList;

      do
      {
         file = findData.cFileName;
         if (FileName::GetName(file) == "." || FileName::GetName(file) == "..")
         {
            continue;
         }

         fileList.push_back(file);
      }
      while (::FindNextFileA(findFile, &findData));

      ::FindClose(findFile);
      return fileList;
   }

   
   void File::GetFileInfosOfDirectory(std::vector < FileInfo >& file_infos,
      const std::string & searchDirectory,
      const std::string& searchExtension)
   {
      HANDLE findFile;
      WIN32_FIND_DATAA findData;
      std::string search_dir;
      if (searchExtension.empty())
      {
         search_dir = FileName::AddFileNameAndPath("*", searchDirectory);
      }
      else
      {
         search_dir = FileName::AddFileNameAndPath("*." + searchExtension, searchDirectory);
      }
      findFile = ::FindFirstFileA(search_dir.c_str(), &findData);
      if (findFile == INVALID_HANDLE_VALUE) return;

      do
      {
         FileInfo file_info;
         file_info.name = findData.cFileName;
         if (FileName::GetName(file_info.name) == "." || 
             FileName::GetName(file_info.name) == "..")
         {
            continue;
         }

         LARGE_INTEGER last_write_time;
         last_write_time.LowPart  = findData.ftLastWriteTime.dwLowDateTime;
         last_write_time.HighPart = findData.ftLastWriteTime.dwHighDateTime;
         file_info.last_modified  = last_write_time.QuadPart;
         file_info.is_directory   = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
         LARGE_INTEGER file_size;
         file_size.LowPart   = findData.nFileSizeLow;
         file_size.HighPart  = findData.nFileSizeHigh;
         file_info.file_size = file_size.QuadPart;
         file_infos.push_back(file_info);
      }
      while (::FindNextFileA(findFile, &findData));

      ::FindClose(findFile);
   }
}
#endif
