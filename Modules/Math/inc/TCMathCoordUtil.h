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

#ifndef _TC_COORD_UTIL_H_
#define _TC_COORD_UTIL_H_

#include "TCMathCoordN.h"

namespace tc
{
   namespace math
   {
      /**
      * @addtogroup TC_MATH
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of Coordinate utilities in the namespace tc::math
      * @author Thomas Goessler
      */

      /**
      * Computes the distance of two 3-coordinates
      * @param a     the 1st 3D coordinate
      * @param b     the 2nd 3D coordinate
      * @return The distance between the two coordinates
      */
      template <class COORD_TYPE>
      inline double Distance(const COORD_TYPE& a, const COORD_TYPE& b)
      {
         COORD_TYPE c = a-b;
         return c.Length();
      }

      /**
      * Computes the square distance of two 3-coordinates
      * @param a     the 1st 3D coordinate
      * @param b     the 2nd 3D coordinate
      * @return The square distance between the two coordinates
      */
      template <class COORD_TYPE>
      inline double Distance2(const COORD_TYPE &a, const COORD_TYPE &b)
      {
         COORD_TYPE c = a-b;
         return c.Length2();
      }

      /**
      * Computes the dot product of coordinates.
      * @param a     the 1st 3D coordinate
      * @param b     the 2nd 3D coordinate
      * @return the Dot product of the 2 coordinates
      */
      template <class COORD_TYPE>
      inline typename COORD_TYPE::DataType DotProduct(const COORD_TYPE& a, const COORD_TYPE& b)
      {
         typename COORD_TYPE::DataType val(0);
         typename COORD_TYPE::ConstIterator it1=a.Begin();
         typename COORD_TYPE::ConstIterator it2=b.Begin();

         for (; it1!=a.End(); ++it1, ++it2)
         {
            val += (*it1) * (*it2);
         }

         return val;
      }

      /**
      * Computes the square dot product of two 3-coordinates
      * @param a     the 1st 3D coordinate
      * @param b     the 2nd 3D coordinate
      * @return the square Dot product of the 2 coordinates
      */
      template <class COORD_TYPE>
      inline typename COORD_TYPE::DataType DotProduct2(const COORD_TYPE& a, const COORD_TYPE& b)
      {
         typename COORD_TYPE::DataType v = DotProduct(a, b);
         return v * v;
      }

      /**
      * Computes the the normalized vector of the coord and returns it. 
      * The cross product of a and b, written a x b, is defined by: 
      * @param  coord The coord for which to get the normalized one
      * @return The normalize coord
      */
      template <class COORD_TYPE>
      inline COORD_TYPE Normalize(const COORD_TYPE& coord)
      {
         COORD_TYPE new_coord(coord);
         new_coord.Normalize();
         return new_coord;
      }

      /**
      * Computes the cross product of two 3-coordinates. 
      * The cross product of a and b, written a x b, is defined by: 
      * @param  a the 1st vector
      * @param  b the 2nd vector
      * @return the cross product of the two coordinates
      */
      template <class COORD_TYPE>
      inline COORD_TYPE CrossProduct(const COORD_TYPE& a, const COORD_TYPE& b)
      {
         return a ^ b;
      }

      /**
       * @return the maximum of all components of the coordinate
       */
      template <class COORD_TYPE>
      typename COORD_TYPE::DataType MaxValue(const COORD_TYPE& a)
      {
         return *std::max_element(a.Begin(), a.End());
      }

      template <class COORD_TYPE>
      inline COORD_TYPE Max(const COORD_TYPE& a, const COORD_TYPE& b)
      {
         COORD_TYPE coord;
         typename COORD_TYPE::Iterator it=coord.Begin();
         typename COORD_TYPE::ConstIterator it1=a.Begin();
         typename COORD_TYPE::ConstIterator it2=b.Begin();

         for (; it!=coord.End(); ++it, ++it1, ++it2)
         {
            *it = util::Max(*it1, *it2);
         }

         return coord;
      }

      template <class COORD_TYPE>
      inline COORD_TYPE Min(const COORD_TYPE& a, const COORD_TYPE& b)
      {
         COORD_TYPE coord;
         typename COORD_TYPE::Iterator it=coord.Begin();
         typename COORD_TYPE::ConstIterator it1=a.Begin();
         typename COORD_TYPE::ConstIterator it2=b.Begin();

         for (; it!=coord.End(); ++it, ++it1, ++it2)
         {
            *it = util::Min(*it1, *it2);
         }

         return coord;
      }

      /**
      * Calculates the normal vector of the plane defined by the points
      * @param numPoints Number of points defining the plane
      * @param coords The coordinates of the points defining the plane
      * @return The not normalized normal vector of the plane
      */
      template <class T>
      inline CoordN<T,3> Normalvector(uint32_t numPoints, const CoordN<T,3>* coords)
      {
         CoordN<T,3> out;

         for (uint32_t pos=0; pos<numPoints; pos++)
         {
            uint32_t index = (pos+1) % numPoints;
            out[0] += (coords[pos][1] - coords[index][1]) * (coords[pos][2] + coords[index][2]);
            out[1] += (coords[pos][2] - coords[index][2]) * (coords[pos][0] + coords[index][0]);
            out[2] += (coords[pos][0] - coords[index][0]) * (coords[pos][1] + coords[index][1]);
         }
         return out;
      }

      /**
      * Computes the angle between two 3d vectors
      * @param a     the 1st 3D vector
      * @param b     the 2nd 3D vector
      * @return the angle between the two vectors
      */
      template <class COORD_TYPE>
      inline double Angle(const COORD_TYPE& a, const COORD_TYPE& b)
      {
         if (DotProduct(a, b) < 0.0)
         {
            double sinValue = (-b-a).Length()/2;
            if (sinValue > 1.0) sinValue = 1.0;
            if (sinValue < -1.0) sinValue = -1.0;
            return PI - 2*asin(sinValue);
         }
         else
         {
            double sinValue = (b-a).Length()/2;
            if (sinValue > 1.0) sinValue = 1.0;
            if (sinValue < -1.0) sinValue = -1.0;
            return 2*asin(sinValue);
         }
      }

      template <class T>
      inline bool IsPointInCircle(const CoordN<T,2> &Point,
         const CoordN<T,2> &center,
         double radius)
      {   
         double distance = sqrt((double)((Point[0]-center[0])*(Point[0]-center[0]) 
            + (Point[1]-center[1])*(Point[1]-center[1])));

         return distance <= radius;
      }

      template <class T>
      inline bool CheckInsidePolygon(const CoordN<T,2> &point,
         uint32_t num_points, const CoordN<T,2> *polygon)
      {
         uint32_t crossings = 0;
         //loop over all edges of polygon
         for(uint32_t num1=0; num1<num_points; num1++)
         {
            uint32_t num2 = (num1 - 1 + num_points) % num_points;
            CoordN<T,2> point1 = polygon[num1] - point;
            CoordN<T,2> point2 = polygon[num2] - point;

            //check if edge i-num2 straddles x axis
            if (((point1[1] > 0.0) && (polygon[num2][1] <= 0.0)) ||
               ((point2[1] > 0.0) && (point1[1] <= 0.0))  )
            {
               //edge crosses ray if strictly positive intersection
               double det = point2[1] - point1[1];
               double d   = point1[0] * point2[1] -
                  point2[0] * point1[1]  ;

               if((d / det) > 0.0) crossings++;
            }
         }

         //if number of ray crossings odd, point is within polygon
         return (crossings % 2) != 0;
      }

      /**
      * @}
      */

   } // namespace math
} // namespace tc


#endif // _TC_COORD_UTIL_H_

