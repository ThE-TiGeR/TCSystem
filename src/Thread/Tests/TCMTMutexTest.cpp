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

#include "TCMTMutexTest.h"

#include "TCMTFactory.h"
#include "TCMTMutex.h"
#include "TCTime.h"

#include <jf/unittest/test_case.h>

#include "TCNewEnable.h"

namespace TC
{
    namespace MT
    {
        namespace Tests
        {
            class MutexLockUnlockTest : public jf::unittest::TestCase
            {
            public:
                MutexLockUnlockTest()
                    :jf::unittest::TestCase("TC::MT::Tests::MutexLockUnlockTest")
                {
                }

                virtual void run()
                {
                    {
                        MutexPtr m = Factory::CreateMutex(false);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Lock());
                        JFUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex(true);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->UnLock());
                        JFUNIT_ASSERT(m->Lock());
                        JFUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex(false);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->TryLock());
                        JFUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex(false);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->TryLock(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->UnLock());
                    }
                }
            };

            class MutexSharedCreateTest : public jf::unittest::TestCase
            {
            public:
                MutexSharedCreateTest()
                    :jf::unittest::TestCase("TC::MT::Tests::MutexSharedCreateTest")
                {
                }

                virtual void run()
                {
                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_WHEN_NOT_EXISTS);
                        JFUNIT_ASSERT(m);
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_WHEN_EXISTS);
                        JFUNIT_ASSERT(!m);
                    }

                    {
                        MutexPtr m1 = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m1);
                        MutexPtr m2 = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_WHEN_EXISTS);
                        JFUNIT_ASSERT(m2);
                    }

                    {
                        MutexPtr m1 = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m1);
                        MutexPtr m2 = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_WHEN_NOT_EXISTS);
                        JFUNIT_ASSERT(!m2);
                    }
                }
            };

            class MutexSharedLockUnlockTest : public jf::unittest::TestCase
            {
            public:
                MutexSharedLockUnlockTest()
                    :jf::unittest::TestCase("TC::MT::Tests::MutexSharedLockUnlockTest")
                {
                }

                virtual void run()
                {
                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->Lock());
                        JFUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedLockUnlockTest",
                            true, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->UnLock());
                        JFUNIT_ASSERT(m->Lock());
                        JFUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->TryLock());
                        JFUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, Factory::CRM_ALWAYS);
                        JFUNIT_ASSERT(m);
                        JFUNIT_ASSERT(m->TryLock(Time::FromMilliSeconds(100)));
                        JFUNIT_ASSERT(m->UnLock());
                    }
                }
            };

            MutexSuite::MutexSuite()
                : jf::unittest::TestSuite("TC::MT::Tests::MutexSuite")
            {
                add_test(new MutexLockUnlockTest);
                add_test(new MutexSharedCreateTest);
                add_test(new MutexSharedLockUnlockTest);
            }

        }
    }
}

#endif
