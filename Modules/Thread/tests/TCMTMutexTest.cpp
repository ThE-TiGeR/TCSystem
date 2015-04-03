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
//  $Id: TCMTMutexTest.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifdef HAVE_UNIT_TESTS

#include "TCMTMutexTest.h"

#include "TCMTFactory.h"
#include "TCMTMutex.h"
#include "TCTime.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
    namespace multi_threading
    {
        namespace tests
        {
            class MutexLockUnlockTest : public unit::TestCase
            {
            public:
                MutexLockUnlockTest()
                    :unit::TestCase("tc::multi_threading::tests::MutexLockUnlockTest")
                {
                }

                virtual void Execute()
                {
                    {
                        MutexPtr m = factory::CreateMutex(false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Lock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = factory::CreateMutex(true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->UnLock());
                        TCUNIT_ASSERT(m->Lock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = factory::CreateMutex(false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryLock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = factory::CreateMutex(false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryLock(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->UnLock());
                    }
                }
            };

            class MutexSharedCreateTest : public unit::TestCase
            {
            public:
                MutexSharedCreateTest()
                    :unit::TestCase("tc::multi_threading::tests::MutexSharedCreateTest")
                {
                }

                virtual void Execute()
                {
                    {
                        MutexPtr m = factory::CreateMutex("MutexSharedCreateTest",
                            false, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                    }

                    {
                        MutexPtr m = factory::CreateMutex("MutexSharedCreateTest",
                            false, factory::CRM_WHEN_NOT_EXISTS);
                        TCUNIT_ASSERT(m);
                    }

                    {
                        MutexPtr m = factory::CreateMutex("MutexSharedCreateTest",
                            false, factory::CRM_WHEN_EXISTS);
                        TCUNIT_ASSERT(!m);
                    }

                    {
                        MutexPtr m1 = factory::CreateMutex("MutexSharedCreateTest",
                            false, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m1);
                        MutexPtr m2 = factory::CreateMutex("MutexSharedCreateTest",
                            false, factory::CRM_WHEN_EXISTS);
                        TCUNIT_ASSERT(m2);
                    }

                    {
                        MutexPtr m1 = factory::CreateMutex("MutexSharedCreateTest",
                            false, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m1);
                        MutexPtr m2 = factory::CreateMutex("MutexSharedCreateTest",
                            false, factory::CRM_WHEN_NOT_EXISTS);
                        TCUNIT_ASSERT(!m2);
                    }
                }
            };

            class MutexSharedLockUnlockTest : public unit::TestCase
            {
            public:
                MutexSharedLockUnlockTest()
                    :unit::TestCase("tc::multi_threading::tests::MutexSharedLockUnlockTest")
                {
                }

                virtual void Execute()
                {
                    {
                        MutexPtr m = factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Lock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = factory::CreateMutex("MutexSharedLockUnlockTest",
                            true, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->UnLock());
                        TCUNIT_ASSERT(m->Lock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryLock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryLock(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->UnLock());
                    }
                }
            };

            MutexSuite::MutexSuite()
                : unit::TestSuite("tc::multi_threading::tests::MutexSuite")
            {
                AddTest(unit::Test::Ptr(new MutexLockUnlockTest));
                AddTest(unit::Test::Ptr(new MutexSharedCreateTest));
                AddTest(unit::Test::Ptr(new MutexSharedLockUnlockTest));
            }

        }
    }
}

#endif
