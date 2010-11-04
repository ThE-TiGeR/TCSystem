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
//  $Id$
//*******************************************************************************
#ifdef HAVE_UNIT_TESTS

#include "TCMTEventTest.h"

#include "TCMTEvent.h"
#include "TCMTFactory.h"
#include "TCTime.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace TC
{
    namespace MT
    {
        namespace Tests
        {

            class EventSetWaitTest : public Unit::TestCase
            {
            public:
                EventSetWaitTest()
                    :Unit::TestCase("TC::MT::Tests::EventSetWaitTest")
                {
                }

                virtual void Execute()
                {
                    {
                        EventPtr m = Factory::CreateEvent(false, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(false, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Wait());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Wait());
                        TCUNIT_ASSERT(m->Wait());
                    }
                }
            };

            class EventTryTest : public Unit::TestCase
            {
            public:
                EventTryTest()
                    :Unit::TestCase("TC::MT::Tests::EventTryTest")
                {
                }

                virtual void Execute()
                {
                    {
                        EventPtr m = Factory::CreateEvent(false, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(!m->Try());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(false, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(!m->Try());
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->Try());
                        TCUNIT_ASSERT(m->Try());
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, true);
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

            class EventWaitTimeTest : public Unit::TestCase
            {
            public:
                EventWaitTimeTest()
                    :Unit::TestCase("TC::MT::Tests::EventWaitTimeTest")
                {
                }

                virtual void Execute()
                {
                    {
                        EventPtr m = Factory::CreateEvent(false, false);
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
                        EventPtr m = Factory::CreateEvent(false, true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(!m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->Set());
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->TryWait(Time::FromMilliSeconds(100)));
                    }

                    {
                        EventPtr m = Factory::CreateEvent(true, true);
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
                : Unit::TestSuite("TC::MT::Tests::EventSuite")
            {
                AddTest(new EventSetWaitTest);
                AddTest(new EventTryTest);
                AddTest(new EventWaitTimeTest);
            }

        }
    }
}

#endif
