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
// see https://bitbucket.org/the____tiger/tcsystem for details.
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

namespace tc
{
   namespace
   {
      class UtilAbs : public unit::TestCase
      {
      public:
         UtilAbs() : unit::TestCase("tc::tests::UtilAbs") {}
         virtual void Execute();
      };

      void UtilAbs::Execute()
      {
         TCUNIT_ASSERT(tc::util::Abs(-1) == 1);
         TCUNIT_ASSERT(tc::util::Abs( 1) == 1);
         TCUNIT_ASSERT(tc::util::Abs(-1234567) == 1234567);
         TCUNIT_ASSERT(tc::util::Abs( 1234567) == 1234567);

         TCUNIT_ASSERT(tc::util::Abs( 0.0) == 0.0);
         TCUNIT_ASSERT(tc::util::Abs(-0.0) == 0.0);
         TCUNIT_ASSERT(tc::util::Abs( 0.1) == 0.1);
         TCUNIT_ASSERT(tc::util::Abs(-0.1) == 0.1);
         TCUNIT_ASSERT(tc::util::Abs( 1.1) == 1.1);
         TCUNIT_ASSERT(tc::util::Abs(-1.1) == 1.1);
      }

      class UtilMinMax : public unit::TestCase
      {
      public:
         UtilMinMax() : unit::TestCase("tc::tests::UtilMinMax") {}
         virtual void Execute();
      };

      void UtilMinMax::Execute()
      {
         TCUNIT_ASSERT(tc::util::Min(1, 2) == 1);

         TCUNIT_ASSERT(tc::util::Min(-2, -3) == -3);

         TCUNIT_ASSERT(tc::util::Min(1, 2, 3) == 1);

         TCUNIT_ASSERT(tc::util::Min(1, -2, -3) == -3);

         TCUNIT_ASSERT(tc::util::Max(1, 2) == 2);

         TCUNIT_ASSERT(tc::util::Max(-2, -3) == -2);

         TCUNIT_ASSERT(tc::util::Max(1, 2, 3) == 3);

         TCUNIT_ASSERT(tc::util::Max(1, -2, -3) == 1);
      }



      class UtilStlContainer : public unit::TestCase
      {
      public:
         UtilStlContainer() : unit::TestCase("tc::tests::UtilStlContainer") {}
         virtual void Execute();
      };

      void UtilStlContainer::Execute()
      {
         std::vector<int> vec(10);

         TCUNIT_ASSERT(vec.size() == 10);
         TCUNIT_ASSERT(vec.capacity() == 10);

         tc::util::FreeMemoryOfStlContainer(vec);
         TCUNIT_ASSERT(vec.size() == 0);
         TCUNIT_ASSERT(vec.capacity() == 0);

         std::vector<int> vec1(10);
         vec1.push_back(1);

         TCUNIT_ASSERT(vec1.size() == 11);
         TCUNIT_ASSERT(vec1.capacity() > 11);

         tc::util::MinimizeMemoryOfStlContainer(vec1);
         TCUNIT_ASSERT(vec1.size() == 11);
         TCUNIT_ASSERT(vec1.capacity() == 11);
      }

      class UtilSafeRelease : public unit::TestCase
      {
      public:
         UtilSafeRelease() : unit::TestCase("tc::tests::UtilSafeRelease") {}
         virtual void Execute();
      };

      void UtilSafeRelease::Execute()
      {
         int* x = 0;
         tc::util::SafeRelease(x);
         TCUNIT_ASSERT(x == 0);

         tc::util::SafeReleaseArray(x);
         TCUNIT_ASSERT(x == 0);

         x = new int[10];
         tc::util::SafeReleaseArray(x);
         TCUNIT_ASSERT(x == 0);

         x = new int;
         tc::util::SafeRelease(x);
         TCUNIT_ASSERT(x == 0);
      }

      class UtilArraySize : public unit::TestCase
      {
      public:
         UtilArraySize() : unit::TestCase("tc::tests::UtilArraySize") {}
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

         TCUNIT_ASSERT(tc::util::ArraySize(c_array) == 100);
         TCUNIT_ASSERT(tc::util::ArraySize(s_array) == 100);
      }

      class UtilSwap : public unit::TestCase
      {
      public:
         UtilSwap() : unit::TestCase("tc::tests::UtilSwap") {}
         virtual void Execute()
         {
            TCUNIT_ASSERT(TestSwap( uint8_t(0)));
            TCUNIT_ASSERT(TestSwap( uint8_t(125)));
            TCUNIT_ASSERT(TestSwap( uint8_t(255)));

            TCUNIT_ASSERT(TestSwap(uint16_t(0)));
            TCUNIT_ASSERT(TestSwap(uint16_t(12345)));
            TCUNIT_ASSERT(TestSwap(uint16_t(65535)));

            TCUNIT_ASSERT(TestSwap(uint32_t(0)));
            TCUNIT_ASSERT(TestSwap(uint32_t(1234567890ul)));
            TCUNIT_ASSERT(TestSwap(uint32_t(4294967295ul)));

            TCUNIT_ASSERT(TestSwap(int8_t(0)));
            TCUNIT_ASSERT(TestSwap(int8_t(127)));
            TCUNIT_ASSERT(TestSwap(int8_t(-128)));

            TCUNIT_ASSERT(TestSwap(int16_t(0)));
            TCUNIT_ASSERT(TestSwap(int16_t(32767)));
            TCUNIT_ASSERT(TestSwap(int16_t(-32768)));

            TCUNIT_ASSERT(TestSwap(int32_t(0)));
            TCUNIT_ASSERT(TestSwap(int32_t(2147483647)));
            TCUNIT_ASSERT(TestSwap(int32_t(-2147483647)));

            TCUNIT_ASSERT(TestSwap(double(123456.789)));
            TCUNIT_ASSERT(TestSwap(float(123456.789f)));

            TCUNIT_ASSERT(TestSwap(uint64_t(0)));
            TCUNIT_ASSERT(TestSwap(uint64_t(1234567)));
            TCUNIT_ASSERT(TestSwap(uint64_t(TC_UINT64_VAL(12345678901234567890))));
            TCUNIT_ASSERT(TestSwap(uint64_t(TC_UINT64_VAL(18446744073709551615))));

            TCUNIT_ASSERT(TestSwap(int64_t(0)));
            TCUNIT_ASSERT(TestSwap(int64_t(1234567)));
            TCUNIT_ASSERT(TestSwap(int64_t(-1234567)));
            TCUNIT_ASSERT(TestSwap(int64_t(TC_SINT64_VAL(9223372036854775807))));
            TCUNIT_ASSERT(TestSwap(int64_t(TC_SINT64_VAL(-9223372036854775807))));

         }

      private:
         template<typename T>
         bool TestSwap(T val)
         {
            T orig = val;
            tc::util::SwapBytes(val);
            tc::util::SwapBytes(val);
            return val == orig;
         }
      };
   }

   namespace tests
   {
      UtilTestSuite::UtilTestSuite()
         :unit::TestSuite("tc::tests::UtilTestSuite")
      {
         AddTest(unit::Test::Ptr(new UtilAbs));
         AddTest(unit::Test::Ptr(new UtilMinMax));
         AddTest(unit::Test::Ptr(new UtilStlContainer));
         AddTest(unit::Test::Ptr(new UtilSafeRelease));
         AddTest(unit::Test::Ptr(new UtilSwap));
      }
   }
}

#endif
