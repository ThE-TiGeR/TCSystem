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
// Copyright (C) 2003 - 2007 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCMathCoord4D.h 884 2008-07-24 09:48:48Z the_____tiger $
//*******************************************************************************

#ifndef _TC_MATH_COORD4D_H_
#define _TC_MATH_COORD4D_H_

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
      * @brief This file provides the definition of TC::Math::Coord4D
      * @author Thomas Goessler
      */

      /**
      * @brief Class for handling 3D values
      * With implementation of all common coordinate or vector operators needed
      */
      template <class T>
      class Coord4D
      {
      public:
         /** all four components are init with 0 */
         Coord4D()
         {
            m_data[0] = 0;
            m_data[1] = 0;
            m_data[2] = 0;
            m_data[3] = 0;
         }

         /**
         * Init all components with one value
         */
         explicit Coord4D(T vIn)
         {
            m_data[0] = vIn;
            m_data[1] = vIn;
            m_data[2] = vIn;
            m_data[3] = vIn;
         }

         /**
         * Constructor for setting all four components
         * with different values
         *
         * @param xIn value to init first vector component
         * @param yIn value to init second vector component
         * @param zIn value to init third vector component
         */
         Coord4D(T xIn, T yIn, T zIn, T wIn)
         {
            m_data[0] = xIn;
            m_data[1] = yIn;
            m_data[2] = zIn;
            m_data[3] = wIn;
         }

         /**
         * Copy constructor
         *
         * @param coord value to copy
         */
         Coord4D(const Coord4D<T>& coord)
         {
            m_data[0]=coord.m_data[0];
            m_data[1]=coord.m_data[1];
            m_data[2]=coord.m_data[2];
            m_data[3]=coord.m_data[3];
         } 


         /** Get one of the 4 component values */
         inline const T& operator[] (sint32 pos) const { return m_data[pos];}
         /** Set one of the 4 component values */
         inline T& operator[] (sint32 pos) { return m_data[pos];}

         // --------------------------------------------------
         // Assignment operators
         Coord4D<T>& operator=(const Coord4D<T>& a) 
         { 
            m_data[0]=a.m_data[0]; 
            m_data[1]=a.m_data[1]; 
            m_data[2]=a.m_data[2];    
            m_data[3]=a.m_data[3];    
            return *this;
         }
         Coord4D<T>& operator+=(const Coord4D<T>& b) 
         {
            m_data[0]+=b.m_data[0]; 
            m_data[1]+=b.m_data[1]; 
            m_data[2]+=b.m_data[2]; 
            m_data[3]+=b.m_data[3]; 
            return *this;
         }
         Coord4D<T>& operator-=(const Coord4D<T>& b) 
         {
            m_data[0]-=b.m_data[0]; 
            m_data[1]-=b.m_data[1]; 
            m_data[2]-=b.m_data[2]; 
            m_data[3]-=b.m_data[3]; 
            return *this;
         }
         Coord4D<T>& operator*=(const T &vIn)        
         {
            m_data[0]*=vIn; 
            m_data[1]*=vIn; 
            m_data[2]*=vIn;                         
            m_data[3]*=vIn;                         
            return *this;
         }
         Coord4D<T>& operator/=(const T &vIn)        
         { 
            m_data[0]/=vIn; 
            m_data[1]/=vIn; 
            m_data[2]/=vIn;                        
            m_data[3]/=vIn;                        
            return *this;
         }
         // --------------------------------------------------


         /** @return The template argument pointer type of this coordinate */
         operator T*() {return m_data;}
         /** @return The const template argument pointer type of this coordinate */
         operator const T*() const {return m_data;}

         /** @return The square length of the vector */
         double Length2() const 
         { 
            return m_data[0]*m_data[0] + 
                   m_data[1]*m_data[1] + 
                   m_data[2]*m_data[2] + 
                   m_data[3]*m_data[3];
         }
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
               m_data[3] = (T)((double)(m_data[2])/len);
            }
         }

         typedef T DataType;
      private:
         /** stores the four components of the coordinate */
         T m_data[4];      
      };

      template <class T>
      inline Coord4D<T> operator*(const Coord4D<T> &v, const T &vIn)
      {
         return Coord4D<T>(v[0]*vIn, v[1]*vIn, v[2]*vIn, v[3]*vIn);
      }

      template <class T>
      inline Coord4D<T> operator*(const T &vIn, const Coord4D<T> &v)
      {
         return Coord4D<T>(v[0]*vIn, v[1]*vIn, v[2]*vIn, v[3]*vIn);
      }

      template <class T>
      inline Coord4D<T> operator/(const Coord4D<T> &v, const T &vIn)
      {
         return Coord4D<T>(v[0]/vIn, v[1]/vIn, v[2]/vIn, v[3]/vIn);
      }

      template <class T>
      inline Coord4D<T> operator/(const T &vIn, const Coord4D<T> &v)
      {
         return Coord4D<T>(v[0]/vIn, v[1]/vIn, v[2]/vIn, v[3]/vIn);
      }

      template <class T>
      inline Coord4D<T> operator+(const Coord4D<T>& v, const Coord4D<T>& b)
      {
         return Coord4D<T>(v[0]+b[0], v[1]+b[1], v[2]+b[2], v[3]+b[3]);
      }

      template <class T>
      inline Coord4D<T> operator-(const Coord4D<T>& v, const Coord4D<T>& b)
      {
         return Coord4D<T>(v[0]-b[0], v[1]-b[1], v[2]-b[2], v[3]-b[3]);
      }

      template <class T>
      inline Coord4D<T> operator-(const Coord4D<T>& v)
      {
         return Coord4D<T>(-v[0], -v[1], -v[2], , -v[3]);
      }


      // ------------------------------------------------------------------------------------------------------------
      // operators for coordinates checking
      // ------------------------------------------------------------------------------------------------------------
      template <class T>
      inline bool operator>(const Coord4D<T> &a, const Coord4D<T> &b)
      {
         if (a[0] > b[0]) return true;
         if (a[0] < b[0]) return false;
         if (a[1] > b[1]) return true;
         if (a[1] < b[1]) return false;
         if (a[2] > b[2]) return true;
         if (a[2] < b[2]) return false;
         if (a[2] > b[3]) return true;
         if (a[2] < b[3]) return false;
         return false;
      }

      template <class T>
      inline bool operator<(const Coord4D<T> &a, const Coord4D<T> &b)
      {
         if (a[0] < b[0]) return true;
         if (a[0] > b[0]) return false;
         if (a[1] < b[1]) return true;
         if (a[1] > b[1]) return false;
         if (a[2] < b[2]) return true;
         if (a[2] > b[2]) return false;
         if (a[3] < b[3]) return true;
         if (a[3] > b[3]) return false;
         return false;
      }

      template <class T>
      inline bool operator==(const Coord4D<T> &a, const Coord4D<T> &b)
      {
         return (a[0]==b[0] && a[1]==b[1] && a[2]==b[2] && a[3]==b[3]);
      }

      template <class T>
      inline bool operator!=(const Coord4D<T> &a, const Coord4D<T> &b)
      {
         return (a[0]!=b[0] || a[1]!=b[1] || a[2]!=b[2] || a[3]!=b[3]);
      }

      template <class T>
      inline bool operator>=(const Coord4D<T> &a, const Coord4D<T> &b)
      {
         return (a==b || a>b);
      }

      template <class T>
      inline bool operator<=(const Coord4D<T> &a, const Coord4D<T> &b) 
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
      inline StreamPtr operator>>(StreamPtr stream, Coord4D<T>&coord)
      {
         return stream >> coord[0] >> coord[1] >> coord[2] >> coord[3];
      }
      /**
      * Write operator for writing the data to a TCStream
      *
      * @param stream stream where to write the data to
      * @param coord The coordinate to write on the stream
      * @return the stream
      */
      template <class T>
      inline StreamPtr operator<<(StreamPtr stream, const Coord4D<T>&coord)
      {
         return stream << coord[0] << space << coord[1] << space << coord[2] << space << coord[3];
      }

      /**
      * @}
      */

   } // namespace Math
} // namespace TC

#endif //_TC_MATH_COORD4D_H_
