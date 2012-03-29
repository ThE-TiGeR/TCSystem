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
//  $Id:$
//*******************************************************************************

#ifndef _TC_MATH_MATRIX_N_H_
#define _TC_MATH_MATRIX_N_H_

#include "TCStream.h"

#include <boost/operators.hpp>

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
      * @brief This file provides the definition of tc::math::MatrixN
      * @author Thomas Goessler
      */

      /**
      * @brief Class for handling N values
      * With implementation of all common coordinate or vector operators needed
      */
      template <typename T, uint32 SIZE>
      class MatrixN: public boost::totally_ordered< MatrixN<T, SIZE>,
         boost::additive       < MatrixN<T, SIZE>,
         boost::multipliable2  < MatrixN<T, SIZE>, T,
         boost::dividable2     < MatrixN<T, SIZE>, T
      > > > >
      {
      public:
         typedef T* Iterator;
         typedef const T* ConstIterator;
         typedef T DataType;

         enum
         {
            NUM_COMPONENTS  = SIZE,
            ARRAY_DIMENSION = SIZE * SIZE
         };

      public:
         MatrixN()
         {
         }
         MatrixN(const MatrixN<T, SIZE>& val) 
         {
            (*this) = val;
         }
         template <class T1>
         explicit MatrixN(const MatrixN<T1, SIZE>& val)
         {
            const T1* in_data = val.m_data;
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data, ++in_data)
            {
               *data = T(*in_data);
            }
         } 
         explicit MatrixN(T val) 
         {
            (*this) = val;
         }

         MatrixN<T, SIZE>& operator=(const MatrixN<T, SIZE>& val) 
         {
            const T* in_data = val.m_data;
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data, ++in_data)
            {
               *data = *in_data;
            }
            return *this;
         }

         MatrixN<T, SIZE>& operator=(T val) 
         {
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data)
            {
               *data = T(0);
            }

            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; data+=(SIZE+1))
            {
               *data = val;
            }

            return *this;
         }

         const T& operator()(uint32 i, uint32 j)const 
         {
            return m_data[i*SIZE+j];
         }

         T& operator()(const uint32 i, uint32 j)
         {
            return m_data[i*SIZE+j];
         }

         const T* operator[](uint32 i) const 
         {
            return &m_data[i*SIZE];
         }

         T* operator[](uint32 i) 
         {
            return &m_data[i*SIZE];
         }

         void Identity()
         {
            for (uint32 i=0; i<SIZE; i++)
            {
               for (uint32 j=0; j<SIZE; j++)
               {
                  m_data[i*SIZE+j] = i == j ? 1 : 0;
               }
            }
         }

         void Transpose(const MatrixN<T, SIZE>& val)
         {
            for (uint32 i=0; i<SIZE; i++)
            {
               for (uint32 j=0; j<SIZE; j++)
               {
                  m_data[i*SIZE+j] = val.m_data[j*SIZE+i];
               }
            }
         }

         MatrixN<T, SIZE> &operator+=(const MatrixN<T, SIZE>& val) 
         {
            const T* in_data = val.m_data;
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data, ++in_data)
            {
               *data += *in_data;
            }
            return *this;
         }

         MatrixN<T, SIZE> &operator-=(const MatrixN<T, SIZE>& val) 
         {
            const T* in_data = val.m_data;
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data, ++in_data)
            {
               *data -= *in_data;
            }
            return *this;
         }

         MatrixN<T, SIZE> &operator/=(T val) 
         {
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data)
            {
               *data /= val;
            }
            return *this;         
         }

         MatrixN<T, SIZE> &operator*=(T val)
         {
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data)
            {
               *data *= val;
            }
            return *this;         
         }

         bool operator==(const MatrixN<T, SIZE>& val) const
         {
            const T* in_data = val.m_data;
            for (const T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data, ++in_data)
            {
               if (*data != *in_data)
               {
                  return false;
               }
            }
            return true;
         }

         bool operator<(const MatrixN<T, SIZE>& val) const
         {
            const T* in_data = val.m_data;
            const T* data = m_data;
            for (; data<m_data+ARRAY_DIMENSION-1; ++data, ++in_data)
            {
               if (*data < *in_data)
               {
                  return true;
               }
               else if (*data > *in_data)
               {
                  return false;
               }
            }
            return *data < *in_data;
         }

         operator T*() {return m_data;}
         operator const T*() const {return m_data;}

         Iterator Begin() {return m_data;}
         ConstIterator Begin() const {return m_data;}

         Iterator End() {return m_data + ARRAY_DIMENSION;}
         ConstIterator End() const {return m_data + ARRAY_DIMENSION;}

      private:
         T m_data[ARRAY_DIMENSION];

         template <class T1, uint32 SIZE1> friend class MatrixN;
      };

      /**
      * @}
      */

   } // namespace math
} // namespace tc

#endif //_TC_MATH_MATRIX_N_H_
