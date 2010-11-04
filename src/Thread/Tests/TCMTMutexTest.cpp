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

#include "TCMTMutexTest.h"

#include "TCMTFactory.h"
#include "TCMTMutex.h"
#include "TCTime.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace TC
{
    namespace MT
    {
        namespace Tests
        {
            class MutexLockUnlockTest : public Unit::TestCase
            {
            public:
                MutexLockUnlockTest()
                    :Unit::TestCase("TC::MT::Tests::MutexLockUnlockTest")
                {
                }

                virtual void Execute()
                {
                    {
                        MutexPtr m = Factory::CreateMutex(false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Lock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex(true);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->UnLock());
                        TCUNIT_ASSERT(m->Lock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex(false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryLock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex(false);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryLock(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->UnLock());
                    }
                }
            };

            class MutexSharedCreateTest : public Unit::TestCase
            {
            public:
                MutexSharedCreateTest()
                    :Unit::TestCase("TC::MT::Tests::MutexSharedCreateTest")
                {
                }

                virtual void Execute()
                {
                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_WHEN_NOT_EXISTS);
                        TCUNIT_ASSERT(m);
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_WHEN_EXISTS);
                        TCUNIT_ASSERT(!m);
                    }

                    {
                        MutexPtr m1 = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m1);
                        MutexPtr m2 = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_WHEN_EXISTS);
                        TCUNIT_ASSERT(m2);
                    }

                    {
                        MutexPtr m1 = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m1);
                        MutexPtr m2 = Factory::CreateMutex("MutexSharedCreateTest",
                            false, Factory::CRM_WHEN_NOT_EXISTS);
                        TCUNIT_ASSERT(!m2);
                    }
                }
            };

            class MutexSharedLockUnlockTest : public Unit::TestCase
            {
            public:
                MutexSharedLockUnlockTest()
                    :Unit::TestCase("TC::MT::Tests::MutexSharedLockUnlockTest")
                {
                }

                virtual void Execute()
                {
                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, Factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->Lock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedLockUnlockTest",
                            true, Factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->UnLock());
                        TCUNIT_ASSERT(m->Lock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, Factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryLock());
                        TCUNIT_ASSERT(m->UnLock());
                    }

                    {
                        MutexPtr m = Factory::CreateMutex("MutexSharedLockUnlockTest",
                            false, Factory::CRM_ALWAYS);
                        TCUNIT_ASSERT(m);
                        TCUNIT_ASSERT(m->TryLock(Time::FromMilliSeconds(100)));
                        TCUNIT_ASSERT(m->UnLock());
                    }
                }
            };

            MutexSuite::MutexSuite()
                : Unit::TestSuite("TC::MT::Tests::MutexSuite")
            {
                AddTest(new MutexLockUnlockTest);
                AddTest(new MutexSharedCreateTest);
                AddTest(new MutexSharedLockUnlockTest);
            }

        }
    }
}

#endif
