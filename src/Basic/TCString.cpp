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

#include "TCString.h"

#include "TCStringImp.h"
#include "TCStringTraits.h"
#include "TCSharedPtr.h"
#include "TCUtil.h"

#include <cctype>

#include "TCNewEnable.h"

namespace TC
{
   typedef StringImpTmpl<std::string, StringTraits> StringImp;

   std::string String::ToString(char val)
   {
      char string[2] = {val, 0};

      return string;
   }

   std::string String::ToString(bool val)
   {
      return val ? "1" : "0";
   }

   std::string String::ToString(uint16 val)
   {
      char string[32];
      Snprintf(string, Util::ArraySize(string), "%hu", val);

      return string;
   }

   std::string String::ToString(sint16 val)
   {
      char string[32];
      Snprintf(string, Util::ArraySize(string), "%hd", val);

      return string;
   }

   std::string String::ToString(uint32 val)
   {
      char string[32];
      Snprintf(string, Util::ArraySize(string), "%u", val);

      return string;
   }

   std::string String::ToString(sint32 val)
   {
      char string[32];
      Snprintf(string, Util::ArraySize(string), "%d", val);

      return string;
   }

   std::string String::ToString(uint64 val)
   {
      char string[32];
      Snprintf(string, Util::ArraySize(string), "%" TC_UINT64_FORMAT, val);

      return string;
   }

   std::string String::ToString(sint64 val)
   {
      char string[32];
      Snprintf(string, Util::ArraySize(string), "%" TC_SINT64_FORMAT, val);

      return string;
   }

   std::string String::ToString(float val)
   {
      char string[32];
      Snprintf(string, Util::ArraySize(string), "%f", val);

      return string;
   }

   std::string String::ToString(double val)
   {
      char string[32];
      Snprintf(string, Util::ArraySize(string), "%lf", val);

      return string;
   }

   std::string String::ToString(const char* val)
   {
      return val ? val : "";
   }

   bool String::IsIntValue(const std::string& val)
   {
      sint32 tmp;
      return std::sscanf(val.c_str(), "%d", &tmp) == 1;
   }

   bool String::IsFltValue(const std::string& val)
   {
      double tmp;
      return std::sscanf(val.c_str(), "%lf", &tmp) == 1;
   }

   bool String::IsValue(const std::string& val)
   {
      return (IsIntValue(val) || IsFltValue(val));
   }

   double String::ToDouble(const std::string& val)
   {
      double v;
      if (std::sscanf(val.c_str(), "%lf", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   float String::ToFloat(const std::string& val)
   {
      float v;
      if (std::sscanf(val.c_str(), "%f", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   sint8 String::ToSint8(const std::string& val)
   {
      sint8 v;
      if (std::sscanf(val.c_str(), "%c", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   uint8 String::ToUint8(const std::string& val)
   {
      uint8 v;
      if (std::sscanf(val.c_str(), "%c", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   sint16 String::ToSint16(const std::string& val)
   {
      sint16 v;
      if (std::sscanf(val.c_str(), "%hd", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   uint16 String::ToUint16(const std::string& val)
   {
      uint16 v;
      if (std::sscanf(val.c_str(), "%hu", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   sint32 String::ToSint32(const std::string& val)
   {
      sint32 v;
      if (std::sscanf(val.c_str(), "%d", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   uint32 String::ToUint32(const std::string& val)
   {
      uint32 v;
      if (std::sscanf(val.c_str(), "%u", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   sint64 String::ToSint64(const std::string& val)
   {
      sint64 v;
      if (std::sscanf(val.c_str(), "%" TC_SINT64_FORMAT, &v) != 1)
      {
         return 0;
      }

      return v;
   }

   uint64 String::ToUint64(const std::string& val)
   {
      uint64 v;
      if (std::sscanf(val.c_str(), "%" TC_UINT64_FORMAT, &v) != 1)
      {
         return 0;
      }

      return v;
   }

   bool String::ToBool(const std::string& val)
   {
      char v;
      if (std::sscanf(val.c_str(), "%c", &v) != 1)
      {
         return false;
      }

      return v != 0;
   }

   std::string String::ToUpper(const std::string& text_in)
   {
      return StringImp::ToUpper(text_in);
   }

   std::string String::ToLower(const std::string& text_in)
   {
      return StringImp::ToLower(text_in);
   }

   bool String::Split(const std::string& text, const std::string& split,
      std::string& part1, std::string& part2)
   {
      return StringImp::Split(text, split, part1, part2);
   }

   bool String::Split(const std::string& text, uint32 pos,
      std::string& part1, std::string& part2)
   {
      return StringImp::Split(text, pos, part1, part2);
   }

   bool String::Split(const std::string& text, const std::string& limiter,
      std::vector< std::string >& data)
   {
      return StringImp::Split(text, limiter, data);
   }

   void String::Join(const std::vector<std::string>& texts, const std::string& limiter, std::string& text)
   {
      StringImp::Join(texts, limiter, text);
   }

   std::string String::TrimmSpaces(const std::string& text)
   {
      return StringImp::TrimmSpaces(text);
   }

   std::string String::TrimmLeadingSpaces(const std::string& text)
   {
      return StringImp::TrimmLeadingSpaces(text);
   }

   std::string String::TrimmTrailingSpaces(const std::string& text)
   {
      return StringImp::TrimmTrailingSpaces(text);
   }

   std::string String::DeleteBlanksAndTabs(const std::string& text_in)
   {
      std::string text = Replace(text_in, " ", "");
      return Replace(text, "\t", "");
   }

   std::string String::Replace(const std::string& text_in, char r1, char r2)
   {
      return StringImp::Replace(text_in, r1, r2);
   }

   std::string String::Replace(const std::string& text_in,
      const std::string& r1, const std::string& r2)
   {
      return StringImp::Replace(text_in, r1, r2);
   }

   sint32 String::Snprintf(char* buf, uint32 size_of_buf, const char* fmt, ...)
   {
      va_list arguments;
      va_start(arguments, fmt);
      sint32 len = StringTraits::VsnPrintf(buf, size_of_buf, fmt, arguments);
      va_end(arguments);

      return len;
   }

   sint32 String::VSnprintf(char* buf, uint32 size_of_buf, const char *fmt, va_list arguments)
   {
      return StringTraits::VsnPrintf(buf, size_of_buf, fmt, arguments);
   }

   sint32 StringICompare(const char* s1, const char* s2)
   {
       return StringImp::StringICompare(s1, s2);
   }
   
   sint32 StringICompare(const char* s1, const char* s2, uint32 len_to_compare)
   {
       return StringImp::StringICompare(s1, s2, len_to_compare);
   }

   std::string String::Print(const char *fmt, ...)
   {
      va_list arguments;
      va_start(arguments, fmt);
      std::string buf = StringImp::VPrint(fmt , arguments);
      va_end(arguments);

      return buf;
   }

   std::string String::VPrint(const char *fmt, va_list arguments)
   {
      return StringImp::VPrint(fmt , arguments);
   }

}
