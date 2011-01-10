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
//  $Id$
//*******************************************************************************

#include "TCTypes.h"

#include <cctype>

namespace TC
{
   template <typename STRING_TYPE, typename FILE_NAME_TRAITS, typename STRING_TRAITS>
   class FileNameImpTmpl
   {
   public:
      static STRING_TYPE GetExtension(const STRING_TYPE& file_name)
      {
         STRING_TYPE name = GetName(file_name);

         typename STRING_TYPE::size_type pos = name.rfind('.');
         if (pos == STRING_TYPE::npos)
         {
            return STRING_TYPE();
         }

         return name.substr(pos+1);
      }

      static STRING_TYPE GetExtensionIfExtension(const STRING_TYPE& file_name, const STRING_TYPE& additional_ext)
      {
         if (GetExtension(file_name) != additional_ext)
         {
            return STRING_TYPE();
         }

         STRING_TYPE tmp_name = RemoveExtension((file_name));
         return GetExtension(tmp_name);
      }

      static STRING_TYPE GetName(const STRING_TYPE& file_name)
      {
         STRING_TYPE name = Simplify(file_name);

         typename STRING_TYPE::size_type pos = name.rfind(FILE_NAME_TRAITS::PATH_SEPERATOR);
         if (pos != STRING_TYPE::npos)
         {
            name = name.substr(pos+1);
         }

         return name;
      }

      static STRING_TYPE GetPath(const STRING_TYPE& file_name_in)
      {
         STRING_TYPE file_name = Simplify(file_name_in);
         typename STRING_TYPE::size_type pos = file_name.rfind(FILE_NAME_TRAITS::PATH_SEPERATOR);
         if (pos == STRING_TYPE::npos)
         {
            return STRING_TYPE();
         }

         return Simplify(file_name.substr(0, pos+1));
      }

      static STRING_TYPE RemoveExtension(const STRING_TYPE& file_name_in)
      {
         STRING_TYPE path = GetPath(file_name_in);
         STRING_TYPE name = GetName(file_name_in);

         typename STRING_TYPE::size_type pos = name.rfind('.');
         if (pos != STRING_TYPE::npos)
         {
            name = name.substr(0, pos);
         }

         return AddFileNameAndPath(name, path);
      }

      static STRING_TYPE AddPaths(const STRING_TYPE& path1,
         const STRING_TYPE& path2)
      {
         return AddFileNameAndPath(path2, path1);
      }

      static STRING_TYPE AddFileNameAndPath(const STRING_TYPE& file_name_in,
         const STRING_TYPE& path_in)
      {
         STRING_TYPE path = Simplify(path_in);
         STRING_TYPE file_name = Simplify(file_name_in);
         if (path.empty())
         {
            return file_name;
         }
         else if (path[path.length()-1] == FILE_NAME_TRAITS::PATH_SEPERATOR[0])
         {
            return Simplify(path + file_name);
         }
         else
         {
            return Simplify(path + FILE_NAME_TRAITS::PATH_SEPERATOR + file_name);
         }
      }

      static STRING_TYPE AddFileNameAndExtension(const STRING_TYPE& file_name_in,
         const STRING_TYPE& ext)
      {
         STRING_TYPE file_name = Simplify(file_name_in);
         if (!ext.length())
         {
            return file_name;
         }

         if (file_name.length() &&
            file_name[file_name.length()-1] == FILE_NAME_TRAITS::EXTENSION_SEPERATOR[0])
         {
            return file_name + ext;
         }
         else
         {
            return file_name + FILE_NAME_TRAITS::EXTENSION_SEPERATOR + ext;
         }
      }

      static bool IsPathSeperator(typename STRING_TYPE::value_type c)
      {
         return c == FILE_NAME_TRAITS::PATH_SEPERATOR[0];
      }

      // Does file represent topmost directory
      static bool IsTopDirectory(const STRING_TYPE& file)
      {
#ifndef TCOS_WINDOWS
         return IsPathSeperator(file[0]) && file[1]=='\0';
#else
         return (IsPathSeperator(file[0]) && 
            (file[1]=='\0' || (IsPathSeperator(file[1]) && file[2]=='\0'))) || 
            (std::isalpha(static_cast<int>(file[0])) && 
            file[1]==':' && 
            (file[2]=='\0' || (IsPathSeperator(file[2]) && file[3]=='\0')));
#endif
      }

