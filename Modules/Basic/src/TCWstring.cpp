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
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCWstring.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#include <cstdlib>
#include "TCWString.h"

#include "TCStringImp.h"
#include "TCUtil.h"
#include "TCWStringTraits.h"

#include "TCNewEnable.h"

namespace tc
{
   typedef StringImpTmpl<std::wstring, WStringTraits> WStringImp;

   std::wstring wstring::ToString(wchar_t val)
   {
      const wchar_t string[2] = {val, 0};
      return string;
   }

   std::wstring wstring::ToString(bool val)
   {
      return val ? L"1" : L"0";
   }

   std::wstring wstring::ToString(uint16_t val)
   {
      wchar_t string[32];
      Snprintf(string, util::ArraySize(string), L"%u", val);

      return string;
   }

   std::wstring wstring::ToString(int16_t val)
   {
      wchar_t string[32];
      Snprintf(string, util::ArraySize(string), L"%d", val);

      return string;
   }

   std::wstring wstring::ToString(uint32_t val)
   {
      wchar_t string[32];
      Snprintf(string, util::ArraySize(string), L"%u", val);

      return string;
   }

   std::wstring wstring::ToString(int32_t val)
   {
      wchar_t string[32];
      Snprintf(string, util::ArraySize(string), L"%d", val);

      return string;
   }

   std::wstring wstring::ToString(uint64_t val)
   {
      wchar_t string[32];
      Snprintf(string, util::ArraySize(string), L"%" TC_UINT64_WFORMAT, val);

      return string;
   }

   std::wstring wstring::ToString(int64_t val)
   {
      wchar_t string[32];
      Snprintf(string, util::ArraySize(string), L"%" TC_SINT64_WFORMAT, val);

      return string;
   }

   std::wstring wstring::ToString(float val)
   {
      wchar_t string[32];
      Snprintf(string, util::ArraySize(string), L"%f", val);

      return string;
   }

   std::wstring wstring::ToString(double val)
   {
      wchar_t string[32];
      Snprintf(string, util::ArraySize(string), L"%lf", val);

      return string;
   }

   bool wstring::IsIntValue(const std::wstring& val)
   {
      int32_t tmp;
      return std::swscanf(val.c_str(), L"%d", &tmp) == 1;
   }

   bool wstring::IsFltValue(const std::wstring& val)
   {
      double tmp;
      return std::swscanf(val.c_str(), L"%lf", &tmp) == 1;
   }

   bool wstring::IsValue(const std::wstring& val)
   {
      return (IsIntValue(val) || IsFltValue(val));
   }

