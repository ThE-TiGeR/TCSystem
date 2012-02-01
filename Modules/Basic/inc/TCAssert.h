//************************************************************
//
// *******   ***   ***               *
//    *     *     *                  *
//    *    *      *                *****
//    *    *       ***  *   *   **   *    **    ***
//    *    *          *  * *   *     *   ****  * * *
//    *     *         *   *      *   * * *     * * *
//    *      ***   ***    *     **   **   **   *   *
//                        *
// see http://sourceforge.net/projects/tcsystem/ for details.
// Copyright (C) 2003 - 2007 Thomas Goessler (the____tiger@hotmail.com)
//************************************************************
//
// TCSystem is the legal property of its developers.
// Please refer to the COPYRIGHT file distributed with this source distribution.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------
//  $Id$
//----------------------------------------------------------------------

#ifndef _TC_ASSERT_H_
#define _TC_ASSERT_H_

#include "TCTypes.h"

/**
 * @addtogroup TC_BASE_DEBUG
 * @{
 */

/**
 * @file
 * @brief This file provides the definition of TC_ASSERT, TC_VERIFY and TC_CT_ASSERT
 *
 * @author Thomas Goessler
 */

#ifndef NDEBUG
#  include <cassert>
   /**
    * @brief Macro for asserting errors.
    * The expression of the macro is not executed in release builds
    */
#  define TC_ASSERT(EX) assert(EX)
   /**
    * @brief Macro for asserting errors.
    * The expression of the macro is executed in release builds but not checked
    */
#  define TC_VERIFY(EX) assert(EX)
#else
#  define TC_ASSERT(EX)  ((void)0)
#  define TC_VERIFY(EX) EX
#endif

namespace tc
{
   namespace imp
   {
      /** @brief Internal class for implementing compile time assert */
      template <bool val> struct CompileTimeAssertFailure;
      /** @brief Internal class for implementing compile time assert */
      template <>
      struct CompileTimeAssertFailure<true> {};
      /** @brief Internal class for implementing compile time assert */
      template<int x> struct CompileTimeAssertTest {};
   }
}

#define TC_IMPL_JOIN( X, Y )     TC_IMPL_DO_JOIN( X, Y )
#define TC_IMPL_DO_JOIN( X, Y )  TC_IMPL_DO_JOIN2(X,Y)
#define TC_IMPL_DO_JOIN2( X, Y ) X##Y

/**
 * @brief Macro for asserting errors  at compile time.
 */
#define TC_CT_ASSERT(VAL)                                                                    \
   typedef tc::imp::CompileTimeAssertTest<                                                  \
               sizeof(tc::imp::CompileTimeAssertFailure<(bool)(VAL)>)                       \
                                          > TC_IMPL_JOIN(__tc_static_assert_type_, __LINE__)

/**
* @}
*/

#endif // _TC_ASSERT_H_
