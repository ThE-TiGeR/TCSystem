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
//  $Id: TCFileWin32.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#include "TCDefines.h"

#if TCOS_WINDOWS
#include "TCFile.h"

#include "TCFileName.h"
#include "TCOutput.h"
#include "TCSystem.h"
#include "TCUtil.h"
#include "TCWString.h"

#include <windows.h>
#include <winbase.h>
#include <accctrl.h>
#include <aclapi.h>

#undef CreateDir
#undef GetUserName
#undef CreateFile
#undef GetClassName

#include "TCNewEnable.h"

namespace tc
{
   bool file::ChangeDirectory(const std::string& directory_in)
   {
      if (directory_in.empty())
      {
         return false;
      }
      std::wstring directory(wstring::ToString(directory_in));
      return ::SetCurrentDirectoryW(directory.c_str()) == TRUE;
   }

   std::string file::GetDirectory()
   {
      wchar_t buffer[1024] = L"";
      if (!::GetCurrentDirectoryW(static_cast<DWORD>(util::ArraySize(buffer)), buffer))
      {
         return "";
      }

      return wstring::ToString(buffer);
   }

   bool file::Exists(const std::string & file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      DWORD atts = ::GetFileAttributesW(file.c_str());

      return atts != 0xFFFFFFFF;
   }

   // Check whether its a directory
   bool file::IsDirectory(const std::string & file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      DWORD atts = ::GetFileAttributesW(file.c_str());

      return (atts != 0xFFFFFFFF) && (atts & FILE_ATTRIBUTE_DIRECTORY);
   }

   // Check whether its a file
   bool file::IsFile(const std::string & file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      DWORD atts = ::GetFileAttributesW(file.c_str());

      return (atts != 0xFFFFFFFF) && !(atts & FILE_ATTRIBUTE_DIRECTORY);
   }

   // Return 1 if file is readable
   bool file::IsReadable(const std::string &file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      DWORD atts = ::GetFileAttributesW(file.c_str());

      return (atts != 0xFFFFFFFF) && !(atts & FILE_WRITE_ATTRIBUTES);
   }

   // Return 1 if file is writeable
   bool file::IsWriteable(const std::string &file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      DWORD atts = ::GetFileAttributesW(file.c_str());

      return (atts != 0xFFFFFFFF) && (atts & FILE_WRITE_ATTRIBUTES);
   }

   // Return 1 if file is executable
   bool file::IsExecutable(const std::string &file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      DWORD atts = ::GetFileAttributesW(file.c_str());

      return (atts != 0xFFFFFFFF) && (atts & FILE_EXECUTE);
   }

   // Change the mode flags for this file
   bool file::SetFileAttr(const std::string &file_in, uint32_t attr)
   {
      std::wstring file(wstring::ToString(file_in));
      attr &= (static_cast<uint32_t>(FileAttributes::READONLY) | 
         static_cast<uint32_t>(FileAttributes::ARCHIVE) | 
         static_cast<uint32_t>(FileAttributes::SYSTEM) |
         static_cast<uint32_t>(FileAttributes::HIDDEN));
      return SetFileAttributesW(file.c_str(), attr) == TRUE ? true : false;
   }

