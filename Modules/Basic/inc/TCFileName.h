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

#ifndef _TC_FILE_NAME_H_
#define _TC_FILE_NAME_H_

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
    * @brief This file provides the definition of tc::file_name
    *
    * @author Thomas Goessler
    */

   /**
    * @brief This namespace provided functions for manipulating file names or file paths
    */
   namespace file_name
   {
      /** @brief Get the path seperator string */
      TCBASE_API const char* GetPathSeparator();

       /**
       * @brief Get the extension of a filename
       * e.g. hallo.txt will return txt
       * @param file_name The full filename
       * @return The extension
       */
      TCBASE_API std::string GetExtension(const std::string& file_name);
       /**
       * @brief Get the extension of a filename
       * e.g. hallo.txt.gz will return txt if gz is passed as extension
       * @param file_name The full filename
       * @param additional_ext extension which the file must have to search for the next extension
       * @return The extension
       */
      TCBASE_API std::string GetExtensionIfExtension(const std::string& file_name, const std::string& additional_ext);
      /**
      * @brief Get the filename from a filename including the path
      *
      * @param file_name The full filename
      * @return The filename without the path
      */
      TCBASE_API std::string GetName(const std::string& file_name);
      /**
      * @brief Get the path from a filename including the path
      *
      * @param file_name The full filename
      * @return The path
      */
      TCBASE_API std::string GetPath(const std::string& file_name);

      /**
      * @brief Add filename and extension
      * @param file_name The filename to which to add the extension
      * @param extension The extension to add to the filename
      * @return The full filename including the extension
      */
      TCBASE_API std::string AddFileNameAndExtension(const std::string& file_name, 
                                                     const std::string& extension);
      /**
      * @brief Add a filename and a path
      * @param file_name The filename only
      * @param path The directory only
      * @return The full filename
      */
      TCBASE_API std::string AddFileNameAndPath(const std::string& file_name,
                                                const std::string& path);
      /**
      * @brief Add a path with another path
      * @param path1 The first path
      * @param path2 The second path
      * @return The new created path
      */
      TCBASE_API std::string AddPaths(const std::string& path1, const std::string& path2);

      /**
      * @brief Get the filename without its extension
      * @param file_name The full filename
      * @return The filename without the extension
      */
      TCBASE_API std::string RemoveExtension(const std::string& file_name);

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
      TCBASE_API std::string Relative(const std::string& base, const std::string& file);

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
      TCBASE_API std::string Simplify(const std::string& file);

      TCBASE_API void GetDirectoriesOfFileName(const std::string& file_name, 
          std::vector<std::string>& directories);
      TCBASE_API void GetDirectoriesOfPath(const std::string& path, 
          std::vector<std::string>& directories);
   } // namespace file_name

   /**
    * @}
    */

} // namespace tc

#endif // _TC_FILE_NAME_H_
