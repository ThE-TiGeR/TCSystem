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
#ifndef _TCSTRING_H_
#define _TCSTRING_H_

#include "TCTypes.h"

#include <string>
#include <vector>
#include <cstdarg>

namespace TC
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file 
    * This header file provides the definition of the namespace TC::String. 
    * 
    * @author Thomas Goessler 
    */

    /**
    * @brief This namespace provided functions for manipulating and converting strings
    */
   namespace String
   {
      TCBASE_API std::string ToString(const char* val);
      TCBASE_API std::string ToString(char val);
      TCBASE_API std::string ToString(bool val);
      TCBASE_API std::string ToString(uint64 val);
      TCBASE_API std::string ToString(uint32 val);
      TCBASE_API std::string ToString(uint16 val);
      TCBASE_API std::string ToString(uint8 val);
      TCBASE_API std::string ToString(sint64 val);
      TCBASE_API std::string ToString(sint32 val);
      TCBASE_API std::string ToString(sint16 val);
      TCBASE_API std::string ToString(sint8 val);
      TCBASE_API std::string ToString(float val);
      TCBASE_API std::string ToString(double val);

      TCBASE_API bool IsIntValue(const std::string& text);
      TCBASE_API bool IsFltValue(const std::string& text);
      TCBASE_API bool IsValue(const std::string& text);

      TCBASE_API bool ToBool(const std::string& text);

      TCBASE_API uint64 ToUint64(const std::string& text);
      TCBASE_API uint32 ToUint32(const std::string& text);
      TCBASE_API uint16 ToUint16(const std::string& text);
      TCBASE_API uint8  ToUint8(const std::string& text);

      TCBASE_API sint64 ToSint64(const std::string& text);
      TCBASE_API sint32 ToSint32(const std::string& text);
      TCBASE_API sint16 ToSint16(const std::string& text);
      TCBASE_API sint8  ToSint8(const std::string& text);

      TCBASE_API double ToDouble(const std::string& text);
      TCBASE_API float ToFloat(const std::string& text);

      TCBASE_API bool Split(const std::string& text, const std::string& split,
                 std::string& text1, std::string& text2);
      TCBASE_API bool Split(const std::string& text, uint32 pos,
                 std::string& text1, std::string& text2);
      TCBASE_API bool Split(const std::string& text, const std::string& split,
                 std::vector<std::string>& texts);
      TCBASE_API void Join(const std::vector<std::string>& texts, const std::string& split,
                std::string& text);

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
      TCBASE_API sint32 StringICompare(const char* s1, const char* s2);
      /**
       * @short Compare two strings case-insensitively
       * @param s1 One string
       * @param s2 Another string
       * @param len_to_compare len of the two strings which should be compared
       * @return 0 if equal
       */
      TCBASE_API sint32 StringICompare(const char* s1, const char* s2, uint32 len_to_compare);

      TCBASE_API sint32 Snprintf(char* buf, uint32 size_of_buf, const char *fmt, ...);
      TCBASE_API sint32 VSnprintf(char* buf, uint32 size_of_buf, const char *fmt, va_list arguments);
      TCBASE_API std::string Print(const char* format, ...);
      TCBASE_API std::string VPrint(const char* format, va_list arguments);

   } // namespace String

   /**
    * @}
    */

} // namspace TC

#endif  // _TCSTRING_H_
