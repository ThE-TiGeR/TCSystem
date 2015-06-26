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
      template <typename T, uint32_t SIZE1, uint32_t SIZE2>
      class MatrixN: public boost::totally_ordered< MatrixN<T, SIZE1, SIZE2>,
         boost::additive       < MatrixN<T, SIZE1, SIZE2>,
         boost::multipliable2  < MatrixN<T, SIZE1, SIZE2>, T,
         boost::dividable2     < MatrixN<T, SIZE1, SIZE2>, T
      > > > >
      {
      public:
         typedef T* Iterator;
         typedef const T* ConstIterator;
         typedef T DataType;
         typedef ptrdiff_t DifferenceType;

         enum SizeTypes
         {
            NUM_COMPONENTS1  = SIZE1,
            NUM_COMPONENTS2  = SIZE1,
            ARRAY_DIMENSION = SIZE1 * SIZE2
         };

         enum class PredefinedType
         {
            ZERO,
            IDENTITY
         };

      public:
         MatrixN()
         {
         }

         explicit MatrixN(PredefinedType init)
         {
            if (init == PredefinedType::IDENTITY)
            {
               Identity();
            }
            else
            {
               Clear();
            }
         }

         MatrixN(const MatrixN& val) 
         {
            (*this) = val;
         }
         template <class T1>
         explicit MatrixN(const MatrixN<T1, SIZE1, SIZE2>& val)
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

         MatrixN<T, SIZE1, SIZE2>& operator=(const MatrixN& val) 
         {
            const T* in_data = val.m_data;
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data, ++in_data)
            {
               *data = *in_data;
            }
            return *this;
         }

         MatrixN<T, SIZE1, SIZE2>& operator=(T val) 
         {
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data)
            {
               *data = T(0);
            }

            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; data+=(SIZE1+1))
            {
               *data = val;
            }

            return *this;
         }

         const T& operator()(DifferenceType i, DifferenceType j)const 
         {
            return m_data[i*SIZE1+j];
         }

         T& operator()(DifferenceType i, DifferenceType j)
         {
            return m_data[i*SIZE1+j];
         }

         const T* operator[](DifferenceType i) const 
         {
            return &m_data[i*SIZE1];
         }

         T* operator[](DifferenceType i) 
         {
            return &m_data[i*SIZE1];
         }

         void Clear()
         {
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data)
            {
               *data = T(0);
            }
         }

         void Identity()
         {
            for (uint32_t i=0; i<SIZE1; i++)
            {
               for (uint32_t j=0; j<SIZE2; j++)
               {
                  m_data[i*SIZE1+j] = i == j ? T(1) : T(0);
               }
            }
         }

         void Transpose(const MatrixN& val)
         {
            for (uint32_t i=0; i<SIZE1; i++)
            {
               for (uint32_t j=0; j<SIZE2; j++)
               {
                  m_data[i*SIZE1+j] = val.m_data[j*SIZE1+i];
               }
            }
         }

         MatrixN<T, SIZE1, SIZE2> &operator+=(const MatrixN& val) 
         {
            const T* in_data = val.m_data;
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data, ++in_data)
            {
               *data += *in_data;
            }
            return *this;
         }

         MatrixN<T, SIZE1, SIZE2> &operator-=(const MatrixN& val) 
         {
            const T* in_data = val.m_data;
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data, ++in_data)
            {
               *data -= *in_data;
            }
            return *this;
         }

         MatrixN<T, SIZE1, SIZE2> &operator/=(T val) 
         {
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data)
            {
               *data /= val;
            }
            return *this;         
         }

         MatrixN<T, SIZE1, SIZE2> &operator*=(T val)
         {
            for (T* data=m_data; data<m_data+ARRAY_DIMENSION; ++data)
            {
               *data *= val;
            }
            return *this;         
         }

         bool operator==(const MatrixN& val) const
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

         bool operator<(const MatrixN& val) const
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

         template <class T1, uint32_t SIZEX1, uint32_t SIZEX2> friend class MatrixN;
      };

      /**
      * @}
      */

   } // namespace math
} // namespace tc

#endif //_TC_MATH_MATRIX_N_H_
