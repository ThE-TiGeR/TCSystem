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

#ifndef _TC_MATH_UTIL_H_
#define _TC_MATH_UTIL_H_

#include "TCTypes.h"
#include "TCMathApi.h"

#include <cmath>
#include <limits>

#undef PI

namespace TC
{
/**
 * @brief TC::Math is intended to provide methods to perform common math operations.
 *
 * These include providing constants such as Pi; conversion 
 * from degrees to radians; vector operations such as dot and cross products
 * matrix determinant for 2x2 and 3x3 matrices; and random 
 * number generation.  Note that these are not all implemented yet.
 */
namespace Math
{
   /**
    * @addtogroup TC_MATH
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of Math utilities in the namespace TC::Math
    * @author Thomas Goessler
    */

   /** The constant value of PI */
   const double PI = 3.1415926535897932385E0;
   /** The constant value one Megabyte in bytes */
   const double MEGA_BYTES = 1024. * 1024.;

   /** @return the absolute value of the given value */
   template <class T>
   inline T Abs(const T& x)
   {
      return (x < 0) ? -x : x;
   }

   /** integer version of pow(..) */
   template<class T>
   inline T Pow(T x, uint32 y)
   {
      if (y == 0)
      {
         return 1;
      }

      T ret = x;
      while (--y)
      {
         ret*=x;
      }
      return ret;
   }

   /**
   * @brief aligns a given number to 32
   * @param size - value that should be aligned
   * @return size aligned to 32 bytes
   */
   inline uint32 Align32(uint32 size)
   {
      return ((size + 31) & (~31));
   }

   /** @brief Compare two double values if equal by checking their difference */
   inline bool Compare(double val1, double val2)
   {
      return Abs(val1 - val2) < std::numeric_limits<double>::epsilon();
   }

   /**
   * @brief aligns a given number to 16
   * @param size - value that should be aligned
   * @return size aligned to 16 bytes
   */
   inline uint32 Align16(uint32 size)
   {
      return ((size + 15) & (~15));
   }

   /**
    * Function converts angle in degrees to angle in radians
    * @param val angle value in degrees
    * @return angle value in radians
    */
   template <class T>
   inline double Deg2Rad(T val)
   {
      return static_cast<double>(val) * PI / 180.0;
   }

   /**
    * Function converts angle in radians to angle in degrees
    * @param val angle value in radians
    * @return angle value in degrees
    */
   template <class T>
   inline double Rad2Deg(T val)
   {
      return static_cast<double>(val) * 180.0 / PI;
   }

   /**
    * Function converts a value in the range between minValue and maxValue to 
    * a value between 0 and one
    * @param minValue The value defines the minuimum when the value is 0
    * @param maxValue The value defines the maximum when the value is 1
    * @param value The value which should be converted to a value between 0 and one
    * @return The converted value between 0 and 0
    */
   template <class T>
   inline T Normalize(T minValue, T maxValue, T value)
   {
      // check if min and max was exchanged
      T startSize   = (T)0.;
      T multiplier  = (T)1.;
      if (maxValue < minValue)
      {
         Swap(minValue, maxValue);
         startSize  = (T)1.;
         multiplier = (T)-1.;
      }

      value = Max(minValue, value);
      value = Min(maxValue, value);

      if (maxValue - minValue)
         return (T)(startSize + multiplier * ((value -  minValue) / (maxValue - minValue)));
      else
         return (T)1.;
   }

   /**
    * This function calculates sin(x)/x in a safe way, i.e. without overflows near 0.
    * @param  x  Trivial.
    * @return sin(x)/x.
    */
   template <class T>
   inline double SinXOverX(T x)
   {
      if (1.0 + x*x > 1.0)
         return std::sin(x)/x;
      else
         return 1;
   }

   /**
    * Round any kind of value to the first significant value
    *
    * e.g. 53465  > 50000
    *      0.0875 > 0.08
    *      -234   > -200
    *
    * @param val The value which to round
    * @return the rounded value
    */
   template <class T>
   inline T RoundToFirstSignificantValue(T val)
   {
      // on 0 we do nothing
      if (val == (T)0) return (T)0;

      double logRange   = std::log10(Abs(val));
      sint32 logRangeFloor = (sint32)std::floor(logRange);
      double normalized = (sint32)std::pow(10.0, logRange - logRangeFloor);

      sint32 i;
      if (logRangeFloor > 0)
         for (i=0; i<logRangeFloor; i++) normalized *= 10.;
      else {
         sint32 absLogRangeFloor = Abs(logRangeFloor);
         for (i=0; i<absLogRangeFloor; i++) normalized *= 0.1;
      }

      if (val < (T)0) normalized = -normalized;

      return (T)normalized;
   }

   /** compare double or float value with its specified system epsilon */
   template <class T>
   bool Compare(const T& val1, const T& val2)
   {
      return Abs(val1 - val2) > std::numeric_limits<T>::epsilon();
   }

   /**
    * function returns a true value if value is
    * "not-a-number" (NaN), and false otherwise.
    */
   TCMATH_API bool IsNaN(double val);

   /**
    * @return true value if value is between -INF and +INF
    *         false other wise
    */
   TCMATH_API bool IsFinite(double val);

   /**
    * @return true value if value is infinite
    *         false other wise
    */
   TCMATH_API bool IsInf(double val);

   /**
    * @}
    */

} // namespace Math
} // namespace TC

#endif // _TC_MATH_UTIL_H_