   bool file::Remove(const std::string & file_in)
   {
      if (!Exists(file_in))
      {
         return false;
      }

      std::wstring file(wstring::ToString(file_in));
      if (IsDirectory(file_in))
      {
         return ::RemoveDirectoryW(file.c_str()) == TRUE;
      }
      else
      {
         return ::DeleteFileW(file.c_str()) == TRUE;
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
      SharedPtr<file::Progress> progress(*(SharedPtr<file::Progress>*)lpData);
      double val = (double)(TotalFileSize.QuadPart/TotalBytesTransferred.QuadPart) * 100;

      progress->OnCurrentStatus(uint32_t(val));

      return 0;
   }

   bool file::Copy(const std::string& source_in, const std::string& dest_in, SharedPtr<Progress> progress)
   {
      std::wstring source(wstring::ToString(source_in));
      if (!IsFile(source_in))
      {
         return false;
      }

      std::wstring dest(wstring::ToString(dest_in));
      if (progress)
      {
         if (::CopyFileExW(source.c_str(), dest.c_str(), &CopyProgress, &progress, 0, 0) != 0)
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
         return ::CopyFileExW(source.c_str(), dest.c_str(), 0, 0, 0, 0) != 0;
      }
   }

   bool file::Move(const std::string& source_in, const std::string& dest_in)
   {
      if (!IsFile(source_in))
      {
         return false;
      }

      std::wstring source(wstring::ToString(source_in));
      std::wstring dest(wstring::ToString(dest_in));
      return ::MoveFileExW(source.c_str(), dest.c_str(),
         MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH) == TRUE;
   }

   bool file::CreateDir(const std::string& path_in)
   {
      if (path_in.empty()) return false;

      std::wstring path(wstring::ToString(path_in));
      return ::CreateDirectoryW(path.c_str(), 0) == TRUE;
   }

   bool file::RemoveDir(const std::string& path_in)
   {
      if (path_in.empty()) return false;

      std::wstring path(wstring::ToString(path_in));
      return ::RemoveDirectoryW(path.c_str()) == TRUE;
   }

   // Return time file was last modified
   uint64_t file::GetModificationTime(const std::string &file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      HANDLE f = ::CreateFileW(file.c_str(),
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
   uint64_t file::GetLastAccessTime(const std::string &file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      HANDLE f = ::CreateFileW(file.c_str(),
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
   uint64_t file::GetCreationTime(const std::string &file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      HANDLE f = ::CreateFileW(file.c_str(),
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
   uint64_t file::GetFileSize(const std::string &file_in)
   {
      std::wstring file(wstring::ToString(file_in));
      HANDLE f = ::CreateFileW(file.c_str(),
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

   static std::string Win32FileInformation(const std::string &file_in, SECURITY_INFORMATION info_type)
   {
      std::wstring file(wstring::ToString(file_in));
      // Get the handle of the file object.
      HANDLE hFile = CreateFileW(
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
         TCERRORS("TCBASE", system::GetLastErrorMessage().c_str());
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
         TCERRORS("TCBASE", system::GetLastErrorMessage().c_str());
         return "";
      }
      ::CloseHandle(hFile);
      hFile = INVALID_HANDLE_VALUE;
      ::GlobalFree(pSD);

      SID_NAME_USE eUse = SidTypeUnknown;
      char AcctName[256];
      char DomainName[256];
      DWORD dwAcctName = static_cast<DWORD>(util::ArraySize(AcctName));
      DWORD dwDomainName = static_cast<DWORD>(util::ArraySize(DomainName));
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
         TCERRORS("TCBASE", system::GetLastErrorMessage().c_str());
         return "";
      }
      ::GlobalFree(pSidOwner);

      return std::string(DomainName) + "\\" + AcctName;
   }

   // get name of file user
   std::string file::GetFileUser(const std::string &file)
   {
      return Win32FileInformation(file, OWNER_SECURITY_INFORMATION);
   }

   // get name of file group
   std::string file::GetFileGroup(const std::string &file)
   {
      return Win32FileInformation(file, GROUP_SECURITY_INFORMATION);
   }

   std::vector < std::string >
   file::GetFileListOfDirectory(const std::string & searchDirectory,
                                const std::string & searchExtension)
   {
      std::vector < std::string > fileList;
      std::string file;

      HANDLE findFile;
      WIN32_FIND_DATAW findData;
      std::string search_dir;
      if (searchExtension.empty())
      {
         search_dir = file_name::AddFileNameAndPath("*", searchDirectory);
      }
      else
      {
         search_dir = file_name::AddFileNameAndPath("*." + searchExtension, searchDirectory);
      }
      std::wstring wsearch_dir = wstring::ToString(search_dir);
      findFile = ::FindFirstFileW(wsearch_dir.c_str(), &findData);
      if (findFile == INVALID_HANDLE_VALUE) return fileList;

      do
      {
         file = wstring::ToString(findData.cFileName);
         if (file_name::GetName(file) == "." || file_name::GetName(file) == "..")
         {
            continue;
         }

         fileList.push_back(file);
      }
      while (::FindNextFileW(findFile, &findData));

      ::FindClose(findFile);
      return fileList;
   }

   
   void file::GetFileInfosOfDirectory(std::vector < FileInfo >& file_infos,
      const std::string & searchDirectory,
      const std::string& searchExtension)
   {
      HANDLE findFile;
      WIN32_FIND_DATAW findData;
      std::string search_dir;
      if (searchExtension.empty())
      {
         search_dir = file_name::AddFileNameAndPath("*", searchDirectory);
      }
      else
      {
         search_dir = file_name::AddFileNameAndPath("*." + searchExtension, searchDirectory);
      }
      std::wstring wsearch_dir = wstring::ToString(search_dir);
      findFile = ::FindFirstFileW(wsearch_dir.c_str(), &findData);
      if (findFile == INVALID_HANDLE_VALUE) return;

      do
      {
         FileInfo file_info;
         file_info.name = wstring::ToString(findData.cFileName);
         if (file_name::GetName(file_info.name) == "." || 
             file_name::GetName(file_info.name) == "..")
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
      while (::FindNextFileW(findFile, &findData));

      ::FindClose(findFile);
   }
}
#endif
