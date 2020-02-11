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
//  $Id: TCWFileName.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TC_WFILE_NAME_H_
#define _TC_WFILE_NAME_H_

#include "TCTypes.h"

#include <string>
#include <vector>

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition tc::wfile_name
    *
    * @author Thomas Goessler
    */

   /**
   * @brief This namespace provided functions for manipulating file names or file paths
   */
   namespace wfile_name
   {
       /** @brief Get the path seperator string */
       TCBASE_API const wchar_t* GetPathSeparator();

       /**
       * @brief Get the extension of a filename
       * @param file_name The full filename
       * @return The extension
       */
       TCBASE_API std::wstring GetExtension(const std::wstring& file_name);
       /**
       * @brief Get the extension of a filename
       * e.g. hallo.txt.gz will return txt if gz is passed as extension
       * @param file_name The full filename
       * @param additional_ext extension which the file must have to search for the next extension
       * @return The extension
       */
      TCBASE_API std::wstring GetExtensionIfExtension(const std::wstring& file_name, const std::wstring& additional_ext);

       /**
       * @brief Get the filename from a filename including the path
       *
       * @param file_name The full filename
       * @return The filename without the path
       */
       TCBASE_API std::wstring GetName(const std::wstring& file_name);
       /**
       * @brief Get the path from a filename including the path
       *
       * @param file_name The full filename
       * @return The path
       */
       TCBASE_API std::wstring GetPath(const std::wstring& file_name);

       /**
       * @brief Add filename and extension
       * @param file_name The filename to which to add the extension
       * @param extension The extension to add to the filename
       * @return The full filename including the extension
       */
       TCBASE_API std::wstring AddFileNameAndExtension(const std::wstring& file_name, 
           const std::wstring& extension);
       /**
       * @brief Add a filename and a path
       * @param file_name The filename only
       * @param path The directory only
       * @return The full filename
       */
       TCBASE_API std::wstring AddFileNameAndPath(const std::wstring& file_name,
           const std::wstring& path);
       /**
       * @brief Add a path with another path
       * @param path1 The first path
       * @param path2 The second path
       * @return The new created path
       */
       TCBASE_API std::wstring AddPaths(const std::wstring& path1, const std::wstring& path2);

       /**
       * @brief Get the filename without its extension
       * @param file_name The full filename
       * @return The filename without the extension
       */
       TCBASE_API std::wstring RemoveExtension(const std::wstring& file_name);

      /**
       * Examples:
       *
       * Base       File         Result      Comment
       * /          /a/b         /a/b        Branch point is /
       * /p/q/r     /a/b/c       /a/b/c      Branch point is /
       * /a/b/c     /a/b/c/d     d           Branch point is /a/b/c
       * /a/b/c/    /a/b/c/d     d           Branch point is /a/b/c
       * /a/b/c/d   /a/b/c       ../         Branch point is /a/b/c
       * /a/b/c/d   /a/b/q       ../../q     Branch point is /a/b
       * /a/b/c     /a/b/c       .           Equal
       * /a/b/c/    /a/b/c/      .           Equal
       * ../a/b/c   ../a/b/c/d   d           Branch point is ../a/b/c
       * ./a        ./b          ../b        Branch point assumed to be ..
       * a          b            ../b        Branch point assumed to be ..
       * Return relative path of file to given base directory
       */
      TCBASE_API std::wstring Relative(const std::string& base,const std::wstring& file);

       /**
       * Simplify a file path; the path will remain relative if it was relative,
       * or absolute if it was absolute.  Also, a trailing "/" will be preserved
       * as this is important in other functions.
       *
       * Examples:
       *
       *  /aa/bb/../cc    -> /aa/cc
       *  /aa/bb/../cc/   -> /aa/cc/
       *  /aa/bb/../..    -> /
       *  ../../bb        -> ../../bb
       *  ../../bb/       -> ../../bb/
       *  /../            -> /
       *  ./aa/bb/../../  -> ./
       *  a/..            -> .
       *  a/../           -> ./
       *  ./a             -> ./a
       *  /////./././     -> /
       *  c:/../          -> c:/
       *  c:a/..          -> c:
       *  /.              -> /
       */
       TCBASE_API std::wstring Simplify(const std::wstring& file);

       TCBASE_API std::vector<std::wstring> GetDirectoriesOfFileName(const std::wstring& file_name);
       TCBASE_API std::vector<std::wstring> GetDirectoriesOfPath(const std::wstring& path);
   } // namespace wfile_name


   /**
    * @}
    */

} // namespace tc

#endif // _TC_WFILE_NAME_H_
