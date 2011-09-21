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
#include "TCMathTestSuite.h"

#include "TCUnitTestCase.h"
#include "TCMathCoordNTestSuite.h"
#include "TCMathMatrixNTestSuite.h"
#include "TCMathUtil.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Math
   {
      namespace Tests
      {
         class PowTest : public Unit::TestCase
         {
         public:
            PowTest() : Unit::TestCase("TC::Math::Tests::PowTest") {}
            virtual void Execute();
         };

         void PowTest::Execute()
         {
            TCUNIT_ASSERT(TC::Math::Pow(2, 2) == 4);
            TCUNIT_ASSERT(TC::Math::Pow(2, 3) == 8);

            TCUNIT_ASSERT(TC::Math::Pow(7, 6) == 117649);
         }

         class AlignTest : public Unit::TestCase
         {
         public:
            AlignTest() : Unit::TestCase("TC::Math::Tests::AlignTest") {}
            virtual void Execute();
         };

         void AlignTest::Execute()
         {
            TCUNIT_ASSERT(TC::Math::Align32(0) == 0);
            TCUNIT_ASSERT(TC::Math::Align32(4) == 32);
            TCUNIT_ASSERT(TC::Math::Align32(31) == 32);
            TCUNIT_ASSERT(TC::Math::Align32(32) == 32);
            TCUNIT_ASSERT(TC::Math::Align32(33) == 64);

            TCUNIT_ASSERT(TC::Math::Align16(0) == 0);
            TCUNIT_ASSERT(TC::Math::Align16(4) == 16);
            TCUNIT_ASSERT(TC::Math::Align16(15) == 16);
            TCUNIT_ASSERT(TC::Math::Align16(32) == 32);
            TCUNIT_ASSERT(TC::Math::Align16(33) == 48);
         }

         class CompareTests : public Unit::TestCase
         {
         public:
            CompareTests() : Unit::TestCase("TC::Math::Tests::CompareTests") {}
            virtual void Execute();
         };

         void CompareTests::Execute()
         {
            TCUNIT_ASSERT(TC::Math::Compare(2.0001, 2.0001));
            TCUNIT_ASSERT(!TC::Math::Compare(-2.0001, 2.0001));
            TCUNIT_ASSERT(!TC::Math::Compare(2.0001, 2.0001001));
         }


         Suite::Suite()
            :Unit::TestSuite("TC::Math::Tests::Suite")
         {
            AddTest(Unit::Test::Ptr(new CoordNSuite));
            AddTest(Unit::Test::Ptr(new MatrixNSuite));
            AddTest(Unit::Test::Ptr(new PowTest));
         }
      }
   }
}

#endif
