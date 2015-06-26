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
//  $Id: TCUnitTreeTestResult.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************


#include "TCUnitTreeTestResult.h"

#include "TCUnitTestCase.h"
#include "TCUnitTestSuite.h"

#include <iostream>
#include <cassert>

namespace {

class indent
{
public:
   indent(size_t num) : num_(num) {}
   size_t num() const { return num_; }
private:
   size_t num_;
};
static std::ostream& operator<<(std::ostream& o, const indent& i)
{
   size_t num = i.num();
   while (num--)
      o << "  ";
   return o;
}

}

namespace tc {
namespace unit {

   void TreeTestResult::Report::print(std::ostream& o) const
   {
      switch (type_)
      {
      case Type::FAILURE:
      {
         o << "  Failure: " << testcase_->Name() << ": "
            << failure_.FailedCondition()
            << " (" << failure_.Filename() << ':' << failure_.Line() << ')';
         break;
      }
      case Type::ERROR:
      {
         o << "  Error:   " << testcase_->Name() << ": " << message_;
         break;
      }
      }
      o << '\n';
   }

   TreeTestResult::TreeTestResult(std::ostream& ostream)
      : ostream_(ostream),
      cur_failure(std::string(), std::string(), 0),
      p_cur_failure(0),
      p_cur_error(0),
      num_suites_entered_(0),
      num_tests_run_(0),
      num_success_(0),
      num_failure_(0),
      num_error_(0),
      num_assertion_(0),
      unclean_test_() {}

   void TreeTestResult::EnterSuite(TestSuite::CPtr s)
   {
      num_suites_entered_++;
      ostream_ << indent(suite_stack_.size()) << "+ " << s->Name() << '\n';
      suite_stack_.push(s);
   }

   void TreeTestResult::LeaveSuite(TestSuite::CPtr /*s*/)
   {
      assert(suite_stack_.size() != 0);
      suite_stack_.pop();
   }

   void TreeTestResult::EnterTest(TestCase::CPtr c)
   {
      num_tests_run_++;
      ostream_ << indent(suite_stack_.size()) << "- " << c->Name() << "...";
   }

   void TreeTestResult::LeaveTest(TestCase::CPtr c)
   {
      if (p_cur_error) {
         num_error_++;
         reports_.push_back(Report(c, *p_cur_error));
         ostream_ << "error";
      }
      else if (p_cur_failure) {
         num_failure_++;
         reports_.push_back(Report(c, *p_cur_failure));
         ostream_ << "failed";
      }
      else {
         num_success_++;
         ostream_ << "ok";
      }

      ostream_ << '\n';
      p_cur_failure = 0;
      p_cur_error = 0;
   }

   void TreeTestResult::AddSuccess(TestCase::CPtr) {}

   void TreeTestResult::AddFailure(TestCase::CPtr, const Failure& f)
   {
      cur_failure = f;
      p_cur_failure = &cur_failure;
   }

   void TreeTestResult::AddError(TestCase::CPtr, const std::string& message)
   {
      cur_error = message;
      p_cur_error = &cur_error;
   }

   void TreeTestResult::UncleanAlarm(TestCase::CPtr t)
   {
      unclean_test_ = t;
   }


   void TreeTestResult::print_summary() const
   {
      ostream_ << "------------------------\n";

      if (unclean_test_) {
         ostream_ << "ALARM: environment has not been cleared\n";
         ostream_ << "       " << unclean_test_->Name() << '\n';
         ostream_ << "------------------------\n";
      }

      ostream_ << "#Success:          " << num_success_ << '\n';
      ostream_ << "#Failures:         " << num_failure_ << '\n';
      ostream_ << "#Errors:           " << num_error_ << '\n';
      ostream_ << "#Tests run:        " << num_tests_run_ << '\n';
      ostream_ << "#Suites entered:   " << num_suites_entered_ << '\n';
      ostream_ << "#Asserts entered:  " << num_assertion_ << '\n';
      ostream_ << "------------------------\n";

      if (!ok()) {
         ostream_ << "Details follow ...\n";
         ostream_ << "------------------------\n";
         for (Reports::const_iterator i = reports_.begin();
            i != reports_.end();
            ++i)
            i->print(ostream_);
      }
   }

   void TreeTestResult::AddAssertion(TestCase::CPtr)
   {
      num_assertion_++;
   }

}
}