      static STRING_TYPE Relative(const STRING_TYPE& base_in, const STRING_TYPE& file_in)
      {
         STRING_TYPE base(Simplify(base_in));
         STRING_TYPE file(Simplify(file_in));

         if(!base.empty() && !IsTopDirectory(base))
         {
            sint32 p=0,q=0,bp=0,bq=0;

            // Find branch point
#ifdef TCOS_WINDOWS
            while(base[p] && 
                 ((STRING_TRAITS::ToLower(base[p])==STRING_TRAITS::ToLower(file[q])) || 
                 (IsPathSeperator(base[p]) && IsPathSeperator(file[q]))))
            {
               if(IsPathSeperator(base[p]))
               {
                  bp=p;
                  bq=q;
                  while(0<p && IsPathSeperator(base[p+1])) p++;           // Eat multiple slashes, but not the UNC "\\" at the start
                  while(0<q && IsPathSeperator(file[q+1])) q++;
               }
               p++;
               q++;
            }
#else
            while(base[p] && (base[p]==file[q]))
            {
               if(IsPathSeperator(base[p]))
               {
                  bp=p;
                  bq=q;
                  while(IsPathSeperator(base[p+1])) p++;                  // Eat multiple slashes
                  while(IsPathSeperator(file[q+1])) q++;
               }
               p++;
               q++;
            }
#endif

            // Common prefix except for trailing path separator
            if((base[p]=='\0' || IsPathSeperator(base[p])) && 
               (file[q]=='\0' || IsPathSeperator(file[q])))
            {
               bp=p;
               bq=q;
            }

            // If branch point is not root
#ifdef TCOS_WINDOWS
            if(!((IsPathSeperator(base[0]) && 
               (bp==0 || (IsPathSeperator(base[1]) && bp==1))) || 
               (std::isalpha(static_cast<int>(base[0])) && 
               base[1]==':' && (bp==1 || (IsPathSeperator(base[2]) && bp==2)))))
#else
            if(!(IsPathSeperator(base[0]) && bp==0))
#endif
            {
               // Strip leading path character off, if any
               while(IsPathSeperator(file[bq])) bq++;

               // Non trivial
               if(file[bq])
               {
                  STRING_TYPE result;

                  // Up to branch point
                  while(base[bp])
                  {
                     while(IsPathSeperator(base[bp])) bp++;
                     if(base[bp])
                     {
                        result.append("..");
                        result.append(FILE_NAME_TRAITS::PATH_SEPERATOR);
                        while(base[bp] && !IsPathSeperator(base[bp])) bp++;
                     }
                  }

                  // Append tail end
                  result.append(&file[bq]);
                  return result;
               }
               return ".";
            }
         }
         return file;
      }

      static STRING_TYPE Simplify(const STRING_TYPE& file)
      {
         if(!file.empty())
         {
            STRING_TYPE result(file);
            FILE_NAME_TRAITS::ReplaceString(result, 
               FILE_NAME_TRAITS::PATH_SEPERATOR_WRONG, FILE_NAME_TRAITS::PATH_SEPERATOR);
            sint32 p=0;
            sint32 q=0;
            sint32 s;
#ifdef TCOS_WINDOWS
            if(IsPathSeperator(result[q]))
            {         // UNC
               result[p++]=FILE_NAME_TRAITS::PATH_SEPERATOR[0]; q++;
               if(IsPathSeperator(result[q]))
               {
                  result[p++]=FILE_NAME_TRAITS::PATH_SEPERATOR[0];
                  while(IsPathSeperator(result[q])) q++;
               }
            }
            else if(std::isalpha(static_cast<int>(result[q])) && result[q+1]==':')
            {
               result[p++]=result[q++]; result[p++]=':'; q++;
               if(IsPathSeperator(result[q]))
               {
                  result[p++]=FILE_NAME_TRAITS::PATH_SEPERATOR[0];
                  while(IsPathSeperator(result[q])) q++;
               }
            }
#else
            if(IsPathSeperator(result[q]))
            {
               result[p++]=FILE_NAME_TRAITS::PATH_SEPERATOR[0];
               while(IsPathSeperator(result[q])) q++;
            }
#endif
            s=p;
            while(result[q])
            {
               while(result[q] && !IsPathSeperator(result[q]))
               {
                  result[p++]=result[q++];
               }
               if(IsPathSeperator(result[q]))
               {
                  result[p++]=FILE_NAME_TRAITS::PATH_SEPERATOR[0];
                  while(IsPathSeperator(result[q])) q++;
               }
               if(2<=p && IsPathSeperator(result[p-2]) && result[p-1]=='.')
               {   // Case "xxx/."
                  p--;
                  if(s<p) p--;
               }
               else if(3<=p && IsPathSeperator(result[p-3]) && result[p-2]=='.' && IsPathSeperator(result[p-1]))
               {    // Case "xxx/./"
                  p-=2;
               }
               else if(3<=p && IsPathSeperator(result[p-3]) && result[p-2]=='.' && result[p-1]=='.' && !(((6<=p && IsPathSeperator(result[p-6])) || 5==p) && result[p-5]=='.' && result[p-4]=='.'))
               { // Case "xxx/.."
                  p-=2;
                  if(s<p)
                  {                // Pathological case "/.." will become "/"
                     p--;
                     while(s<p && !IsPathSeperator(result[p-1])) p--;
                     if(s<p && IsPathSeperator(result[p-1])) p--;
                     if(p==0)
                     {                             // Don't allow empty path
                        result[p++]='.';
                     }
                  }
               }
               else if(4<=p && IsPathSeperator(result[p-4]) && result[p-3]=='.' && result[p-2]=='.' && IsPathSeperator(result[p-1]) && !(((7<=p && IsPathSeperator(result[p-7])) || 6==p) && result[p-6]=='.' && result[p-5]=='.'))
               {       // Case "xxx/../"
                  p-=3;
                  if(s<p)
                  {                // Pathological case "/../" will become "/"
                     p--;
                     while(s<p && !IsPathSeperator(result[p-1])) p--;
                     if(p==0)
                     {                             // Don't allow empty path
                        result[p++]='.';
                        result[p++]=FILE_NAME_TRAITS::PATH_SEPERATOR[0];                // Keep trailing "/" around
                     }
                  }
               }
            }
            return result.substr(0, p);
         }
         return STRING_TYPE();
      }
   };
}
