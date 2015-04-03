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
//  $Id: TCMathTestSuite.h,v a33a555a5a40 2012/02/01 22:48:18 Thomas $
//----------------------------------------------------------------------

#ifndef _TC_MATH_TEST_SUITE_H_
#define _TC_MATH_TEST_SUITE_H_

#include "TCMathApi.h"
#include "TCUnitTestSuite.h"

namespace tc
{
   namespace math
   {
      namespace tests
      {
         class TCMATH_API Suite : public unit::TestSuite
         {
         public:
            Suite();
         };
      }
   }
}

#endif // _TC_MATH_TEST_SUITE_H_
