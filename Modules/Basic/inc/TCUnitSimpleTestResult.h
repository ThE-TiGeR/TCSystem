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


#ifndef _TC_UNIT_SIMPLE_TEST_RESULT_H
#define _TC_UNIT_SIMPLE_TEST_RESULT_H

#include "TCUnitTestResult.h"
#include "TCUnitTestSuite.h"
#include "TCUnitTestCase.h"

#include <iostream>

namespace tc 
{
   namespace unit 
   {
      class TCUNIT_API SimpleTestResult : public TestResult
      {
      public:
         SimpleTestResult(std::ostream* ostream = 0)
            :m_ostream(ostream)
            ,m_num_tests_run(0)
            ,m_num_success(0)
            ,m_num_failure(0)
            ,m_num_error(0)
            ,m_num_assertion(0)
            ,m_unclean_test() {}

         bool ok() const { return m_num_failure + m_num_error == 0 && !m_unclean_test; }
         bool unclean() const { return m_unclean_test != 0; }
         int num_tests_run() const { return m_num_tests_run; }
         int num_success() const { return m_num_success; }
         int num_failure() const { return m_num_failure; }
         int num_error() const { return m_num_error; }

         virtual void EnterSuite(TestSuite::CPtr) {}
         virtual void LeaveSuite(TestSuite::CPtr) {}
         virtual void EnterTest(TestCase::CPtr) { m_num_tests_run++; }
         virtual void LeaveTest(TestCase::CPtr) {}
         virtual void AddSuccess(TestCase::CPtr);
         virtual void AddFailure(TestCase::CPtr, const Failure&);
         virtual void AddError(TestCase::CPtr, const std::string&);
         virtual void AddAssertion(TestCase::CPtr);
         virtual void UncleanAlarm(TestCase::CPtr t) { m_unclean_test = t; }

      private:
         std::ostream* m_ostream;
         int m_num_tests_run;
         int m_num_success;
         int m_num_failure;
         int m_num_error;
         int m_num_assertion;
         TestCase::CPtr m_unclean_test;
      };

   }
}

#endif
