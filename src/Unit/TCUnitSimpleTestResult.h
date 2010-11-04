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


#ifndef _TC_UNIT_SIMPLE_TEST_RESULT_H
#define _TC_UNIT_SIMPLE_TEST_RESULT_H

#include "TCUnitTestResult.h"

#include <iostream>

namespace TC 
{
   namespace Unit 
   {
      class TCUNIT_API SimpleTestResult : public TestResult
      {
      public:
         SimpleTestResult(std::ostream* ostream = NULL)
            : ostream_(ostream),
            num_tests_run_(0),
            num_success_(0),
            num_failure_(0),
            num_error_(0),
            num_assertion_(0),
            unclean_test_(NULL) {}

         bool ok() const { return num_failure_ + num_error_ == 0 && !unclean_test_; }
         bool unclean() const { return unclean_test_ != NULL; }
         int num_tests_run() const { return num_tests_run_; }
         int num_success() const { return num_success_; }
         int num_failure() const { return num_failure_; }
         int num_error() const { return num_error_; }

         virtual void enter_suite(const TestSuite*) {}
         virtual void leave_suite(const TestSuite*) {}
         virtual void enter_test(const TestCase*) { num_tests_run_++; }
         virtual void leave_test(const TestCase*) {}
         virtual void add_success(const TestCase*);
         virtual void add_failure(const TestCase*, const Failure&);
         virtual void add_error(const TestCase*, const std::string&);
         virtual void add_assertion(const TestCase*);
         virtual void unclean_alarm(const TestCase* t) { unclean_test_ = t; }

      private:
         std::ostream* ostream_;
         int num_tests_run_;
         int num_success_;
         int num_failure_;
         int num_error_;
         int num_assertion_;
         const TestCase* unclean_test_;
      };

   }
}

#endif
