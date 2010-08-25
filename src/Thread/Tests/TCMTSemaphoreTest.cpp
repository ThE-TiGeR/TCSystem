//*******************************************************************************
//
// *******   ***   ***               *
//    *     *     *                  *
//    *    *      *                *****
//    *    *       ***  *   *   **   *    **    ***
//    *    *          *  * *   *     *   ****  * * *
//    *     *         *   *      *   * * *     * * *
//    *      ***   ***    *     **   **   **   *   *
//                        *
//*******************************************************************************
// see http://sourceforge.net/projects/tcsystem/ for details.
// Copyright (C) 2003 - 2010 Thomas Goessler. All Rights Reserved. 
//*******************************************************************************
//
// TCSystem is the legal property of its developers.
// Please refer to the COPYRIGHT file distributed with this source distribution.
// 
// This library is free software; you can redistribute it and/or             
// modify it under the terms of the GNU Lesser General Public                
// License as published by the Free Software Foundation; either              
// version 2.1 of the License, or (at your option) any later version.        
//                                                                           
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
//                                                                           
// You should have received a copy of the GNU Lesser General Public          
// License along with this library; if not, write to the Free Software       
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
//*******************************************************************************
//  $Id: TCMTConditionPthread.h 929 2009-05-04 21:21:11Z the_____tiger $
//*******************************************************************************
#ifdef HAVE_UNIT_TESTS

#include "TCMTSemaphoreTest.h"

#include "TCMTFactory.h"
#include "TCMTSemaphore.h"
#include "TCTime.h"

#include <jf/unittest/test_case.h>

#include "TCNewEnable.h"

namespace TC
{
    namespace MT
    {
        namespace Tests
        {
            class SemaphorePostWaitTest : public jf::unittest::TestCase
            {
            public:
                SemaphorePostWaitTest()
                    :jf::unittest::TestCase("TC::MT::Tests::SemaphorePostWaitTest")
                {
                }

                virtual void run()
                {
                    {
                        SemaphorePtr m = Factory::CreateSemaphore(0);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Post());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore(1);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Wait());
                        JFUNIT_ASSERT(m->Post());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore(0);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Post());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore(0);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Post());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore(0);
                        JFUNIT_ASSERT(m);
                        Time start_time = Time::NowMonotonic();
                        JFUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        Time wait_time = Time::SinceMonotonic(start_time);
                        JFUNIT_ASSERT(wait_time >= Time::FromMilliSeconds(90));
                        JFUNIT_ASSERT(wait_time <= Time::FromMilliSeconds(200));
                        JFUNIT_ASSERT(m->Post());
                    }

                }
            };

            class SemaphoreSharedCreateTest : public jf::unittest::TestCase
            {
            public:
                SemaphoreSharedCreateTest()
                    :jf::unittest::TestCase("TC::MT::Tests::SemaphoreSharedCreateTest")
                {
                }

                virtual void run()
                {
                    {
                        SemaphorePtr m = Factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, Factory::CRM_WHEN_NOT_EXISTS);
                        JFUNIT_ASSERT(m);
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, Factory::CRM_WHEN_EXISTS);
                        JFUNIT_ASSERT(!m);
                    }

                    {
                        SemaphorePtr m1 = Factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m1);
                        SemaphorePtr m2 = Factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, Factory::CRM_WHEN_EXISTS);
                        JFUNIT_ASSERT(m2);
                    }

                    {
                        SemaphorePtr m1 = Factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m1);
                        SemaphorePtr m2 = Factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, Factory::CRM_WHEN_NOT_EXISTS);
                        JFUNIT_ASSERT(!m2);
                    }
                }
            };

            class SemaphoreSharedPostWaitTest : public jf::unittest::TestCase
            {
            public:
                SemaphoreSharedPostWaitTest()
                    :jf::unittest::TestCase("TC::MT::Tests::SemaphoreSharedPostWaitTest")
                {
                }

                virtual void run()
                {
                    {
                        SemaphorePtr m = Factory::CreateSemaphore("SemaphoreSharedPostWaitTest",
                            0, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Post());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore("SemaphoreSharedPostWaitTest",
                            1, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Wait());
                        JFUNIT_ASSERT(m->Post());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore("SemaphoreSharedPostWaitTest",
                            1, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Try());
                        JFUNIT_ASSERT(m->Post());
                    }

                    {
                        SemaphorePtr m = Factory::CreateSemaphore("SemaphoreSharedPostWaitTest",
                            0, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                        Time start_time = Time::NowMonotonic();
                        JFUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        Time wait_time = Time::SinceMonotonic(start_time);
                        JFUNIT_ASSERT(wait_time >= Time::FromMilliSeconds(90));
                        JFUNIT_ASSERT(wait_time <= Time::FromMilliSeconds(200));
                        JFUNIT_ASSERT(m->Post());
                    }
                }
            };

            SemaphoreSuite::SemaphoreSuite()
                : jf::unittest::TestSuite("TC::MT::Tests::SemaphoreSuite")
            {
                add_test(new SemaphorePostWaitTest);
                add_test(new SemaphoreSharedCreateTest);
                add_test(new SemaphoreSharedPostWaitTest);
            }

        }
    }
}

#endif
