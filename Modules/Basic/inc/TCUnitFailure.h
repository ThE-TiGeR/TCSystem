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
//  $Id$
//*******************************************************************************


#ifndef _TC_UNIT_FAILURE_H
#define _TC_UNIT_FAILURE_H

#include "TCUnitApi.h"
#include "TCSharedPtr.h"

#include <string>

namespace tc 
{
   namespace unit 
   {
      class TCUNIT_API Failure
      {
      public:
         typedef SharedPtr<Failure> Ptr;
         typedef SharedPtr<const Failure> CPtr;

      public:
         Failure(const std::string& failed_condition, const std::string& filename, int line)
            :m_failed_condition(failed_condition)
            ,m_filename(filename)
            ,m_line(line) 
         {
         }

         const std::string& FailedCondition() const { return m_failed_condition; }
         const std::string& Filename() const { return m_filename; }
         int Line() const { return m_line; }

      private:
         std::string m_failed_condition;
         std::string m_filename;
         int m_line;
      };

   }
}

#endif
