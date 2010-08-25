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
//  $Id: TCMathCoord2D.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TCCOORD_2D_H_
#define _TCCOORD_2D_H_

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
    * @brief This file provides the definition of TC::Math::Coord2D
    * @author Thomas Goessler
    */

   /**
    * @brief Class for handling 2D values
    * with implementation of all common operators needed
    */
   template <class T>
   class Coord2D
   {
   public:
      /**
       * default constructor
       * @param vIn value to init both vector components
       */
      Coord2D(T vIn=(T)0)
      {
         m_data[0]=vIn;
         m_data[1]=vIn;
      }
      /**
       * constructor for setting both components
       * @param xIn value to init first vector component
       * @param yIn value to init second vector component
       */
      Coord2D(const T xIn, const T yIn)
      {
         m_data[0]=xIn;
         m_data[1]=yIn;
      }
      /**
       * copy constructor
       * @param a value to copy
       */
      Coord2D(const Coord2D<T>& a)
      {
         m_data[0]=a.m_data[0];
         m_data[1]=a.m_data[1];
      } 
      // --------------------------------------------------

      // --------------------------------------------------
      // accesing values
      const T& operator[] (sint32 pos) const { return m_data[pos];}
      // setting values
      T& operator[] (sint32 pos) { return m_data[pos];}
      // --------------------------------------------------

      // --------------------------------------------------
      // Assignment operators
      Coord2D<T>& operator=(const Coord2D<T>& a)  { m_data[0]=a.m_data[0]; m_data[1]=a.m_data[1]; return *this;}
      Coord2D<T>& operator=(const T &vIn)         { m_data[0]=m_data[1]=vIn; return *this;}
      Coord2D<T>& operator+=(const Coord2D<T>& b) { m_data[0]+=b.m_data[0]; m_data[1]+=b.m_data[1]; return *this;}
      Coord2D<T>& operator+=(const T &b)          { m_data[0]+=b; m_data[1]+=b; return *this;}
      Coord2D<T>& operator-=(const Coord2D<T>& b) { m_data[0]-=b.m_data[0]; m_data[1]-=b.m_data[1]; return *this;}
      Coord2D<T>& operator-=(const T &b)          { m_data[0]-=b; m_data[1]-=b; return *this;}
      Coord2D<T>& operator*=(const T &vIn)        { m_data[0]*=vIn; m_data[1]*=vIn; return *this;}
      Coord2D<T>& operator/=(const T &vIn)        { m_data[0]/=vIn; m_data[1]/=vIn; return *this;}
      // --------------------------------------------------

      /** @return The template argument pointer type of this coordinate */
      operator T*() {return m_data;}
      /** @return The const template argument pointer type of this coordinate */
      operator const T*() const {return m_data;}

      /**
       * convert the vector to a pointer to the values
       * @return the pointer to the values
       */
      T* GetPtr()       {return m_data;}
      /**
       * convert the vector to a pointer to the values
       * @return the const pointer to the values
       */
      const T* GetPtr() const {return m_data;}
      /**
       * @return the square length of the vector
       */
      double Length2() const { return (m_data[0]*m_data[0] + m_data[1]*m_data[1]);}
      /**
       * @return the length of the vector
       */
      double Length () const { return std::sqrt(Length2());}
      /**
       * normalize vector
       * check is done for 0 len
       */
      void Normalize()                                                    
      {
         double len = Length2();
         if (len > 0.0)
         {
            len = std::sqrt(len);
            m_data[0] = (T)((double)(m_data[0])/len);
            m_data[1] = (T)((double)(m_data[1])/len);
         }
      }
      /**
       * @return the maximum of all 3 components
       */
      T MaxValue() const
      {
         return Util::Max(m_data[0], m_data[1]);
      }

      typedef T DataType;
      enum
      {
         NUM_COMPONENTS = 2
      };
   private:
      T m_data[NUM_COMPONENTS];
   };

   template <class T>
   inline const Coord2D<T> operator*(const Coord2D<T> &v, const T &vIn)
   {
      return Coord2D<T>(v[0]*vIn, v[1]*vIn);
   }

   template <class T>
   inline const Coord2D<T> operator*(const T &vIn, const Coord2D<T> &v)
   {
      return Coord2D<T>(v[0]*vIn, v[1]*vIn);
   }

   template <class T>
   inline const T operator*(const Coord2D<T> &a, const Coord2D<T> &b)
   {
      return (a[0] * b[0] + a[1] * b[1]);
   }

   template <class T>
   inline const Coord2D<T> operator/(const Coord2D<T> &v, const T &vIn)
   {
      return Coord2D<T>(v[0]/vIn, v[1]/vIn);
   }

   template <class T>
   inline const Coord2D<T> operator/(const T &vIn, const Coord2D<T> &v)
   {
      return Coord2D<T>(v[0]/vIn, v[1]/vIn);
   }

   template <class T>
   inline const Coord2D<T> operator+(const Coord2D<T>& v, const Coord2D<T>& b)
   {
      return Coord2D<T>(v[0]+b[0], v[1]+b[1]);
   }

   template <class T>
   inline const Coord2D<T> operator+(const Coord2D<T>& v, const T& b)
   {
      return Coord2D<T>(v[0]+b, v[1]+b);
   }

   template <class T>
   inline const Coord2D<T> operator-(const Coord2D<T>& v, const Coord2D<T>& b)
   {
      return Coord2D<T>(v[0]-b[0], v[1]-b[1]);
   }

   template <class T>
   inline const Coord2D<T> operator-(const Coord2D<T>& v, const T& b)
   {
      return Coord2D<T>(v[0]-b, v[1]-b);
   }

   template <class T>
   inline const Coord2D<T> operator-(const Coord2D<T>& v)
   {
      return Coord2D<T>(-v[0], -v[1]);
   }

   // ------------------------------------------------------------------------------------------------------------
   // operators for coordinates checking
   // ------------------------------------------------------------------------------------------------------------
   template <class T>
   inline bool operator>(const Coord2D<T>&a, const Coord2D<T>&b)
   {
      if (a[0] > b[0]) return true;
      if (a[0] < b[0]) return false;
      if (a[1] > b[1]) return true;
      if (a[1] < b[1]) return false;
      return 0;
   }

   template <class T>
   inline bool operator<(const Coord2D<T>&a, const Coord2D<T>&b)
   {
      if (a[0] < b[0]) return true;
      if (a[0] > b[0]) return false;
      if (a[1] < b[1]) return true;
      if (a[1] > b[1]) return false;
      return false;
   }

   template <class T>
   inline bool operator==(const Coord2D<T>&a, const Coord2D<T>&b)
   {
      return (a[0]==b[0] && a[1]==b[1]);
   }

   template <class T>
   inline bool operator!=(const Coord2D<T>&a, const Coord2D<T>&b)
   {
      return (a[0]!=b[0] || a[1]!=b[1]);
   }

   template <class T>
   inline bool operator>=(const Coord2D<T>&a, const Coord2D<T>&b)
   {
      return (a==b || a>b);
   }

   template <class T>
   inline bool operator<=(const Coord2D<T>&a, const Coord2D<T>&b)
   {
      return (a==b || a<b);
   }

   /**
    * read operator for reading the data from a Stream
    * @param stream stream where to read the data from
    * @param coord The coordinate to read from the stream
    * @return the stream
    */
   template <class T>
   inline StreamPtr operator>>(StreamPtr stream, Coord2D<T>& coord)
   {
      return stream >> coord[0] >> coord[1];
   }
   /**
    * write operator for writing the data to a Stream
    * @param stream stream where to write the data to
    * @param coord The coordinate to write to the stream
    * @return the stream
    */
   template <class T>
   inline StreamPtr operator<<(StreamPtr stream, const Coord2D<T>& coord)
   {
      return stream << coord[0] << space << coord[1];
   }

   /**
    * @}
    */

} // namespace Math
} // namespace TC

#endif // _TCCOORD_2D_H_
