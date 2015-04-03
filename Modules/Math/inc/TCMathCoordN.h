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
//  $Id: TCMathCoordN.h,v 8cdc96e922c0 2013/03/05 22:20:17 thomas $
//*******************************************************************************

#ifndef _TCCOORDN_H_
#define _TCCOORDN_H_

#include "TCMathUtil.h"
#include "TCStream.h"
#include "TCUtil.h"

#include <cmath>
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
        * @brief This file provides the definition of tc::math::CoordN
        * @author Thomas Goessler
        */

        /**
        * @brief Class for handling N values
        * With implementation of all common coordinate or vector operators needed
        */
        template <typename T, uint32_t SIZE>
        class CoordN: public boost::totally_ordered      < CoordN<T, SIZE>,
            boost::additive             < CoordN<T, SIZE>,
            boost::ring_operators2      < CoordN<T, SIZE>, T,
            boost::dividable2           < CoordN<T, SIZE>, T
        > > > >
        {
        public:
            typedef T* Iterator;
            typedef const T* ConstIterator;
            typedef T DataType;

            enum
            {
                NUM_COMPONENTS = SIZE
            };

        public:
            /** all components are init with 0 */
            CoordN()
            {
                for (T* data=m_data; data<m_data+SIZE; ++data)
                {
                    *data = T(0);
                }
            }

            /** Init all components with one value */
            template <class T1>
            explicit CoordN(const T1& val)
            {
                for (T* data=m_data; data<m_data+SIZE; ++data)
                {
                    *data = T(val);
                }
            }

            /** Init components with values of an array */
            template <class T1>
            explicit CoordN(const T1 val[])
            {
                for (T* data=m_data; data<m_data+SIZE; ++data, ++val)
                {
                    *data = T(*val);
                }
            }

            /** Init components from two (Coord2D) */
            template <class T1>
            explicit CoordN(const T1& val0, const T1& val1)
            {
                m_data[0] = val0;
                m_data[1] = val1;
                for (T* data=m_data+2; data<m_data+SIZE; ++data)
                {
                    *data = 0;
                }
            }

            /** Init components from three (Coord3D) */
            template <class T1>
            explicit CoordN(const T1& val0, const T1& val1, const T1& val2)
            {
                m_data[0] = T(val0);
                m_data[1] = T(val1);
                m_data[2] = T(val2);
                for (T* data=m_data+3; data<m_data+SIZE; ++data)
                {
                    *data = 0;
                }
            }

            /** Init components from three (Coord4D) */
            template <class T1>
            explicit CoordN(const T1& val0, const T1& val1, const T1& val2, const T1& val3)
            {
               m_data[0] = T(val0);
               m_data[1] = T(val1);
               m_data[2] = T(val2);
               m_data[3] = T(val3);
               for (T* data=m_data+4; data<m_data+SIZE; ++data)
               {
                  *data = 0;
               }
            }

            /** Copy constructor */
            CoordN(const CoordN& coord)
            {
                const T* in_data = coord.m_data;
                for (T* data=m_data; data<m_data+SIZE; ++data, ++in_data)
                {
                    *data = *in_data;
                }
            } 

            /** Copy constructor with different template type */
            template <class T1>
            explicit CoordN(const CoordN<T1, SIZE>& coord)
            {
                const T1* in_data = coord.m_data;
                for (T* data=m_data; data<m_data+SIZE; ++data, ++in_data)
                {
                    *data = T(*in_data);
                }
            } 

            /** Get one of the n component values */
            inline const T& operator[](int pos) const { return m_data[pos];}
            /** Set one of the n component values */
            inline T& operator[](int pos) { return m_data[pos];}

            CoordN<T, SIZE>& operator=(T val) 
            { 
                for (T* data=m_data; data<m_data+SIZE; ++data)
                {
                    *data = val;
                }
                return *this;
            }

            CoordN<T, SIZE>& operator+=(const CoordN<T, SIZE>& val)
            { 
                const T* in_data = val.m_data;
                for (T* data=m_data; data<m_data+SIZE; ++data, ++in_data)
                {
                    *data += *in_data;
                }
                return *this;
            }

            CoordN<T, SIZE>& operator+=(T val)
            { 
                for (T* data=m_data; data<m_data+SIZE; ++data)
                {
                    *data += val;
                }
                return *this;
            }

            CoordN<T, SIZE>& operator-=(const CoordN<T, SIZE>& val)
            { 
                const T* in_data = val.m_data;
                for (T* data=m_data; data<m_data+SIZE; ++data, ++in_data)
                {
                    *data -= *in_data;
                }
                return *this;
            }

            CoordN<T, SIZE>& operator-=(T val)
            { 
                for (T* data=m_data; data<m_data+SIZE; ++data)
                {
                    *data -= val;
                }
                return *this;
            }

            CoordN<T, SIZE>& operator*=(T val)
            { 
                for (T* data=m_data; data<m_data+SIZE; ++data)
                {
                    *data *= val;
                }
                return *this;
            }

            CoordN<T, SIZE>& operator/=(T val)
            { 
                for (T* data=m_data; data<m_data+SIZE; ++data)
                {
                    *data /= val;
                }
                return *this;
            }

            bool operator==(const CoordN<T, SIZE>& val) const
            {
                const T* in_data = val.m_data;
                for (const T* data=m_data; data<m_data+SIZE; ++data, ++in_data)
                {
                    if (*data != *in_data)
                    {
                        return false;
                    }
                }
                return true;
            }

            bool operator<(const CoordN<T, SIZE>& val) const
            {
                const T* in_data = val.m_data;
                const T* data = m_data;
                for (; data<m_data+SIZE-1; ++data, ++in_data)
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

            /** @return The template argument pointer type of this coordinate */
            operator T*() {return m_data;}
            /** @return The const template argument pointer type of this coordinate */
            operator const T*() const {return m_data;}

            /** @return The square length of the vector */
            double Length2() const 
            { 
                double len = 0;
                for (const T* data=m_data; data<m_data+SIZE; ++data)
                {
                    len += (*data) * (*data);
                }
                return len;
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
                    this->operator/=(T(len));
                }
            }

            Iterator Begin() {return m_data;}
            ConstIterator Begin() const {return m_data;}

            Iterator End() {return m_data + SIZE;}
            ConstIterator End() const {return m_data + SIZE;}

        private:
            /** stores the three components of the coordinate */
            T m_data[NUM_COMPONENTS];  

            template <class T1, uint32_t SIZE1> friend class CoordN;
        };

        template <class T, uint32_t SIZE>
        inline const CoordN<T, SIZE> operator-(const CoordN<T, SIZE>& v)
        {
            return v * T(-1);
        }

        template <class T>
        inline const CoordN<T, 3> operator^(const CoordN<T, 3> &a, const CoordN<T, 3> &b)
        {
            return CoordN<T, 3>(a[1] * b[2] - a[2] * b[1],
                a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]);
        }

        /**
        * Read operator for reading the data from a TCStream
        *
        * @param stream stream where to read the data from
        * @param coord The coordinate to read from the stream
        * @return the stream
        */
        template <class T, uint32_t SIZE>
        inline StreamPtr operator>>(StreamPtr stream, CoordN<T, SIZE>&coord)
        {
            for (T* data=coord.Begin(); data<coord.End(); ++data)
            {
                stream >> (*data);
            }

            return stream;
        }
        /**
        * Write operator for writing the data to a TCStream
        *
        * @param stream stream where to write the data to
        * @param coord The coordinate to write on the stream
        * @return the stream
        */
        template <class T, uint32_t SIZE>
        inline StreamPtr operator<<(StreamPtr stream, const CoordN<T, SIZE>&coord)
        {
            for (const T* data=coord.Begin(); data<coord.End(); ++data)
            {
                stream << (*data);
                if (data + 1 < coord.End())
                {
                    stream << space;
                }
            }
            return stream;
        }

        /**
        * @}
        */

    } // namespace math
} // namespace tc

#endif //_TCCOORDN_H_
