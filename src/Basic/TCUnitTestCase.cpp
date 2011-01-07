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


#include "TCUnitTestCase.h"

#include "TCUnitTestResult.h"
#include "TCUnitFailure.h"
#include "TCUnitCleanLiness.h"

#include <cassert>
#include <cstdio>

namespace 
{
   class FailureException : public std::exception
   {
   public:
      FailureException(const TC::Unit::Failure& failure) : failure_(failure) {}
      const TC::Unit::Failure& failure() const { return failure_; }
      /** Inherited from std::exception. Overloaded fatally since nobody
      should get a chance to call it. */
      virtual const char* what() { assert(false); return 0;}

   private:
      TC::Unit::Failure failure_;
   };

   static void add_failure_description(std::string& msg, const TC::Unit::Failure& f)
   {
      msg += f.FailedCondition();
      msg += " (";
      msg += f.Filename();
      msg += ':';
      char tmp[16];
      std::sprintf(tmp, "%d", f.Line());
      msg += tmp;
      msg += ')';
   }

}

namespace TC 
{
   namespace Unit 
   {

      void TestCase::InternalRun(TestResult::Ptr result, CleanlinessCheck::CPtr cleanliness_check)
      {
         m_result = result;
         result->EnterTest(Ptr(this, NoDelete()));

         bool setup_ok = false;
         try {
            this->Setup();
            setup_ok = true;
         }
         catch (const FailureException& e) {
            std::string msg("setup: ");
            add_failure_description(msg, e.failure());
            result->AddError(Ptr(this, NoDelete()), msg);
         }
         catch (const std::exception& e) {
            std::string msg("setup: ");
            msg += e.what();
            result->AddError(Ptr(this, NoDelete()), msg);
         }
         catch (...) {
            result->AddError(Ptr(this, NoDelete()), "setup: \"...\" caught");
         }

         // only if setup went ok go on to execute the test code and
         // teardown
         if (setup_ok) {
            try {
               this->Execute();
               result->AddSuccess(Ptr(this, NoDelete()));
            }
            catch (const FailureException& f) {
               result->AddFailure(Ptr(this, NoDelete()), f.failure());
            }
            catch (const std::exception& e) {
               result->AddError(Ptr(this, NoDelete()), e.what());
            }
            catch (...) {
               result->AddError(Ptr(this, NoDelete()), "\"...\" caught");
            }

            try {
               this->Teardown();
            }
            catch (const FailureException& e) {
               std::string msg("teardown: ");
               add_failure_description(msg, e.failure());
               result->AddError(Ptr(this, NoDelete()), msg);
            }
            catch (const std::exception& e) {
               std::string msg("teardown: ");
               msg += e.what();
               result->AddError(Ptr(this, NoDelete()), msg);
            }
            catch (...) {
               result->AddError(Ptr(this, NoDelete()), "teardown: \"...\" caught");
            }
         }

         result->LeaveTest(Ptr(this, NoDelete()));

         if (cleanliness_check && !cleanliness_check->EnvironmentIsClean())
            result->UncleanAlarm(Ptr(this, NoDelete()));

         m_result = TestResult::Ptr();
      }

      void TestCase::do_cond_fail(
         bool condition,
         const std::string& condition_str,
         const std::string& filename,
         int line)
      {
         if (m_result)
            m_result->AddAssertion(Ptr(this, NoDelete()));

         if (!condition)
            throw FailureException(Failure(condition_str, filename, line));
      }

   }
}

