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
//  $Id: TCUnitTestSuite.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************


#ifndef _TC_UNIT_TEST_SUITE_H
#define _TC_UNIT_TEST_SUITE_H

#include "TCUnitTest.h"
#include "TCSharedPtr.h"

#include <vector>

namespace tc 
{
   namespace unit 
   {

      class TCUNIT_API TestSuite : public Test
      {
      public:
         typedef SharedPtr<TestSuite> Ptr;
         typedef SharedPtr<const TestSuite> CPtr;
         typedef std::vector<Test::Ptr> TestsVector;

      public:
         TestSuite(const std::string& name) : Test(name) {}
         virtual ~TestSuite();

         void AddTest(Test::Ptr);
         const TestsVector& Tests() const { return m_tests; }

      public:
         virtual void InternalRun(TestResult::Ptr, CleanlinessCheck::CPtr);

      private:
         TestsVector m_tests;
      };

   }
}

#endif
