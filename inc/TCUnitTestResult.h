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


#ifndef _TC_UNIT_TEST_RESULT_H
#define _TC_UNIT_TEST_RESULT_H

#include "TCUnitFailure.h"

namespace TC 
{
   namespace Unit 
   {
      class TestSuite;
      class TestCase;

      class TCUNIT_API TestResult
      {
      public:
         virtual ~TestResult() {}

         virtual void enter_suite(const TestSuite*) = 0;
         virtual void leave_suite(const TestSuite*) = 0;
         virtual void enter_test(const TestCase*) = 0;
         virtual void leave_test(const TestCase*) = 0;
         virtual void add_success(const TestCase*) = 0;
         virtual void add_failure(const TestCase*, const Failure&) = 0;
         virtual void add_error(const TestCase*, const std::string& message) = 0;
         virtual void add_assertion(const TestCase*) = 0;
         virtual void unclean_alarm(const TestCase*) = 0;
      };

   }
}

#endif