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
//  $Id: TCString.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifndef _TCSTRING_H_
#define _TCSTRING_H_

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
    * This header file provides the definition of the namespace tc::string. 
    * 
    * @author Thomas Goessler 
    */

    /**
    * @brief This namespace provided functions for manipulating and converting strings
    */
   namespace string
   {
      TCBASE_API std::string ToString(const char* val);
      TCBASE_API std::string ToString(char val);
      TCBASE_API std::string ToString(bool val);
      TCBASE_API std::string ToString(uint64_t val);
      TCBASE_API std::string ToString(uint32_t val);
      TCBASE_API std::string ToString(uint16_t val);
      TCBASE_API std::string ToString(uint8_t val);
      TCBASE_API std::string ToString(int64_t val);
      TCBASE_API std::string ToString(int32_t val);
      TCBASE_API std::string ToString(int16_t val);
      TCBASE_API std::string ToString(int8_t val);
      TCBASE_API std::string ToString(float val);
      TCBASE_API std::string ToString(double val);

      TCBASE_API bool IsIntValue(const std::string& text);
      TCBASE_API bool IsFltValue(const std::string& text);
      TCBASE_API bool IsValue(const std::string& text);

      TCBASE_API bool ToBool(const std::string& text);

      TCBASE_API uint64_t ToUint64(const std::string& text);
      TCBASE_API uint32_t ToUint32(const std::string& text);
      TCBASE_API uint16_t ToUint16(const std::string& text);
      TCBASE_API uint8_t  ToUint8(const std::string& text);

      TCBASE_API int64_t ToSint64(const std::string& text);
      TCBASE_API int32_t ToSint32(const std::string& text);
      TCBASE_API int16_t ToSint16(const std::string& text);
      TCBASE_API int8_t  ToSint8(const std::string& text);

      TCBASE_API uint32_t HexToUint32(const std::string& text);

      TCBASE_API double ToDouble(const std::string& text);
      TCBASE_API float ToFloat(const std::string& text);

      TCBASE_API bool Split(const std::string& text, const std::string& split,
                 std::string& text1, std::string& text2);
      TCBASE_API bool Split(const std::string& text, std::string::size_type pos,
                 std::string& text1, std::string& text2);
      TCBASE_API std::vector<std::string> Split(const std::string& text, const std::string& split);
      TCBASE_API std::string Join(const std::vector<std::string>& texts, const std::string& split);

      TCBASE_API std::string Replace(const std::string& text, char c1, char c2);
      TCBASE_API std::string Replace(const std::string& text, const std::string& text1, const std::string& text2);
      TCBASE_API std::string ToLower(const std::string& text);
      TCBASE_API std::string ToUpper(const std::string& text);

      TCBASE_API std::string DeleteBlanksAndTabs(const std::string& text);
      TCBASE_API std::string TrimmSpaces(const std::string& text);
      /** @short Removes spaces at the beginning of the string */
      TCBASE_API std::string TrimmLeadingSpaces(const std::string& text);
      /** @short Removes spaces at the end of the string */
      TCBASE_API std::string TrimmTrailingSpaces(const std::string& text);

      /**
       * @short Compare two strings case-insensitively
       * @param s1 One string
       * @param s2 Another string
       * @return 0 if equal
       */
      TCBASE_API int32_t StringICompare(const char* s1, const char* s2);
      /**
       * @short Compare two strings case-insensitively
       * @param s1 One string
       * @param s2 Another string
       * @param len_to_compare len of the two strings which should be compared
       * @return 0 if equal
       */
      TCBASE_API int32_t StringICompare(const char* s1, const char* s2, uint32_t len_to_compare);

      TCBASE_API int32_t Snprintf(char* buf, uint32_t size_of_buf, const char *fmt, ...);
      TCBASE_API int32_t VSnprintf(char* buf, uint32_t size_of_buf, const char *fmt, va_list arguments);
      TCBASE_API std::string Print(const char* format, ...);
      TCBASE_API std::string VPrint(const char* format, va_list arguments);

   } // namespace string

   /**
    * @}
    */

} // namspace tc

#endif  // _TCSTRING_H_
