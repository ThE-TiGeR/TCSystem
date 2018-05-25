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
// Copyright (C) 2003 - 2018 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCUnitTreeTestResult.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TC_UNIT_TREE_TEST_RESULT_H
#define _TC_UNIT_TREE_TEST_RESULT_H

#include "TCUnitTestResult.h"
#include "TCUnitTestSuite.h"
#include "TCUnitTestCase.h"

#include <stack>
#include <vector>

namespace tc 
{
   namespace unit 
   {
      class TCUNIT_API TreeTestResult : public TestResult
      {
      public:
         typedef SharedPtr<TreeTestResult> Ptr;
         typedef SharedPtr<const TreeTestResult> CPtr;

      public:
         TreeTestResult(std::ostream& ostream);

         virtual void EnterSuite(TestSuite::CPtr);
         virtual void LeaveSuite(TestSuite::CPtr);
         virtual void EnterTest(TestCase::CPtr);
         virtual void LeaveTest(TestCase::CPtr);
         virtual void AddSuccess(TestCase::CPtr);
         virtual void AddFailure(TestCase::CPtr, const Failure&);
         virtual void AddError(TestCase::CPtr, const std::string& message);
         virtual void AddAssertion(TestCase::CPtr);
         virtual void UncleanAlarm(TestCase::CPtr);

         void print_summary() const;
         bool ok() const { return num_success_ == num_tests_run_; }

      private:
         typedef std::stack<TestSuite::CPtr> SuiteStack;

         class Report 
         {
         public:
            Report(TestCase::CPtr c, const Failure& f)
               : testcase_(c),
               type_(Type::FAILURE),
               failure_(f) {}
            Report(TestCase::CPtr c, const std::string& m)
               : testcase_(c),
               type_(Type::ERROR),
               failure_(std::string(), std::string(), 0),
               message_(m) {}
            void print(std::ostream&) const;
         private:
            enum class Type 
            {
               FAILURE,
               ERROR
            };
            TestCase::CPtr testcase_;
            Type type_;
            Failure failure_;
            std::string message_;
         };
         typedef std::vector<Report> Reports;

      private:
         std::ostream& ostream_;

         SuiteStack suite_stack_;

         Failure cur_failure;
         const Failure* p_cur_failure;
         std::string cur_error;
         const std::string* p_cur_error;

         Reports reports_;

         int num_suites_entered_;
         int num_tests_run_;
         int num_success_;
         int num_failure_;
         int num_error_;
         int num_assertion_;

         // the one who made the environmental-cleanliness-check fail.
         TestCase::CPtr unclean_test_;

         TreeTestResult(const TreeTestResult&);
         TreeTestResult& operator=(const TreeTestResult&);
      };

   }
}

#endif
