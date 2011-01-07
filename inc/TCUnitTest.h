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


#ifndef _TC_UNIT_TEST_H
#define _TC_UNIT_TEST_H

#include "TCUnitTestResult.h"
#include "TCUnitCleanLiness.h"
#include "TCUnitApi.h"
#include "TCNonCopyable.h"

#include <string>

namespace TC 
{
   namespace Unit 
   {
      class TCUNIT_API Test: protected NonCopyAble
      {
      public:
         typedef SharedPtr<Test> Ptr;
         typedef SharedPtr<const Test> CPtr;

      public:
         Test(const std::string& name)
            :m_name(name)
         {
         }
         virtual ~Test() {}

         const std::string& Name() const { return m_name; }

         virtual void InternalRun(TestResult::Ptr, CleanlinessCheck::CPtr) = 0;

      private:
         std::string m_name;
      };

   }
}

#endif
