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
#ifndef _TC_WSTRING_H_
#define _TC_WSTRING_H_

#include "TCTypes.h"

#include <string>
#include <vector>
#include <cstdarg>

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file 
    * This header file provides the definition of the namespace tc::String. 
    * 
    * @author Thomas Goessler 
    */

    /**
    * @brief This namespace provided functions for manipulating and converting strings
    */
   namespace wstring
   {
      TCBASE_API std::wstring ToString(const char* val);
      TCBASE_API std::string  ToString(const wchar_t* val);
      TCBASE_API std::wstring ToString(const std::string& val);
      TCBASE_API std::string  ToString(const std::wstring& val);
      TCBASE_API std::wstring ToString(wchar_t val);
      TCBASE_API std::wstring ToString(bool val);
      TCBASE_API std::wstring ToString(uint64_t val);
      TCBASE_API std::wstring ToString(uint32_t val);
      TCBASE_API std::wstring ToString(uint16_t val);
      TCBASE_API std::wstring ToString(uint8_t val);
      TCBASE_API std::wstring ToString(int64_t val);
      TCBASE_API std::wstring ToString(int32_t val);
      TCBASE_API std::wstring ToString(int16_t val);
      TCBASE_API std::wstring ToString(int8_t val);
      TCBASE_API std::wstring ToString(float val);
      TCBASE_API std::wstring ToString(double val);

      TCBASE_API bool IsIntValue(const std::wstring& text);
      TCBASE_API bool IsFltValue(const std::wstring& text);
      TCBASE_API bool IsValue(const std::wstring& text);

      TCBASE_API bool  ToBool(const std::wstring& text);

      TCBASE_API uint64_t ToUint64(const std::wstring& text);
      TCBASE_API uint32_t ToUint32(const std::wstring& text);
      TCBASE_API uint16_t ToUint16(const std::wstring& text);
      TCBASE_API uint8_t  ToUint8(const std::wstring& text);

      TCBASE_API int64_t ToSint64(const std::wstring& text);
      TCBASE_API int32_t ToSint32(const std::wstring& text);
      TCBASE_API int16_t ToSint16(const std::wstring& text);
      TCBASE_API int8_t  ToSint8(const std::wstring& text);

      TCBASE_API double ToFlt64(const std::wstring& text);
      TCBASE_API float ToFlt32(const std::wstring& text);

      TCBASE_API bool Split(const std::wstring& text, const std::wstring& split,
                 std::wstring& text1, std::wstring& text2);
      TCBASE_API bool Split(const std::wstring& text, const std::wstring& split,
                 std::vector<std::wstring>& texts);
      TCBASE_API void Join(const std::vector<std::wstring>& texts, const std::wstring& split,
                std::wstring& text);

      TCBASE_API std::wstring Replace(const std::wstring& text, wchar_t c1, wchar_t c2);
      TCBASE_API std::wstring Replace(const std::wstring& text, const std::wstring& text1, const std::wstring& text2);
      TCBASE_API std::wstring ToLower(const std::wstring& text);
      TCBASE_API std::wstring ToUpper(const std::wstring& text);

      TCBASE_API std::wstring DeleteBlanksAndTabs(const std::wstring& text);
      TCBASE_API std::wstring TrimmSpaces(const std::wstring& text);
      /** @short Removes spaces at the beginning of the string */
      TCBASE_API std::wstring TrimmLeadingSpaces(const std::wstring& text);
      /** @short Removes spaces at the end of the string */
      TCBASE_API std::wstring TrimmTrailingSpaces(const std::wstring& text);

      /**
       * @short Compare two strings case-insensitively
       * @param s1 One string
       * @param s2 Another string
       * @return 0 if equal
       */
      TCBASE_API int32_t StringICompare(const wchar_t* s1, const wchar_t* s2);
      /**
       * @short Compare two strings case-insensitively
       * @param s1 One string
       * @param s2 Another string
       * @param len_to_compare len of the two strings which should be compared
       * @return 0 if equal
       */
      TCBASE_API int32_t StringICompare(const wchar_t* s1, const wchar_t* s2, uint32_t len_to_compare);

      TCBASE_API int32_t Snprintf(wchar_t* buffer, uint32_t size_of_buf, const wchar_t*, ...);
      TCBASE_API int32_t VSnprintf(wchar_t* buffer, uint32_t size_of_buf, const wchar_t*, va_list arguments);
      TCBASE_API std::wstring Print(const wchar_t* format, ...);
      TCBASE_API std::wstring VPrint(const wchar_t* format, va_list arguments);

   } // namespace wstring

   /**
    * @}
    */

} // namspace tc

#endif  // _TC_WSTRING_H_