   double wstring::ToFlt64(const std::wstring& val)
   {
      double v;
      if (std::swscanf(val.c_str(), L"%lf", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   float wstring::ToFlt32(const std::wstring& val)
   {
      float v;
      if (std::swscanf(val.c_str(), L"%f", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   int8_t wstring::ToSint8(const std::wstring& val)
   {
      int8_t v;
      if (std::swscanf(val.c_str(), L"%hc", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   uint8_t wstring::ToUint8(const std::wstring& val)
   {
      uint8_t v;
      if (std::swscanf(val.c_str(), L"%hc", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   int16_t wstring::ToSint16(const std::wstring& val)
   {
      int16_t v;
      if (std::swscanf(val.c_str(), L"%hd", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   uint16_t wstring::ToUint16(const std::wstring& val)
   {
      uint16_t v;
      if (std::swscanf(val.c_str(), L"%hu", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   int32_t wstring::ToSint32(const std::wstring& val)
   {
      int32_t v;
      if (std::swscanf(val.c_str(), L"%d", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   uint32_t wstring::ToUint32(const std::wstring& val)
   {
      uint32_t v;
      if (std::swscanf(val.c_str(), L"%u", &v) != 1)
      {
         return 0;
      }

      return v;
   }

   int64_t wstring::ToSint64(const std::wstring& val)
   {
      int64_t v;
      if (std::swscanf(val.c_str(), L"%" TC_SINT64_WFORMAT, &v) != 1)
      {
         return 0;
      }

      return v;
   }

   uint64_t wstring::ToUint64(const std::wstring& val)
   {
      uint64_t v;
      if (std::swscanf(val.c_str(), L"%" TC_UINT64_WFORMAT, &v) != 1)
      {
         return 0;
      }

      return v;
   }

   bool wstring::ToBool(const std::wstring& val)
   {
      wchar_t v;
      if (std::swscanf(val.c_str(), L"%c", &v) != 1)
      {
         return false;
      }

      return v != 0;
   }

   std::wstring wstring::ToUpper(const std::wstring& text_in)
   {
      return WStringImp::ToUpper(text_in);
   }

   std::wstring wstring::ToLower(const std::wstring& text_in)
   {
      return WStringImp::ToLower(text_in);
   }

   bool wstring::Split(const std::wstring& text, const std::wstring& split,
      std::wstring& part1, std::wstring& part2)
   {
      return WStringImp::Split(text, split, part1, part2);
   }

   std::vector<std::wstring> wstring::Split(const std::wstring& text, const std::wstring& limiter)
   {
      return WStringImp::Split(text, limiter);
   }

   std::wstring wstring::Join(const std::vector<std::wstring>& texts, const std::wstring& limiter)
   {
      return WStringImp::Join(texts, limiter);
   }

   std::wstring wstring::TrimmSpaces(const std::wstring& text)
   {
      return WStringImp::TrimmSpaces(text);
   }

   std::wstring wstring::TrimmLeadingSpaces(const std::wstring& text)
   {
      return WStringImp::TrimmLeadingSpaces(text);
   }

   std::wstring wstring::TrimmTrailingSpaces(const std::wstring& text)
   {
      return WStringImp::TrimmTrailingSpaces(text);
   }

   std::wstring wstring::DeleteBlanksAndTabs(const std::wstring& text_in)
   {
      const std::wstring text = WStringImp::Replace(text_in, L" ", L"");
      return WStringImp::Replace(text, L"\t", L"");
   }

   std::wstring wstring::Replace(const std::wstring& text_in, wchar_t r1, wchar_t r2)
   {
      return WStringImp::Replace(text_in, r1, r2);
   }

   std::wstring wstring::Replace(const std::wstring& text_in,
      const std::wstring& r1, const std::wstring& r2)
   {
      return WStringImp::Replace(text_in, r1, r2);
   }

   int32_t wstring::VSnprintf(wchar_t* buf, uint32_t size_of_buf, const wchar_t* fmt, va_list arguments)
   {
      return WStringTraits::VsnPrintf(buf, size_of_buf, fmt, arguments);
   }

   int32_t wstring::Snprintf(wchar_t* buf, uint32_t size_of_buf, const wchar_t* fmt, ...)
   {
      va_list arguments;
      va_start(arguments, fmt);
      const int32_t len = WStringTraits::VsnPrintf(buf, size_of_buf, fmt, arguments);
      va_end(arguments);

      return len;
   }

   int32_t StringICompare(const wchar_t* s1, const wchar_t* s2)
   {
       return WStringImp::StringICompare(s1, s2);
   }
   
   int32_t StringICompare(const wchar_t* s1, const wchar_t* s2, uint32_t len_to_compare)
   {
       return WStringImp::StringICompare(s1, s2, len_to_compare);
   }

   std::wstring wstring::Print(const wchar_t *fmt, ...)
   {
      va_list arguments;
      va_start(arguments, fmt);
      std::wstring buf = WStringImp::VPrint(fmt , arguments);
      va_end(arguments);

      return buf;
   }

   std::wstring wstring::VPrint(const wchar_t *fmt, va_list arguments)
   {
      return WStringImp::VPrint(fmt , arguments);
   }

   std::wstring wstring::ToString(const char* utf8_string)
   {
      if (utf8_string)
      {
      return ToString(std::string(utf8_string));
      }

      return L"";
   }

   std::wstring wstring::ToString(const std::string& utf8_string)
   {
      if (utf8_string.length() > 0)
      {
#ifdef TCOS_POSIX
         int32_t wc_buffer_size = ::mbstowcs(0, utf8_string.c_str(), 0);
#else
         // convert to to wchar string from uft8 multibyte string
         const int32_t wc_buffer_size = ::MultiByteToWideChar(CP_UTF8,
                                                              0,
                                                              utf8_string.c_str(),
                                                              -1,
                                                              0,
                                                              0);
#endif

         if (wc_buffer_size > 0)
         {
            wchar_t* p_wc_buff = new wchar_t[wc_buffer_size + 1];

#ifdef TCOS_POSIX
            ::mbstowcs(p_wc_buff, utf8_string.c_str(), wc_buffer_size+1);
#else
            ::MultiByteToWideChar(CP_UTF8,
               0,
               utf8_string.c_str(),
               -1,
               p_wc_buff,
               wc_buffer_size);
#endif
            p_wc_buff[wc_buffer_size] = 0;
            std::wstring wc_string(p_wc_buff);
            delete[] p_wc_buff;

            return wc_string;
         }
      }
      return L"";
   }

   std::string wstring::ToString(const wchar_t* string)
   {
      if (string)
      {
         return ToString(std::wstring(string));
      }

      return "";
   }

   std::string wstring::ToString(const std::wstring& string)
   {
      if (string.length() > 0)
      {
#ifdef TCOS_POSIX
         int32_t c_buffer_size = ::wcstombs(0, string.c_str(), 0);
#else
         // convert from wchar string to uft8 multibyte string
         const int32_t c_buffer_size = ::WideCharToMultiByte(CP_UTF8, 0, string.c_str(), -1, 0, 0, 0, 0);
#endif

         if (c_buffer_size > 0)
         {
            char* p_c_buff = new char[c_buffer_size + 1];

#ifdef TCOS_POSIX
            ::wcstombs(p_c_buff, string.c_str(), c_buffer_size+1);
#else
            ::WideCharToMultiByte(CP_UTF8, 0, string.c_str(), -1, p_c_buff, c_buffer_size, 0, 0);
#endif

            p_c_buff[c_buffer_size] = 0;
            std::string c_string(p_c_buff);
            delete[] p_c_buff;

            return c_string;
         }
      }
      return "";
   }

}
