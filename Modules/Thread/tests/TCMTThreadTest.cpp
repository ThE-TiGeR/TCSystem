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

#include "TCMTThreadTest.h"

#include "TCMTEvent.h"
#include "TCMTFactory.h"
#include "TCMTThread.h"
#include "TCUtil.h"
#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
    namespace multi_threading
    {
        namespace tests
        {

            class ThreadTest : public unit::TestCase, 
                public ThreadObject
            {
            public:
                ThreadTest()
                    :unit::TestCase("tc::multi_threading::tests::ThreadTest"),
                    m_exeption(0)
                {
                }

                ~ThreadTest()
                {
                    util::SafeRelease(m_exeption);
                }

                virtual void Execute()
                {
                    m_thread_start_event = factory::CreateEvent();
                    m_thread_started_event = factory::CreateEvent();
                    m_thread_handle = factory::CreateThread("tc::multi_threading::tests::ThreadTest");

                    TCUNIT_ASSERT(m_thread_handle);
                    TCUNIT_ASSERT(m_thread_handle->Start(ThreadObjectPtr(this, NoDelete())));

//                     uint32 thread_id = m_thread_handle->GetID();
//                     TCUNIT_ASSERT(m_thread_handle == GetThreadHandle(thread_id));

                    TCUNIT_ASSERT(m_thread_start_event->Set());
                    TCUNIT_ASSERT(m_thread_started_event->Wait());

                    TCUNIT_ASSERT(m_thread_handle->Join());

                    m_thread_handle = ThreadPtr();
                    m_thread_start_event = EventPtr();
                    m_thread_started_event = EventPtr();

                    if (m_exeption)
                    {
                        throw(*m_exeption);
                    }
             }

            private:
                bool Run()
                {
                    try
                    {
                        m_thread_start_event->Wait();
                        m_thread_started_event->Set();

                        TCUNIT_ASSERT(m_thread_handle == factory::GetCurrentThread());
                    }

                    catch (const unit::Failure& ex)
                    {
                        m_exeption = new unit::Failure(ex);
                    }

                    return true;
                }

                ThreadPtr m_thread_handle;
                EventPtr m_thread_start_event;
                EventPtr m_thread_started_event;
                unit::Failure* m_exeption;
          };


            class ThreadMessageTest : public unit::TestCase
            {
            public:
                ThreadMessageTest()
                    :unit::TestCase("tc::multi_threading::tests::ThreadMessageTest"),
                    m_exeption(0)
                {
                }

                ~ThreadMessageTest()
                {
                    delete m_exeption;
                }


                enum MessageIds
                {
                    TEST_MESSAGE_ID1 = Message::MSG_ID_USER_START,
                    TEST_MESSAGE_ID2,
                    TEST_MESSAGE_ID3,
                    TEST_MESSAGE_ID4
                };

                virtual void Execute()
                {
                    /*
                    ThreadPtr h = factory::CreateThread("tc::multi_threading::tests::ThreadMessageTest");
                    TCUNIT_ASSERT(h);
                    TCUNIT_ASSERT(h->Start(ThreadObjectPtr(this, NoDelete())));

                    Message::Post(h, TEST_MESSAGE_ID1, 0);
                    Message::Post(h, TEST_MESSAGE_ID2, (void*)0x1);
                    Message::Post(h, TEST_MESSAGE_ID3, (void*)0x1);

                    System::Sleep(1000);

                    Message::Post(h, TEST_MESSAGE_ID1, 0);
                    System::Sleep(1000);
                    Message::Post(h, TEST_MESSAGE_ID3, (void*)0x1);
                    System::Sleep(1000);

                    Message::Post(h, TEST_MESSAGE_ID1, 0);
                    Message::Post(h, TEST_MESSAGE_ID2, 0);
                    Message::Post(h, TEST_MESSAGE_ID3, 0);
                    Message::Post(h, TEST_MESSAGE_ID4, 0);

                    h->Join();
                    ReleaseThreadHandle(h);

                    if (m_exeption)
                    {
                        throw(*m_exeption);
                    }
                    TCUNIT_ASSERT(false);
                    */
                }

            private:
                bool Run()
                {
//                     try
//                     {
//                         void* msg = 0x0;
//                         uint32 msg_id = 0;
//                         const uint32 all_ids[] = {TEST_MESSAGE_ID1, TEST_MESSAGE_ID2, TEST_MESSAGE_ID3, TEST_MESSAGE_ID4};
//                         uint32 num_all_ids = util::ArraySize(all_ids);
// 
//                         {
//                             TCUNIT_ASSERT(Message::Get(TEST_MESSAGE_ID1, &msg) == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg == 0x0);
// 
//                             TCUNIT_ASSERT(Message::Get(TEST_MESSAGE_ID2, &msg) == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg == (void*)0x1);
// 
//                             TCUNIT_ASSERT(Message::Get(num_all_ids, all_ids, msg_id, &msg)
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg == (void*)0x1);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID3);
//                         }
// 
//                         {
//                             TCUNIT_ASSERT(Message::Get(TEST_MESSAGE_ID1, &msg, Time::FromMilliSeconds(200)) == Message::MSG_RECEIVE_FAILED);
//                             TCUNIT_ASSERT(Message::Get(num_all_ids, all_ids, msg_id, &msg, Time::FromMilliSeconds(200))
//                                 == Message::MSG_RECEIVE_FAILED);
// 
//                             TCUNIT_ASSERT(Message::Get(TEST_MESSAGE_ID1, &msg, Time::FromMilliSeconds(2000)) == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(Message::Get(num_all_ids, all_ids, msg_id, &msg, Time::FromMilliSeconds(2000))
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg == (void*)0x1);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID3);
//                         }
// 
//                         {
//                             TCUNIT_ASSERT(Message::TryGet(TEST_MESSAGE_ID1, &msg, true) == Message::MSG_RECEIVE_FAILED);
//                             TCUNIT_ASSERT(Message::TryGet(num_all_ids, all_ids, msg_id, &msg, true)
//                                 == Message::MSG_RECEIVE_FAILED);
//                             System::Sleep(2000);
// 
//                             TCUNIT_ASSERT(Message::TryGet(TEST_MESSAGE_ID1, &msg, true) == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(Message::TryGet(TEST_MESSAGE_ID2, &msg, false) == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(Message::TryGet(TEST_MESSAGE_ID2, &msg, false) == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(Message::TryGet(TEST_MESSAGE_ID2, &msg, false) == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(Message::TryGet(TEST_MESSAGE_ID2, &msg, false) == Message::MSG_RECEIVED);
// 
//                             TCUNIT_ASSERT(Message::TryGet(num_all_ids, all_ids, msg_id, &msg, true)
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID2);
//                             TCUNIT_ASSERT(Message::TryGet(num_all_ids, all_ids, msg_id, &msg, false)
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID3);
//                             TCUNIT_ASSERT(Message::TryGet(num_all_ids, all_ids, msg_id, &msg, false)
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID3);
//                             TCUNIT_ASSERT(Message::TryGet(num_all_ids, all_ids, msg_id, &msg, false)
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID3);
//                             TCUNIT_ASSERT(Message::TryGet(num_all_ids, all_ids, msg_id, &msg, false)
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID3);
//                             TCUNIT_ASSERT(Message::TryGet(num_all_ids, all_ids, msg_id, &msg, true)
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID3);
//                             TCUNIT_ASSERT(Message::TryGet(num_all_ids, all_ids, msg_id, &msg, true)
//                                 == Message::MSG_RECEIVED);
//                             TCUNIT_ASSERT(msg_id == TEST_MESSAGE_ID4);
//                         }
//                     }
// 
//                     catch (const unit::Failure& ex)
//                     {
//                         m_exeption = new unit::Failure(ex);
//                     }
// 
//                     return 0;
                   return false;
                }

                unit::Failure* m_exeption;
            };

            ThreadSuite::ThreadSuite()
                : unit::TestSuite("tc::multi_threading::tests::ThreadSuite")
            {
                AddTest(unit::Test::Ptr(new ThreadTest));
                AddTest(unit::Test::Ptr(new ThreadMessageTest));
            }

        }
    }
}

#endif
