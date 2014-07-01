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

#include "TCFileName.h"

#include "TCString.h"
#include "TCStringTraits.h"
#include "TCFileNameImp.h"

#include <cctype>

#include "TCNewEnable.h"

namespace tc
{
   class FileNameTraits
   {
   public:
      static const char*  EXTENSION_SEPERATOR;
      static const char*  PATH_SEPERATOR;
      static const char*  PATH_SEPERATOR_WRONG;

      static void ReplaceString(std::string& data, const std::string& token, const std::string& replace)
      {
         data = string::Replace(data, token, replace);
      }
   };

   const char* FileNameTraits::EXTENSION_SEPERATOR = ".";
   const char* FileNameTraits::PATH_SEPERATOR = "/";
   const char* FileNameTraits::PATH_SEPERATOR_WRONG = "\\";

   typedef FileNameImpTmpl<std::string, FileNameTraits, StringTraits> FileNameImp;

   std::string file_name::GetExtension(const std::string& file_name)
   {
      return FileNameImp::GetExtension(file_name);
   }

   std::string file_name::GetExtensionIfExtension(const std::string& file_name, const std::string& additional_ext)
   {
      return FileNameImp::GetExtensionIfExtension(file_name, additional_ext);
   }

   std::string file_name::GetName(const std::string& file_name)
   {
      return FileNameImp::GetName(file_name);
   }

   std::string file_name::GetPath(const std::string& file_name)
   {
      return FileNameImp::GetPath(file_name);
   }

   std::string file_name::RemoveExtension(const std::string& file_name)
   {
      return FileNameImp::RemoveExtension(file_name);
   }

   std::string file_name::AddPaths(const std::string& path1, const std::string& path2)
   {
      return FileNameImp::AddPaths(path1, path2);
   }

   std::string file_name::AddFileNameAndPath(const std::string& file_name, const std::string& path)
   {
      return FileNameImp::AddFileNameAndPath(file_name, path);
   }

   std::string file_name::AddFileNameAndExtension(const std::string& file_name, const std::string& ext)
   {
      return FileNameImp::AddFileNameAndExtension(file_name, ext);
   }

   std::string file_name::Relative(const std::string& base, const std::string& file)
   {
      return FileNameImp::Relative(base, file);
   }

   std::string file_name::Simplify(const std::string& file)
   {
      return FileNameImp::Simplify(file);
   }

   void file_name::GetDirectoriesOfFileName(const std::string& file_name,
      std::vector<std::string>& directories)
   {
      string::Split(GetPath(file_name).c_str(), FileNameTraits::PATH_SEPERATOR, directories);
   }

   void file_name::GetDirectoriesOfPath(const std::string& path,
      std::vector<std::string>& directories)
   {
      string::Split(Simplify(path).c_str(), FileNameTraits::PATH_SEPERATOR, directories);
   }

   const char* file_name::GetPathSeparator()
   {
      return FileNameTraits::PATH_SEPERATOR;
   }

}
