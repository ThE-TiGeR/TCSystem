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

#include "TCTimeTest.h"

#include "TCTime.h"

#include <jf/unittest/test_case.h>

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class TimeFromSeconds: public jf::unittest::TestCase
      {
      public:
         TimeFromSeconds()
            :jf::unittest::TestCase("TC::Tests::TimeFromSeconds")
         {
         }

         virtual void run()
         {
            Time t = Time::FromSeconds(100);
            JFUNIT_ASSERT(t.Seconds() == 100);
            JFUNIT_ASSERT(t.NanoSeconds() == 0);

            JFUNIT_ASSERT(t.ToMilliSeconds() == 100 * 1000);
            JFUNIT_ASSERT(t.ToMicroSeconds() == 100 * 1000 * 1000);
            JFUNIT_ASSERT(t.ToNanoSeconds() == 100 * 1000 * 1000 * TC_UINT64_VAL(1000));
         }
      };

      class TimeFromMilliSeconds: public jf::unittest::TestCase
      {
      public:
         TimeFromMilliSeconds()
            :jf::unittest::TestCase("TC::Tests::TimeFromMilliSeconds")
         {
         }

         virtual void run()
         {
            {
               Time t = Time::FromMilliSeconds(100);
               JFUNIT_ASSERT(t.Seconds() == 0);
               JFUNIT_ASSERT(t.NanoSeconds() == 100 * 1000 * 1000);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 100);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 100 * 1000);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 100 * 1000 * 1000);
            }

            {
               Time t = Time::FromMilliSeconds(1100);
               JFUNIT_ASSERT(t.Seconds() == 1);
               JFUNIT_ASSERT(t.NanoSeconds() == 100 * 1000 * 1000);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 1100);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 1100 * 1000);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 1100 * 1000 * 1000);
            }
         }
      };

      class FromMicroSeconds: public jf::unittest::TestCase
      {
      public:
         FromMicroSeconds()
            :jf::unittest::TestCase("TC::Tests::FromMicroSeconds")
         {
         }

         virtual void run()
         {
            {
               Time t = Time::FromMicroSeconds(100);
               JFUNIT_ASSERT(t.Seconds() == 0);
               JFUNIT_ASSERT(t.NanoSeconds() == 100 * 1000);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 0);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 100);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 100 * 1000);
            }

            {
               Time t = Time::FromMicroSeconds(1100);
               JFUNIT_ASSERT(t.Seconds() == 0);
               JFUNIT_ASSERT(t.NanoSeconds() == 1100 * 1000);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 1);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 1100);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 1100 * 1000);
            }

            {
               Time t = Time::FromMicroSeconds(1100100);
               JFUNIT_ASSERT(t.Seconds() == 1);
               JFUNIT_ASSERT(t.NanoSeconds() == 100100 * 1000);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 1100);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 1100100);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 1100100 * 1000);
            }
         }
      };

      class FromNanoSeconds: public jf::unittest::TestCase
      {
      public:
         FromNanoSeconds()
            :jf::unittest::TestCase("TC::Tests::FromNanoSeconds")
         {
         }

         virtual void run()
         {
            {
               Time t = Time::FromNanoSeconds(100);
               JFUNIT_ASSERT(t.Seconds() == 0);
               JFUNIT_ASSERT(t.NanoSeconds() == 100);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 0);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 0);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 100);
            }

            {
               Time t = Time::FromNanoSeconds(1100);
               JFUNIT_ASSERT(t.Seconds() == 0);
               JFUNIT_ASSERT(t.NanoSeconds() == 1100);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 0);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 1);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 1100);
            }

            {
               Time t = Time::FromNanoSeconds(1100100);
               JFUNIT_ASSERT(t.Seconds() == 0);
               JFUNIT_ASSERT(t.NanoSeconds() == 1100100);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 1);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 1100);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 1100100);
            }

            {
               Time t = Time::FromNanoSeconds(1100100100);
               JFUNIT_ASSERT(t.Seconds() == 1);
               JFUNIT_ASSERT(t.NanoSeconds() == 100100100);

               JFUNIT_ASSERT(t.ToMilliSeconds() == 1100);
               JFUNIT_ASSERT(t.ToMicroSeconds() == 1100100);
               JFUNIT_ASSERT(t.ToNanoSeconds() == 1100100100);
            }

         }
      };

      class AddTime: public jf::unittest::TestCase
      {
      public:
         AddTime()
            :jf::unittest::TestCase("TC::Tests::AddTime")
         {
         }

         virtual void run()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               Time t3 = t1 + t2;
               JFUNIT_ASSERT(t3.Seconds() == 100);
               JFUNIT_ASSERT(t3.NanoSeconds() == 100);
            }

            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(TC_UINT64_VAL(101110001000));
               Time t3 = t1 + t2;
               JFUNIT_ASSERT(t3.Seconds() == 201);
               JFUNIT_ASSERT(t3.NanoSeconds() == 110001000);
            }
         }
      };

      class AddAssignTime: public jf::unittest::TestCase
      {
      public:
         AddAssignTime()
            :jf::unittest::TestCase("TC::Tests::AddAssignTime")
         {
         }

         virtual void run()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               t2 += t1;
               JFUNIT_ASSERT(t2.Seconds() == 100);
               JFUNIT_ASSERT(t2.NanoSeconds() == 100);
            }

            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(TC_UINT64_VAL(101110001000));
               t2 += t1;
               JFUNIT_ASSERT(t2.Seconds() == 201);
               JFUNIT_ASSERT(t2.NanoSeconds() == 110001000);
            }
         }
      };

      class SubTime: public jf::unittest::TestCase
      {
      public:
         SubTime()
            :jf::unittest::TestCase("TC::Tests::SubTime")
         {
         }

         virtual void run()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               Time t3 = t1 - t2;
               JFUNIT_ASSERT(t3.Seconds() == 99);
               JFUNIT_ASSERT(t3.NanoSeconds() == 1 * 1000 * 1000 * 1000 - 100);
            }
         }
      };

      class SubAssignTime: public jf::unittest::TestCase
      {
      public:
         SubAssignTime()
            :jf::unittest::TestCase("TC::Tests::SubAssignTime")
         {
         }

         virtual void run()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               t1 -= t2;
               JFUNIT_ASSERT(t1.Seconds() == 99);
               JFUNIT_ASSERT(t1.NanoSeconds() == 1 * 1000 * 1000 * 1000 - 100);
            }
         }
      };

      class CompareTime: public jf::unittest::TestCase
      {
      public:
         CompareTime()
            :jf::unittest::TestCase("TC::Tests::CompareTime")
         {
         }

         virtual void run()
         {
            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromNanoSeconds(100);
               JFUNIT_ASSERT(!(t1 == t2));
               JFUNIT_ASSERT(t1 != t2);
            }

            {
               Time t1 = Time::FromSeconds(100);
               Time t2 = Time::FromMilliSeconds(100 * 1000);
               JFUNIT_ASSERT(t1 == t2);
               JFUNIT_ASSERT(!(t1 != t2));
            }
         }
      };
   }

   namespace Tests
   {
      TimeTestSuite::TimeTestSuite()
         :jf::unittest::TestSuite("TC::Tests::TimeTestSuite")
      {
         add_test(new TimeFromSeconds);
         add_test(new TimeFromMilliSeconds);
         add_test(new FromMicroSeconds);
         add_test(new FromNanoSeconds);
         add_test(new AddTime);
         add_test(new AddAssignTime);
         add_test(new SubTime);
         add_test(new SubAssignTime);
         add_test(new CompareTime);
      }
   }
}

#endif
