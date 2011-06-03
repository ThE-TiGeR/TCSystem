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

#ifndef _TC_COORD_UTIL_H_
#define _TC_COORD_UTIL_H_

#include "TCMathCoord3D.h"
#include "TCMathCoord2D.h"

namespace TC
{
   namespace Math
   {
      /**
      * @addtogroup TC_MATH
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of Coordinate utilities in the namespace TC::Math
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
      * @return The square distance between the towo coordinates
      */
      template <class COORD_TYPE>
      inline double Distance2(const COORD_TYPE &a, const COORD_TYPE &b)
      {
         COORD_TYPE c = a-b;
         return c.Length2();
      }

      /**
      * Computes the dot product of two 3-coordinates (float-precision version).
      * @param a     the 1st 3D coordinate
      * @param b     the 2nd 3D coordinate
      * @return the Dot product of the 2 coordinates
      */
      template <class COORD_TYPE>
      inline typename COORD_TYPE::DataType DotProduct(const COORD_TYPE& a, const COORD_TYPE& b)
      {
         return a * b;
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
         COORD_TYPE v = a * b;
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

      template <class T>
      inline CoordN<T,3> Max(const CoordN<T,3>& a, const CoordN<T,3>& b)
      {
         CoordN<T,3> coord;
         coord[0] = Util::Max(a[0], b[0]);
         coord[1] = Util::Max(a[1], b[1]);
         coord[2] = Util::Max(a[2], b[2]);

         return coord;
      }

      template <class T>
      inline CoordN<T,2> Max(const CoordN<T,2>& a, const CoordN<T,2>& b)
      {
         CoordN<T,2> coord;
         coord[0] = Util::Max(a[0], b[0]);
         coord[1] = Util::Max(a[1], b[1]);

         return coord;
      }

      template <class T>
      inline CoordN<T,3> Min(const CoordN<T,3>& a, const CoordN<T,3>& b)
      {
         CoordN<T,3> coord;
         coord[0] = Util::Min(a[0], b[0]);
         coord[1] = Util::Min(a[1], b[1]);
         coord[2] = Util::Min(a[2], b[2]);

         return coord;
      }

      template <class T>
      inline CoordN<T,2> Min(const CoordN<T,2>& a, const CoordN<T,2>& b)
      {
         CoordN<T,2> coord;
         coord[0] = Util::Min(a[0], b[0]);
         coord[1] = Util::Min(a[1], b[1]);

         return coord;
      }

      /**
      * Calculates the normal vector of the plane defined by the points
      * @param numPoints Number of points defining the plane
      * @param coords The coordinates of the points defining the plane
      * @return The not normalized normal vector of the plane
      */
      template <class T>
      inline CoordN<T,3> Normalvector(uint32 numPoints, const CoordN<T,3>* coords)
      {
         CoordN<T,3> out;

         for (uint32 pos=0; pos<numPoints; pos++)
         {
            uint32 index = (pos+1) % numPoints;
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
         uint32 num_points, const CoordN<T,2> *polygon)
      {
         uint32 crossings = 0;
         //loop over all edges of polygon
         for(uint32 num1=0; num1<num_points; num1++)
         {
            uint32 num2 = (num1 - 1 + num_points) % num_points;
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

   } // namespace Math
} // namespace TC


#endif // _TC_COORD_UTIL_H_

