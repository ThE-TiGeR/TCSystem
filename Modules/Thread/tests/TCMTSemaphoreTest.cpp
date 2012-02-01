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
// Copyright (C) 2003 - 2012 Thomas Goessler. All Rights Reserved. 
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
//  $Id$
//*******************************************************************************
#ifdef HAVE_UNIT_TESTS

#include "TCMTSemaphoreTest.h"

#include "TCMTFactory.h"
#include "TCMTSemaphore.h"
#include "TCTime.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
    namespace multi_threading
    {
        namespace tests
        {
            class SemaphorePostWaitTest : public unit::TestCase
            {
            public:
                SemaphorePostWaitTest()
                    :unit::TestCase("tc::multi_threading::tests::SemaphorePostWaitTest")
                {
                }

                virtual void Execute()
                {
                    {
                        SemaphorePtr m = factory::CreateSemaphore(0);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Post());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore(1);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Post());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore(0);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Post());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore(0);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Post());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore(0);
                        TCUNIT_ASSERT(m);
                        Time start_time = Time::NowMonotonic();
                        TCUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        Time wait_time = Time::SinceMonotonic(start_time);
                        TCUNIT_ASSERT(wait_time >= Time::FromMilliSeconds(90));
                        TCUNIT_ASSERT(wait_time <= Time::FromMilliSeconds(200));
                        TCUNIT_ASSERT(m->Post());
                    }

                }
            };

            class SemaphoreSharedCreateTest : public unit::TestCase
            {
            public:
                SemaphoreSharedCreateTest()
                    :unit::TestCase("tc::multi_threading::tests::SemaphoreSharedCreateTest")
                {
                }

                virtual void Execute()
                {
                    {
                        SemaphorePtr m = factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, factory::CRM_WHEN_NOT_EXISTS);
                        TCUNIT_ASSERT(m);
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, factory::CRM_WHEN_EXISTS);
                        TCUNIT_ASSERT(!m);
                    }

                    {
                        SemaphorePtr m1 = factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m1);
                        SemaphorePtr m2 = factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, factory::CRM_WHEN_EXISTS);
                        TCUNIT_ASSERT(m2);
                    }

                    {
                        SemaphorePtr m1 = factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m1);
                        SemaphorePtr m2 = factory::CreateSemaphore("SemaphoreSharedCreateTest",
                            0, factory::CRM_WHEN_NOT_EXISTS);
                        TCUNIT_ASSERT(!m2);
                    }
                }
            };

            class SemaphoreSharedPostWaitTest : public unit::TestCase
            {
            public:
                SemaphoreSharedPostWaitTest()
                    :unit::TestCase("tc::multi_threading::tests::SemaphoreSharedPostWaitTest")
                {
                }

                virtual void Execute()
                {
                    {
                        SemaphorePtr m = factory::CreateSemaphore("SemaphoreSharedPostWaitTest",
                            0, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Post());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore("SemaphoreSharedPostWaitTest",
                            1, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Post());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore("SemaphoreSharedPostWaitTest",
                            1, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Post());
                    }

                    {
                        SemaphorePtr m = factory::CreateSemaphore("SemaphoreSharedPostWaitTest",
                            0, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        Time start_time = Time::NowMonotonic();
                        TCUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        Time wait_time = Time::SinceMonotonic(start_time);
                        TCUNIT_ASSERT(wait_time >= Time::FromMilliSeconds(90));
                        TCUNIT_ASSERT(wait_time <= Time::FromMilliSeconds(200));
                        TCUNIT_ASSERT(m->Post());
                    }
                }
            };

            SemaphoreSuite::SemaphoreSuite()
                : unit::TestSuite("tc::multi_threading::tests::SemaphoreSuite")
            {
                AddTest(unit::Test::Ptr(new SemaphorePostWaitTest));
                AddTest(unit::Test::Ptr(new SemaphoreSharedCreateTest));
                AddTest(unit::Test::Ptr(new SemaphoreSharedPostWaitTest));
            }

        }
    }
}

#endif
