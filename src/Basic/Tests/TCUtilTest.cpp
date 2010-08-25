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
//  $Id: TCUtilTest.cpp 986 2010-05-14 06:44:38Z the_____tiger $
//----------------------------------------------------------------------
#ifdef HAVE_UNIT_TESTS

#include "TCUtilTest.h"

#include "TCUtil.h"
#include "TCMathUtil.h"

#include <jf/unittest/test_case.h>

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class UtilAbs : public jf::unittest::TestCase
      {
      public:
         UtilAbs() : jf::unittest::TestCase("TC::Tests::UtilAbs") {}
         virtual void run();
      };

      void UtilAbs::run()
      {
         JFUNIT_ASSERT(TC::Util::Abs(-1) == 1);
         JFUNIT_ASSERT(TC::Util::Abs( 1) == 1);
         JFUNIT_ASSERT(TC::Util::Abs(-1234567) == 1234567);
         JFUNIT_ASSERT(TC::Util::Abs( 1234567) == 1234567);

         JFUNIT_ASSERT(TC::Util::Abs( 0.0) == 0.0);
         JFUNIT_ASSERT(TC::Util::Abs(-0.0) == 0.0);
         JFUNIT_ASSERT(TC::Util::Abs( 0.1) == 0.1);
         JFUNIT_ASSERT(TC::Util::Abs(-0.1) == 0.1);
         JFUNIT_ASSERT(TC::Util::Abs( 1.1) == 1.1);
         JFUNIT_ASSERT(TC::Util::Abs(-1.1) == 1.1);
      }

      class UtilMinMax : public jf::unittest::TestCase
      {
      public:
         UtilMinMax() : jf::unittest::TestCase("TC::Tests::UtilMinMax") {}
         virtual void run();
      };

      void UtilMinMax::run()
      {
         JFUNIT_ASSERT(TC::Util::Min(1, 2) == 1);

         JFUNIT_ASSERT(TC::Util::Min(-2, -3) == -3);

         JFUNIT_ASSERT(TC::Util::Min(1, 2, 3) == 1);

         JFUNIT_ASSERT(TC::Util::Min(1, -2, -3) == -3);

         JFUNIT_ASSERT(TC::Util::Max(1, 2) == 2);

         JFUNIT_ASSERT(TC::Util::Max(-2, -3) == -2);

         JFUNIT_ASSERT(TC::Util::Max(1, 2, 3) == 3);

         JFUNIT_ASSERT(TC::Util::Max(1, -2, -3) == 1);
      }


      class UtilPow : public jf::unittest::TestCase
      {
      public:
         UtilPow() : jf::unittest::TestCase("TC::Tests::UtilPow") {}
         virtual void run();
      };

      void UtilPow::run()
      {
         JFUNIT_ASSERT(TC::Math::Pow(2, 2) == 4);
         JFUNIT_ASSERT(TC::Math::Pow(2, 3) == 8);

         JFUNIT_ASSERT(TC::Math::Pow(7, 6) == 117649);
      }


      class UtilAlign : public jf::unittest::TestCase
      {
      public:
         UtilAlign() : jf::unittest::TestCase("TC::Tests::UtilAlign") {}
         virtual void run();
      };

      void UtilAlign::run()
      {
         JFUNIT_ASSERT(TC::Math::Align32(0) == 0);
         JFUNIT_ASSERT(TC::Math::Align32(4) == 32);
         JFUNIT_ASSERT(TC::Math::Align32(31) == 32);
         JFUNIT_ASSERT(TC::Math::Align32(32) == 32);
         JFUNIT_ASSERT(TC::Math::Align32(33) == 64);

         JFUNIT_ASSERT(TC::Math::Align16(0) == 0);
         JFUNIT_ASSERT(TC::Math::Align16(4) == 16);
         JFUNIT_ASSERT(TC::Math::Align16(15) == 16);
         JFUNIT_ASSERT(TC::Math::Align16(32) == 32);
         JFUNIT_ASSERT(TC::Math::Align16(33) == 48);
      }

      class UtilCompare : public jf::unittest::TestCase
      {
      public:
         UtilCompare() : jf::unittest::TestCase("TC::Tests::UtilCompare") {}
         virtual void run();
      };

      void UtilCompare::run()
      {
         JFUNIT_ASSERT(TC::Math::Compare(2.0001, 2.0001));
         JFUNIT_ASSERT(!TC::Math::Compare(-2.0001, 2.0001));
         JFUNIT_ASSERT(!TC::Math::Compare(2.0001, 2.0001001));
      }

      class UtilStlContainer : public jf::unittest::TestCase
      {
      public:
         UtilStlContainer() : jf::unittest::TestCase("TC::Tests::UtilStlContainer") {}
         virtual void run();
      };

      void UtilStlContainer::run()
      {
         std::vector<int> vec(10);

         JFUNIT_ASSERT(vec.size() == 10);
         JFUNIT_ASSERT(vec.capacity() == 10);

         TC::Util::FreeMemoryOfStlContainer(vec);
         JFUNIT_ASSERT(vec.size() == 0);
         JFUNIT_ASSERT(vec.capacity() == 0);

         std::vector<int> vec1(10);
         vec1.push_back(1);

         JFUNIT_ASSERT(vec1.size() == 11);
         JFUNIT_ASSERT(vec1.capacity() > 11);

         TC::Util::MinimizeMemoryOfStlContainer(vec1);
         JFUNIT_ASSERT(vec1.size() == 11);
         JFUNIT_ASSERT(vec1.capacity() == 11);
      }

      class UtilSafeRelease : public jf::unittest::TestCase
      {
      public:
         UtilSafeRelease() : jf::unittest::TestCase("TC::Tests::UtilSafeRelease") {}
         virtual void run();
      };

      void UtilSafeRelease::run()
      {
         int* x = 0;
         TC::Util::SafeRelease(x);
         JFUNIT_ASSERT(x == 0);

         TC::Util::SafeReleaseArray(x);
         JFUNIT_ASSERT(x == 0);

         x = new int[10];
         TC::Util::SafeReleaseArray(x);
         JFUNIT_ASSERT(x == 0);

         x = new int;
         TC::Util::SafeRelease(x);
         JFUNIT_ASSERT(x == 0);
      }

      class UtilArraySize : public jf::unittest::TestCase
      {
      public:
         UtilArraySize() : jf::unittest::TestCase("TC::Tests::UtilArraySize") {}
         virtual void run();

         struct test_struct
         {
            int x;
            int y;
         };
      };

      void UtilArraySize::run()
      {
         char c_array[100];
         test_struct s_array[100];

         JFUNIT_ASSERT(TC::Util::ArraySize(c_array) == 100);
         JFUNIT_ASSERT(TC::Util::ArraySize(s_array) == 100);
      }
   }

   namespace Tests
   {
      UtilTestSuite::UtilTestSuite()
         :jf::unittest::TestSuite("TC::Tests::UtilTestSuite")
      {
         add_test(new UtilCompare);
         add_test(new UtilAbs);
         add_test(new UtilMinMax);
         add_test(new UtilPow);
         add_test(new UtilAlign);
         add_test(new UtilStlContainer);
         add_test(new UtilSafeRelease);
      }
   }
}

#endif
