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
//  $Id: $
//*******************************************************************************

#include "TCUnitTestSuite.h"

#include "TCUnitTestResult.h"
#include "TCUnitCleanLiness.h"

#include <cassert>

namespace TC {
namespace Unit {

TestSuite::~TestSuite()
{
    for (TestsVector::const_iterator i = tests_.begin(); i != tests_.end(); ++i)
        delete *i;
}

void TestSuite::AddTest(Test* t)
{
    assert(t);
    tests_.push_back(t);
}

void TestSuite::InternalRun(TestResult* result, const CleanlinessCheck* cleanliness_check)
{
    // not that the test's run_internal() method catches all errors,
    // so it is safe to not wrap the call into try/catch.
    result->enter_suite(this);
    for (TestsVector::const_iterator i = tests_.begin(); i != tests_.end(); ++i) {
        (*i)->InternalRun(result, cleanliness_check);
        if (cleanliness_check && !cleanliness_check->EnvironmentIsClean())
            break;
    }
    result->leave_suite(this);
}

}
}
