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
#ifdef HAVE_UNIT_TESTS
#include "TCBasicTestSuite.h"

#include "TCStringTest.h"
#include "TCWStringTest.h"
#include "TCFileNameTest.h"
#include "TCUtilTest.h"
#include "TCTimeTest.h"
#include "TCWFileNameTest.h"
#include "TCHashTableTest.h"
#include "TCStreamTest.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Tests
   {
      BasicSuite::BasicSuite()
         :Unit::TestSuite("TC::BasicSuite")
      {
         add_test(new UtilTestSuite);
         add_test(new StringTestSuite);
         add_test(new WStringTestSuite);
         add_test(new FileNameTestSuite);
         add_test(new WFileNameTestSuite);
         add_test(new TimeTestSuite);
         add_test(new HashTableTestSuite);
         add_test(new StreamTestSuite);
      }
   }
}

#endif
