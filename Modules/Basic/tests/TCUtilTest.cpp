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

#include "TCUtilTest.h"

#include "TCUtil.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class UtilAbs : public Unit::TestCase
      {
      public:
         UtilAbs() : Unit::TestCase("TC::Tests::UtilAbs") {}
         virtual void Execute();
      };

      void UtilAbs::Execute()
      {
         TCUNIT_ASSERT(TC::Util::Abs(-1) == 1);
         TCUNIT_ASSERT(TC::Util::Abs( 1) == 1);
         TCUNIT_ASSERT(TC::Util::Abs(-1234567) == 1234567);
         TCUNIT_ASSERT(TC::Util::Abs( 1234567) == 1234567);

         TCUNIT_ASSERT(TC::Util::Abs( 0.0) == 0.0);
         TCUNIT_ASSERT(TC::Util::Abs(-0.0) == 0.0);
         TCUNIT_ASSERT(TC::Util::Abs( 0.1) == 0.1);
         TCUNIT_ASSERT(TC::Util::Abs(-0.1) == 0.1);
         TCUNIT_ASSERT(TC::Util::Abs( 1.1) == 1.1);
         TCUNIT_ASSERT(TC::Util::Abs(-1.1) == 1.1);
      }

      class UtilMinMax : public Unit::TestCase
      {
      public:
         UtilMinMax() : Unit::TestCase("TC::Tests::UtilMinMax") {}
         virtual void Execute();
      };

      void UtilMinMax::Execute()
      {
         TCUNIT_ASSERT(TC::Util::Min(1, 2) == 1);

         TCUNIT_ASSERT(TC::Util::Min(-2, -3) == -3);

         TCUNIT_ASSERT(TC::Util::Min(1, 2, 3) == 1);

         TCUNIT_ASSERT(TC::Util::Min(1, -2, -3) == -3);

         TCUNIT_ASSERT(TC::Util::Max(1, 2) == 2);

         TCUNIT_ASSERT(TC::Util::Max(-2, -3) == -2);

         TCUNIT_ASSERT(TC::Util::Max(1, 2, 3) == 3);

         TCUNIT_ASSERT(TC::Util::Max(1, -2, -3) == 1);
      }



      class UtilStlContainer : public Unit::TestCase
      {
      public:
         UtilStlContainer() : Unit::TestCase("TC::Tests::UtilStlContainer") {}
         virtual void Execute();
      };

      void UtilStlContainer::Execute()
      {
         std::vector<int> vec(10);

         TCUNIT_ASSERT(vec.size() == 10);
         TCUNIT_ASSERT(vec.capacity() == 10);

         TC::Util::FreeMemoryOfStlContainer(vec);
         TCUNIT_ASSERT(vec.size() == 0);
         TCUNIT_ASSERT(vec.capacity() == 0);

         std::vector<int> vec1(10);
         vec1.push_back(1);

         TCUNIT_ASSERT(vec1.size() == 11);
         TCUNIT_ASSERT(vec1.capacity() > 11);

         TC::Util::MinimizeMemoryOfStlContainer(vec1);
         TCUNIT_ASSERT(vec1.size() == 11);
         TCUNIT_ASSERT(vec1.capacity() == 11);
      }

      class UtilSafeRelease : public Unit::TestCase
      {
      public:
         UtilSafeRelease() : Unit::TestCase("TC::Tests::UtilSafeRelease") {}
         virtual void Execute();
      };

      void UtilSafeRelease::Execute()
      {
         int* x = 0;
         TC::Util::SafeRelease(x);
         TCUNIT_ASSERT(x == 0);

         TC::Util::SafeReleaseArray(x);
         TCUNIT_ASSERT(x == 0);

         x = new int[10];
         TC::Util::SafeReleaseArray(x);
         TCUNIT_ASSERT(x == 0);

         x = new int;
         TC::Util::SafeRelease(x);
         TCUNIT_ASSERT(x == 0);
      }

      class UtilArraySize : public Unit::TestCase
      {
      public:
         UtilArraySize() : Unit::TestCase("TC::Tests::UtilArraySize") {}
         virtual void Execute();

         struct test_struct
         {
            int x;
            int y;
         };
      };

      void UtilArraySize::Execute()
      {
         char c_array[100];
         test_struct s_array[100];

         TCUNIT_ASSERT(TC::Util::ArraySize(c_array) == 100);
         TCUNIT_ASSERT(TC::Util::ArraySize(s_array) == 100);
      }

      class UtilSwap : public Unit::TestCase
      {
      public:
         UtilSwap() : Unit::TestCase("TC::Tests::UtilSwap") {}
         virtual void Execute()
         {
            TCUNIT_ASSERT(TestSwap( uint8(0)));
            TCUNIT_ASSERT(TestSwap( uint8(125)));
            TCUNIT_ASSERT(TestSwap( uint8(255)));

            TCUNIT_ASSERT(TestSwap(uint16(0)));
            TCUNIT_ASSERT(TestSwap(uint16(12345)));
            TCUNIT_ASSERT(TestSwap(uint16(65535)));

            TCUNIT_ASSERT(TestSwap(uint32(0)));
            TCUNIT_ASSERT(TestSwap(uint32(1234567890ul)));
            TCUNIT_ASSERT(TestSwap(uint32(4294967295ul)));

            TCUNIT_ASSERT(TestSwap(sint8(0)));
            TCUNIT_ASSERT(TestSwap(sint8(127)));
            TCUNIT_ASSERT(TestSwap(sint8(-128)));

            TCUNIT_ASSERT(TestSwap(sint16(0)));
            TCUNIT_ASSERT(TestSwap(sint16(32767)));
            TCUNIT_ASSERT(TestSwap(sint16(-32768)));

            TCUNIT_ASSERT(TestSwap(sint32(0)));
            TCUNIT_ASSERT(TestSwap(sint32(2147483647)));
            TCUNIT_ASSERT(TestSwap(sint32(-2147483647)));

            TCUNIT_ASSERT(TestSwap(double(123456.789)));
            TCUNIT_ASSERT(TestSwap(float(123456.789f)));

            TCUNIT_ASSERT(TestSwap(uint64(0)));
            TCUNIT_ASSERT(TestSwap(uint64(1234567)));
            TCUNIT_ASSERT(TestSwap(uint64(TC_UINT64_VAL(12345678901234567890))));
            TCUNIT_ASSERT(TestSwap(uint64(TC_UINT64_VAL(18446744073709551615))));

            TCUNIT_ASSERT(TestSwap(sint64(0)));
            TCUNIT_ASSERT(TestSwap(sint64(1234567)));
            TCUNIT_ASSERT(TestSwap(sint64(-1234567)));
            TCUNIT_ASSERT(TestSwap(sint64(TC_SINT64_VAL(9223372036854775807))));
            TCUNIT_ASSERT(TestSwap(sint64(TC_SINT64_VAL(-9223372036854775807))));

         }

      private:
         template<typename T>
         bool TestSwap(T val)
         {
            T orig = val;
            TC::Util::SwapBytes(val);
            TC::Util::SwapBytes(val);
            return val == orig;
         }
      };
   }

   namespace Tests
   {
      UtilTestSuite::UtilTestSuite()
         :Unit::TestSuite("TC::Tests::UtilTestSuite")
      {
         AddTest(Unit::Test::Ptr(new UtilAbs));
         AddTest(Unit::Test::Ptr(new UtilMinMax));
         AddTest(Unit::Test::Ptr(new UtilStlContainer));
         AddTest(Unit::Test::Ptr(new UtilSafeRelease));
         AddTest(Unit::Test::Ptr(new UtilSwap));
      }
   }
}

#endif
