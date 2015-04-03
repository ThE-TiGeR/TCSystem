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
// see https://bitbucket.org/the____tiger/tcsystem for details.
// Copyright (C) 2003 - 2014 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCMTEventTest.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifdef HAVE_UNIT_TESTS

#include "TCMTEventTest.h"

#include "TCMTEvent.h"
#include "TCMTFactory.h"
#include "TCTime.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
    namespace multi_threading
    {
        namespace tests
        {

            class EventSetWaitTest : public unit::TestCase
            {
            public:
                EventSetWaitTest()
                    :unit::TestCase("tc::multi_threading::tests::EventSetWaitTest")
                {
                }

                virtual void Execute()
                {
                    {
                        EventPtr m = factory::CreateEvent(false, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = factory::CreateEvent(false, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = factory::CreateEvent(true, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = factory::CreateEvent(true, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Wait());
                    }
                }
            };

            class EventTryTest : public unit::TestCase
            {
            public:
                EventTryTest()
                    :unit::TestCase("tc::multi_threading::tests::EventTryTest")
                {
                }

                virtual void Execute()
                {
                    {
                        EventPtr m = factory::CreateEvent(false, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(!m->Try());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = factory::CreateEvent(false, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = factory::CreateEvent(true, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(!m->Try());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = factory::CreateEvent(true, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Reset());
                        TCUNIT_ASSERT(!m->Try());
                    }
                }
            };

            class EventWaitTimeTest : public unit::TestCase
            {
            public:
                EventWaitTimeTest()
                    :unit::TestCase("tc::multi_threading::tests::EventWaitTimeTest")
                {
                }

                virtual void Execute()
                {
                    {
                        EventPtr m = factory::CreateEvent(false, false);
                        TCUNIT_ASSERT(m);
                        Time start_time = Time::NowMonotonic();
                        TCUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        Time wait_time = Time::SinceMonotonic(start_time);
                        TCUNIT_ASSERT(wait_time >= Time::FromMilliSeconds(90));
                        TCUNIT_ASSERT(wait_time <= Time::FromMilliSeconds(200));

                        TCUNIT_ASSERT(m->Set());
                        start_time = Time::NowMonotonic();
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        wait_time = Time::SinceMonotonic(start_time);
                        TCUNIT_ASSERT(wait_time < Time::FromMilliSeconds(100));
                    }

                    {
                        EventPtr m = factory::CreateEvent(false, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                    }

                    {
                        EventPtr m = factory::CreateEvent(true, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                    }

                    {
                        EventPtr m = factory::CreateEvent(true, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->Reset());
                        Time start_time = Time::NowMonotonic();
                        TCUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        Time wait_time = Time::SinceMonotonic(start_time);
                        TCUNIT_ASSERT(wait_time >= Time::FromMilliSeconds(90));
                        TCUNIT_ASSERT(wait_time <= Time::FromMilliSeconds(200));
                    }
                }
            };

            EventSuite::EventSuite()
                : unit::TestSuite("tc::multi_threading::tests::EventSuite")
            {
                AddTest(unit::Test::Ptr(new EventSetWaitTest));
                AddTest(unit::Test::Ptr(new EventTryTest));
                AddTest(unit::Test::Ptr(new EventWaitTimeTest));
            }

        }
    }
}

#endif
