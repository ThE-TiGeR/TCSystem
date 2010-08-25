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
//  $Id: TCMathCoord3D.h 969 2010-04-14 19:55:28Z the_____tiger $
//*******************************************************************************

#ifndef _TCCOORD3D_H_
#define _TCCOORD3D_H_

#include "TCMathUtil.h"
#include "TCStream.h"
#include "TCUtil.h"

#include <cmath>

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
    * @brief This file provides the definition of TC::Math::Coord3D
    * @author Thomas Goessler
    */

   /**
    * @brief Class for handling 3D values
    * With implementation of all common coordinate or vector operators needed
    */
   template <class T>
   class Coord3D
   {
   public:
      /** all thre components are init with 0 */
      Coord3D()
      {
         m_data[0] = T(0);
         m_data[1] = T(0);
         m_data[2] = T(0);
      }

      /**
       * Init all components with one value
       */
      template <class T1>
      explicit Coord3D(const T1& vIn)
      {
         m_data[0] = T(vIn);
         m_data[1] = T(vIn);
         m_data[2] = T(vIn);
      }

      /**
      * Init components with values of an array
      */
      template <class T1>
      explicit Coord3D(const T1 vIn[])
      {
         m_data[0] = T(vIn[0]);
         m_data[1] = T(vIn[1]);
         m_data[2] = T(vIn[2]);
      }

      /**
       * Constructor for setting all three components
       * with different values
       *
       * @param xIn value to init first vector component
       * @param yIn value to init second vector component
       * @param zIn value to init third vector component
       */
      template <class T1>
      Coord3D(const T1& xIn, const T1& yIn, const T1& zIn)
      {
         m_data[0] = T(xIn);
         m_data[1] = T(yIn);
         m_data[2] = T(zIn);
      }

      /**
       * Copy constructor
       *
       * @param coord value to copy
       */
      Coord3D(const Coord3D<T>& coord)
      {
         m_data[0]=coord.m_data[0];
         m_data[1]=coord.m_data[1];
         m_data[2]=coord.m_data[2];
      } 

      template <class T1>
      explicit Coord3D(const Coord3D<T1>& coord)
      {
         m_data[0]=T(coord.m_data[0]);
         m_data[1]=T(coord.m_data[1]);
         m_data[2]=T(coord.m_data[2]);
      } 

      /** Get one of the 3 component values */
      inline const T& operator[] (sint32 pos) const { return m_data[pos];}
      /** Set one of the 3 component values */
      inline T& operator[] (sint32 pos) { return m_data[pos];}

      // --------------------------------------------------
      // Assignment operators
      Coord3D<T>& operator= (const Coord3D<T>& a) { m_data[0]=a.m_data[0]; m_data[1]=a.m_data[1]; m_data[2]=a.m_data[2];    return *this;}
      Coord3D<T>& operator= (const T &vIn)        { m_data[0]=m_data[1]=m_data[2]=vIn;                                      return *this;}
      Coord3D<T>& operator+=(const Coord3D<T>& b) { m_data[0]+=b.m_data[0]; m_data[1]+=b.m_data[1]; m_data[2]+=b.m_data[2]; return *this;}
      Coord3D<T>& operator+=(const T &b)          { m_data[0]+=b; m_data[1]+=b; m_data[2]+=b;                               return *this;}
      Coord3D<T>& operator-=(const Coord3D<T>& b) { m_data[0]-=b.m_data[0]; m_data[1]-=b.m_data[1]; m_data[2]-=b.m_data[2]; return *this;}
      Coord3D<T>& operator-=(const T &b)          { m_data[0]-=b; m_data[1]-=b; m_data[2]-=b;                               return *this;}
      Coord3D<T>& operator*=(const T &vIn)        { m_data[0]*=vIn; m_data[1]*=vIn; m_data[2]*=vIn;                         return *this;}
      Coord3D<T>& operator/=(const T &vIn)        { m_data[0]/=vIn; m_data[1]/=vIn; m_data[2]/=vIn;                         return *this;}
      // --------------------------------------------------

      
      /** @return The template argument pointer type of this coordinate */
      operator T*() {return m_data;}
      /** @return The const template argument pointer type of this coordinate */
      operator const T*() const {return m_data;}

      /** @return The square length of the vector */
      double Length2() const { return (m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2]);}
      /** @return The length of the vector */
      double Length()  const { return std::sqrt(Length2());}
      /**
       * Normalize vector
       * Check for 0 len is done
       */
      void Normalize()
      {
         double len = Length2();
         if (len > 0.0)
         {
            len = std::sqrt(len);
            m_data[0] = (T)((double)(m_data[0])/len);
            m_data[1] = (T)((double)(m_data[1])/len);
            m_data[2] = (T)((double)(m_data[2])/len);
         }
      }
      
      /**
       * @return the maximum of all 3 components
       */
      T MaxValue() const
      {
         return Util::Max(m_data[0], m_data[1], m_data[2]);
      }

      typedef T DataType;
      enum
      {
         NUM_COMPONENTS = 3
      };
   private:
      /** stores the three components of the coordinate */
      T m_data[NUM_COMPONENTS];  

      template <class T1> friend class Coord3D;
   };

   template <class T>
   inline const Coord3D<T> operator^(const Coord3D<T> &a, const Coord3D<T> &b)
   {
      return Coord3D<T>(a[1] * b[2] - a[2] * b[1],
                        a[2] * b[0] - a[0] * b[2],
                        a[0] * b[1] - a[1] * b[0]);
   }

   template <class T>
   inline const T operator*(const Coord3D<T> &a, const Coord3D<T> &b)
   {
      return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
   }

   template <class T>
   inline const Coord3D<T> operator*(const Coord3D<T> &v, const T &vIn)
   {
      return Coord3D<T>(v[0]*vIn, v[1]*vIn, v[2]*vIn);
   }

   template <class T>
   inline const Coord3D<T> operator*(const T &vIn, const Coord3D<T> &v)
   {
      return Coord3D<T>(v[0]*vIn, v[1]*vIn, v[2]*vIn);
   }

   template <class T>
   inline const Coord3D<T> operator/(const Coord3D<T> &v, const T &vIn)
   {
      return Coord3D<T>(v[0]/vIn, v[1]/vIn, v[2]/vIn);
   }

   template <class T>
   inline const Coord3D<T> operator/(const T &vIn, const Coord3D<T> &v)
   {
      return Coord3D<T>(v[0]/vIn, v[1]/vIn, v[2]/vIn);
   }

   template <class T>
   inline const Coord3D<T> operator/(const Coord3D <T> &in, const Coord3D <T> &in2)
   {
      
      return Coord3D<T>(in[0]/in2[0], in[1]/in2[1], in[2]/in2[2]);

   }

   template <class T>
   inline const Coord3D<T> operator+(const Coord3D<T>& v, const Coord3D<T>& b)
   {
      return Coord3D<T>(v[0]+b[0], v[1]+b[1], v[2]+b[2]);
   }

   template <class T>
   inline const Coord3D<T> operator+(const Coord3D<T>& v, const T& b)
   {
      return Coord3D<T>(v[0]+b, v[1]+b, v[2]+b);
   }

   template <class T>
   inline const Coord3D<T> operator-(const Coord3D<T>& v, const Coord3D<T>& b)
   {
      Coord3D<T> a;
      a[0] = v[0]-b[0];
      a[1] = v[1]-b[1];
      a[2] = v[2]-b[2];
      
      return a;
   }

   template <class T>
   inline const Coord3D<T> operator-(const Coord3D<T>& v, const T& b)
   {
      return Coord3D<T>(v[0]-b, v[1]-b, v[2]-b);
   }

   template <class T>
   inline const Coord3D<T> operator-(const Coord3D<T>& v)
   {
      return Coord3D<T>(-v[0], -v[1], -v[2]);
   }


   // ------------------------------------------------------------------------------------------------------------
   // operators for coordinates checking
   // ------------------------------------------------------------------------------------------------------------
   template <class T>
   inline bool operator>(const Coord3D<T> &a, const Coord3D<T> &b)
   {
      if (a[0] > b[0]) return true;
      if (a[0] < b[0]) return false;
      if (a[1] > b[1]) return true;
      if (a[1] < b[1]) return false;
      if (a[2] > b[2]) return true;
      if (a[2] < b[2]) return false;
      return false;
   }

   template <class T>
   inline bool operator<(const Coord3D<T> &a, const Coord3D<T> &b)
   {
      if (a[0] < b[0]) return true;
      if (a[0] > b[0]) return false;
      if (a[1] < b[1]) return true;
      if (a[1] > b[1]) return false;
      if (a[2] < b[2]) return true;
      if (a[2] > b[2]) return false;
      return false;
   }

   template <class T>
   inline bool operator==(const Coord3D<T> &a, const Coord3D<T> &b)
   {
      return (a[0]==b[0] && a[1]==b[1] && a[2]==b[2]);
   }

   template <class T>
   inline bool operator!=(const Coord3D<T> &a, const Coord3D<T> &b)
   {
      return (a[0]!=b[0] || a[1]!=b[1] || a[2]!=b[2]);
   }

   template <class T>
   inline bool operator>=(const Coord3D<T> &a, const Coord3D<T> &b)
   {
      return (a==b || a>b);
   }

   template <class T>
   inline bool operator<=(const Coord3D<T> &a, const Coord3D<T> &b) 
   {
      return (a==b || a<b);
   }

   /**
    * Read operator for reading the data from a TCStream
    *
    * @param stream stream where to read the data from
    * @param coord The coordinate to read from the stream
    * @return the stream
    */
   template <class T>
   inline StreamPtr operator>>(StreamPtr stream, Coord3D<T>&coord)
   {
      return stream >> coord[0] >> coord[1] >> coord[2];
   }
   /**
    * Write operator for writing the data to a TCStream
    *
    * @param stream stream where to write the data to
    * @param coord The coordinate to write on the stream
    * @return the stream
    */
   template <class T>
   inline StreamPtr operator<<(StreamPtr stream, const Coord3D<T>&coord)
   {
      return stream << coord[0] << space << coord[1] << space << coord[2];
   }

   /**
    * @}
    */

} // namespace Math
} // namespace TC

#endif //_TCCOORD3D_H_
