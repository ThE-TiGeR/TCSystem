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

#include "TCMTEventTest.h"

#include "TCMTEvent.h"
#include "TCMTFactory.h"
#include "TCTime.h"

#include <jf/unittest/test_case.h>

#include "TCNewEnable.h"

namespace TC
{
    namespace MT
    {
        namespace Tests
        {

            class EventSetWaitTest : public jf::unittest::TestCase
            {
            public:
                EventSetWaitTest()
                    :jf::unittest::TestCase("TC::MT::Tests::EventSetWaitTest")
                {
                }

                virtual void run()
                {
                    {
                        EventPtr m = Factory::CreateEvent(false, false);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(false, true);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Wait());
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, false);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->Wait());
                        JFUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, true);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Wait());
                        JFUNIT_ASSERT(m->Wait());
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->Wait());
                        JFUNIT_ASSERT(m->Wait());
                    }
                }
            };

            class EventTryTest : public jf::unittest::TestCase
            {
            public:
                EventTryTest()
                    :jf::unittest::TestCase("TC::MT::Tests::EventTryTest")
                {
                }

                virtual void run()
                {
                    {
                        EventPtr m = Factory::CreateEvent(false, false);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(!m->Try());
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(false, true);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Try());
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, false);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(!m->Try());
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->Try());
                        JFUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, true);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Try());
                        JFUNIT_ASSERT(m->Try());
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->Try());
                        JFUNIT_ASSERT(m->Try());
                        JFUNIT_ASSERT(m->Reset());
                        JFUNIT_ASSERT(!m->Try());
                    }
                }
            };

            class EventWaitTimeTest : public jf::unittest::TestCase
            {
            public:
                EventWaitTimeTest()
                    :jf::unittest::TestCase("TC::MT::Tests::EventWaitTimeTest")
                {
                }

                virtual void run()
                {
                    {
                        EventPtr m = Factory::CreateEvent(false, false);
                        JFUNIT_ASSERT(m);
                        Time start_time = Time::NowMonotonic();
                        JFUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        Time wait_time = Time::SinceMonotonic(start_time);
                        JFUNIT_ASSERT(wait_time >= Time::FromMilliSeconds(90));
                        JFUNIT_ASSERT(wait_time <= Time::FromMilliSeconds(200));

                        JFUNIT_ASSERT(m->Set());
                        start_time = Time::NowMonotonic();
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        wait_time = Time::SinceMonotonic(start_time);
                        JFUNIT_ASSERT(wait_time < Time::FromMilliSeconds(100));
                    }

                    {
                        EventPtr m = Factory::CreateEvent(false, true);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, false);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, true);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->Set());
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->Reset());
                        Time start_time = Time::NowMonotonic();
                        JFUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        Time wait_time = Time::SinceMonotonic(start_time);
                        JFUNIT_ASSERT(wait_time >= Time::FromMilliSeconds(90));
                        JFUNIT_ASSERT(wait_time <= Time::FromMilliSeconds(200));
                    }
                }
            };

            EventSuite::EventSuite()
                : jf::unittest::TestSuite("TC::MT::Tests::EventSuite")
            {
                add_test(new EventSetWaitTest);
                add_test(new EventTryTest);
                add_test(new EventWaitTimeTest);
            }

        }
    }
}

#endif
