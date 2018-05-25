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
//  $Id: TCStringTraits.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCTypes.h"

#include <cctype>
#include <cstdio>

#include "TCNewEnable.h"

namespace tc
{
   class StringTraits
   {
      typedef char ElemType;
   public:

      static ElemType ToUpper(const ElemType& elem)
      {
         return static_cast<ElemType>(std::toupper(elem));
      }

      static ElemType ToLower(const ElemType& elem)
      {
         return static_cast<ElemType>(std::tolower(elem));
      }

      static int VsnPrintf(ElemType* elem, std::size_t size, const ElemType* format, va_list arguments)
      {
#ifdef _MSC_VER
         int len = _vsnprintf(elem, size, format, arguments);
#elif defined TCOS_CYGWIN
         int len = vsnprintf(elem, size, format, arguments);
#elif defined TCOS_ANDROID
         int len = vsnprintf(elem, size, format, arguments);
#else
         int len = std::vsnprintf(elem, size, format, arguments);
#endif
         if (len == -1 || static_cast<std::size_t>(len) >= size)
         {
            elem[size-1] = 0;
            return -1;
         }
         return len;
      }

      static bool IsSpace(int elem)
      {
         if (elem > 0 && elem <= 255)
         {
            return std::isspace(elem) != 0;
         }
         return false;
      }
   };
}
