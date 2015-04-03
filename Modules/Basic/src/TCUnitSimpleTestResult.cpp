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
//  $Id: TCUnitSimpleTestResult.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************


#include "TCUnitSimpleTestResult.h"

#include "TCUnitTestCase.h"

#include <iostream>

namespace tc 
{
   namespace unit 
   {
      void SimpleTestResult::AddSuccess(TestCase::CPtr)
      {
         m_num_success++;
      }

      void SimpleTestResult::AddFailure(TestCase::CPtr tc, const Failure& f)
      {
         m_num_failure++;
         if (m_ostream)
            std::cerr << "FAILURE: " << tc->Name() << ": " << f.FailedCondition()
            << " (" << f.Filename() << ':' << f.Line() << ')' << std::endl;
      }

      void SimpleTestResult::AddError(TestCase::CPtr tc, const std::string& message)
      {
         m_num_error++;
         if (m_ostream)
            std::cerr << "ERROR:   " << tc->Name() << ": " << message << std::endl;
      }

      void SimpleTestResult::AddAssertion(TestCase::CPtr)
      {
         m_num_assertion++;
      }

   }
}
