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

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class TimeFromSeconds: public Unit::TestCase
      {
      public:
         TimeFromSeconds()
            :Unit::TestCase("TC::Tests::TimeFromSeconds")
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

      class TimeFromMilliSeconds: public Unit::TestCase
      {
      public:
         TimeFromMilliSeconds()
            :Unit::TestCase("TC::Tests::TimeFromMilliSeconds")
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

      class FromMicroSeconds: public Unit::TestCase
      {
      public:
         FromMicroSeconds()
            :Unit::TestCase("TC::Tests::FromMicroSeconds")
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

      class FromNanoSeconds: public Unit::TestCase
      {
      public:
         FromNanoSeconds()
            :Unit::TestCase("TC::Tests::FromNanoSeconds")
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

      class AddTime: public Unit::TestCase
      {
      public:
         AddTime()
            :Unit::TestCase("TC::Tests::AddTime")
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

      class AddAssignTime: public Unit::TestCase
      {
      public:
         AddAssignTime()
            :Unit::TestCase("TC::Tests::AddAssignTime")
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

      class SubTime: public Unit::TestCase
      {
      public:
         SubTime()
            :Unit::TestCase("TC::Tests::SubTime")
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

      class SubAssignTime: public Unit::TestCase
      {
      public:
         SubAssignTime()
            :Unit::TestCase("TC::Tests::SubAssignTime")
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

      class CompareTime: public Unit::TestCase
      {
      public:
         CompareTime()
            :Unit::TestCase("TC::Tests::CompareTime")
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

   namespace Tests
   {
      TimeTestSuite::TimeTestSuite()
         :Unit::TestSuite("TC::Tests::TimeTestSuite")
      {
         AddTest(new TimeFromSeconds);
         AddTest(new TimeFromMilliSeconds);
         AddTest(new FromMicroSeconds);
         AddTest(new FromNanoSeconds);
         AddTest(new AddTime);
         AddTest(new AddAssignTime);
         AddTest(new SubTime);
         AddTest(new SubAssignTime);
         AddTest(new CompareTime);
      }
   }
}

#endif
