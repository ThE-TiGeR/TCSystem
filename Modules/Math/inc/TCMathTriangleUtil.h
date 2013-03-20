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

#ifndef _TC_MATH_TRIANGLE_UTIL_H_
#define _TC_MATH_TRIANGLE_UTIL_H_

#include "TCMathCoordN.h"

namespace tc
{
   /**
   * @brief tc::math is intended to provide methods to perform common math operations.
   *
   * These include providing constants such as Pi; conversion 
   * from degrees to radians; vector operations such as dot and cross products
   * matrix determinant for 2x2 and 3x3 matrices; and random 
   * number generation.  Note that these are not all implemented yet.
   */
   namespace math
   {
      /**
      * @addtogroup TC_MATH
      * @{
      */

      template <typename T, uint32_t SIZE>
      double AreaOfTriangle(const CoordN<T, SIZE> &p1, const CoordN<T, SIZE> &p2, const CoordN<T, SIZE> &p3)
      {
         return .5 * ((p2-p1) ^ (p3-p1));
      }

      template <typename T, uint32_t SIZE>
      double EdgeLengthOfTriangle(const CoordN<T, SIZE> &p1, const CoordN<T, SIZE> &p2, const CoordN<T, SIZE> &p3)
      { 
         return (p2-p1).Length() + (p3-p1).Length() + (p2-p3).Length();
      }

      template <typename T, uint32_t SIZE>
      double EdgeLength2OfTriangle(const CoordN<T, SIZE> &p1, const CoordN<T, SIZE> &p2, const CoordN<T, SIZE> &p3)
      { 
         return (p2-p1).Length2() + (p3-p1).Length2() + (p2-p3).Length2(); 
      }

      template <typename T, uint32_t SIZE>
      inline const CoordN<T, SIZE> NormalOfTriangle(const CoordN<T, SIZE> &p1, const CoordN<T, SIZE> &p2, const CoordN<T, SIZE> &p3)
      { 
         return (p1-p2)^(p1-p3);
      }

      template <typename T, uint32_t SIZE>
      double RatioOfTriangle(const CoordN<T, SIZE> &p1, const CoordN<T, SIZE> &p2, const CoordN<T, SIZE> &p3)
      {
         return 6.92820323 * AreaOfTriangle(p1,p2,p3) / EdgeLength2OfTriangle(p1,p2,p3);
      }

      /**
      * @}
      */

   } // namespace math
} // namespace tc

#endif // _TC_MATH_TRIANGLE_UTIL_H_
