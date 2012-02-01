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
// Copyright (C) 2003 - 2012 Thomas Goessler. All Rights Reserved. 
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

#ifndef _TC_STL_ALGORITHM_H_
#define _TC_STL_ALGORITHM_H_

#include "TCTypes.h"

#include <algorithm>

namespace tc
{
   /**
   * @addtogroup TC_BASE
   * @{
   */

   /**
   * @file 
   * This header file provides the definition of the namespace tc::stl_algorithm. 
   * 
   * @author Thomas Goessler 
   */

   /**
   * @brief Defines basic algorithm which can be used on stl containers
   */
   namespace stl_algorithm
   {
      /** @return true if c1 contains all elements from c2 */
      template <typename CONTAINER1, typename CONTAINER2>
      bool Contains(const CONTAINER1& c1, const CONTAINER2& c2)
      {
         return Contains( c1.begin(), c1.end(), c2.begin(), c2.end());
      }

      /** @return true if it_begin1 to it_end1 contains all elements from it_begin2 to  it_end2*/
      template <typename IT1, typename IT2>
      bool Contains(IT1 it_begin1, IT1 it_end1, IT2 it_begin2, IT2 it_end2)
      {
         for (IT2 it=it_begin2; it!=it_end2; ++it)
         {
            if (std::find(it_begin1, it_end1, *it) == it_end1)
            {
               return false;
            }
         }

         return true;
      }
   }

   /**
   * @}
   */
}

#endif // _TC_STL_ALGORITHM_H_
