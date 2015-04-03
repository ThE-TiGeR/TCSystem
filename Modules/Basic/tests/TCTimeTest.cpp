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
//  $Id: TCTimeTest.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//----------------------------------------------------------------------
#ifdef HAVE_UNIT_TESTS

#include "TCTimeTest.h"

#include "TCTime.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace
   {
      class TimeFromSeconds: public unit::TestCase
      {
      public:
         TimeFromSeconds()
            :unit::TestCase("tc::tests::TimeFromSeconds")
         {
         }

         virtual void Execute()
         {
            Time t = Time::FromSeconds(100);
            TCUNIT_ASSERT(t.Seconds() == 100);
            TCUNIT_ASSERT(t.NanoSeconds() == 0);

            TCUNIT_ASSERT(t.ToMilliSeconds() == 100 * 1000);
            TCUNIT_ASSERT(t.ToMicroSeconds() == 100 * 1000 * 1000);
            TCUNIT_ASSERT(t.ToNanoSeconds() == 100 * 1000 * 1000 * TC_UINT64_VAL(1000));
         }
      };

      class TimeFromMilliSeconds: public unit::TestCase
      {
      public:
         TimeFromMilliSeconds()
            :unit::TestCase("tc::tests::TimeFromMilliSeconds")
         {
         }

         virtual void Execute()
         {
            {
               Time t = Time::FromMilliSeconds(100);
               TCUNIT_ASSERT(t.Seconds() == 0);
               TCUNIT_ASSERT(t.NanoSeconds() == 100 * 1000 * 1000);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 100);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 100 * 1000);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 100 * 1000 * 1000);
            }

            {
               Time t = Time::FromMilliSeconds(1100);
               TCUNIT_ASSERT(t.Seconds() == 1);
               TCUNIT_ASSERT(t.NanoSeconds() == 100 * 1000 * 1000);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 1100);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 1100 * 1000);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 1100 * 1000 * 1000);
            }
         }
      };

      class FromMicroSeconds: public unit::TestCase
      {
      public:
         FromMicroSeconds()
            :unit::TestCase("tc::tests::FromMicroSeconds")
         {
         }

         virtual void Execute()
         {
            {
               Time t = Time::FromMicroSeconds(100);
               TCUNIT_ASSERT(t.Seconds() == 0);
               TCUNIT_ASSERT(t.NanoSeconds() == 100 * 1000);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 0);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 100);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 100 * 1000);
            }

            {
               Time t = Time::FromMicroSeconds(1100);
               TCUNIT_ASSERT(t.Seconds() == 0);
               TCUNIT_ASSERT(t.NanoSeconds() == 1100 * 1000);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 1);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 1100);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 1100 * 1000);
            }

            {
               Time t = Time::FromMicroSeconds(1100100);
               TCUNIT_ASSERT(t.Seconds() == 1);
               TCUNIT_ASSERT(t.NanoSeconds() == 100100 * 1000);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 1100);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 1100100);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 1100100 * 1000);
            }
         }
      };

      class FromNanoSeconds: public unit::TestCase
      {
      public:
         FromNanoSeconds()
            :unit::TestCase("tc::tests::FromNanoSeconds")
         {
         }

         virtual void Execute()
         {
            {
               Time t = Time::FromNanoSeconds(100);
               TCUNIT_ASSERT(t.Seconds() == 0);
               TCUNIT_ASSERT(t.NanoSeconds() == 100);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 0);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 0);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 100);
            }

            {
               Time t = Time::FromNanoSeconds(1100);
               TCUNIT_ASSERT(t.Seconds() == 0);
               TCUNIT_ASSERT(t.NanoSeconds() == 1100);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 0);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 1);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 1100);
            }

            {
               Time t = Time::FromNanoSeconds(1100100);
               TCUNIT_ASSERT(t.Seconds() == 0);
               TCUNIT_ASSERT(t.NanoSeconds() == 1100100);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 1);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 1100);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 1100100);
            }

            {
               Time t = Time::FromNanoSeconds(1100100100);
               TCUNIT_ASSERT(t.Seconds() == 1);
               TCUNIT_ASSERT(t.NanoSeconds() == 100100100);

               TCUNIT_ASSERT(t.ToMilliSeconds() == 1100);
               TCUNIT_ASSERT(t.ToMicroSeconds() == 1100100);
               TCUNIT_ASSERT(t.ToNanoSeconds() == 1100100100);
            }

         }
      };

      class AddTime: public unit::TestCase
      {
      public:
         AddTime()
            :unit::TestCase("tc::tests::AddTime")
         {
         }

         virtual void Execute()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               Time t3 = t1 + t2;
               TCUNIT_ASSERT(t3.Seconds() == 100);
               TCUNIT_ASSERT(t3.NanoSeconds() == 100);
            }

            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(TC_UINT64_VAL(101110001000));
               Time t3 = t1 + t2;
               TCUNIT_ASSERT(t3.Seconds() == 201);
               TCUNIT_ASSERT(t3.NanoSeconds() == 110001000);
            }
         }
      };

      class AddAssignTime: public unit::TestCase
      {
      public:
         AddAssignTime()
            :unit::TestCase("tc::tests::AddAssignTime")
         {
         }

         virtual void Execute()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               t2 += t1;
               TCUNIT_ASSERT(t2.Seconds() == 100);
               TCUNIT_ASSERT(t2.NanoSeconds() == 100);
            }

            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(TC_UINT64_VAL(101110001000));
               t2 += t1;
               TCUNIT_ASSERT(t2.Seconds() == 201);
               TCUNIT_ASSERT(t2.NanoSeconds() == 110001000);
            }
         }
      };

      class SubTime: public unit::TestCase
      {
      public:
         SubTime()
            :unit::TestCase("tc::tests::SubTime")
         {
         }

         virtual void Execute()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               Time t3 = t1 - t2;
               TCUNIT_ASSERT(t3.Seconds() == 99);
               TCUNIT_ASSERT(t3.NanoSeconds() == 1 * 1000 * 1000 * 1000 - 100);
            }
         }
      };

      class SubAssignTime: public unit::TestCase
      {
      public:
         SubAssignTime()
            :unit::TestCase("tc::tests::SubAssignTime")
         {
         }

         virtual void Execute()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               t1 -= t2;
               TCUNIT_ASSERT(t1.Seconds() == 99);
               TCUNIT_ASSERT(t1.NanoSeconds() == 1 * 1000 * 1000 * 1000 - 100);
            }
         }
      };

      class CompareTime: public unit::TestCase
      {
      public:
         CompareTime()
            :unit::TestCase("tc::tests::CompareTime")
         {
         }

         virtual void Execute()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               TCUNIT_ASSERT(!(t1 == t2));
               TCUNIT_ASSERT(t1 != t2);
            }

            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromMilliSeconds(100 * 1000);
               TCUNIT_ASSERT(t1 == t2);
               TCUNIT_ASSERT(!(t1 != t2));
            }
         }
      };
   }

   namespace tests
   {
      TimeTestSuite::TimeTestSuite()
         :unit::TestSuite("tc::tests::TimeTestSuite")
      {
         AddTest(unit::Test::Ptr(new TimeFromSeconds));
         AddTest(unit::Test::Ptr(new TimeFromMilliSeconds));
         AddTest(unit::Test::Ptr(new FromMicroSeconds));
         AddTest(unit::Test::Ptr(new FromNanoSeconds));
         AddTest(unit::Test::Ptr(new AddTime));
         AddTest(unit::Test::Ptr(new AddAssignTime));
         AddTest(unit::Test::Ptr(new SubTime));
         AddTest(unit::Test::Ptr(new SubAssignTime));
         AddTest(unit::Test::Ptr(new CompareTime));
      }
   }
}

#endif
