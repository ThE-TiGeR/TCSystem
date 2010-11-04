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


#ifndef _TC_UNIT_FAILURE_H
#define _TC_UNIT_FAILURE_H

#include "TCUnitApi.h"

#include <string>

namespace TC 
{
   namespace Unit 
   {
      class TCUNIT_API Failure
      {
      public:
         Failure(const std::string& failed_condition, const std::string& filename, int line)
            : failed_condition_(failed_condition),
            filename_(filename),
            line_(line) {}

         const std::string& FailedCondition() const { return failed_condition_; }
         const std::string& Filename() const { return filename_; }
         int Line() const { return line_; }

      private:
         std::string failed_condition_;
         std::string filename_;
         int line_;
      };

   }
}

#endif
