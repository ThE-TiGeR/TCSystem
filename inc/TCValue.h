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
//  $Id: TCValue.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TCVALUE_H_
#define _TCVALUE_H_

#include "TCUtil.h"

namespace TC
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file 
    * This header file provides the definition of the class TC::Value, 
    * TC::ByteOrderedValue, TC::LittleEndianValue and TC::BigEndianValue
    * 
    * @author Thomas Goessler 
    */

   /**
   * Class for holding none byte ordered data types
   */
   template < class T>
   class Value
   {
   public:
      /**
      * default constructor
      * value is init with zero
      * this means the template type must have an operator to init with 0
      */
      Value()
      {
         m_val = (T)0;
      }
      /**
      * copy constructor
      * @param val to copy
      */
      Value(const Value<T> &val)
      {
         m_val = val.m_val;
      }
      /**
      * constructor to init with specified value
      * @param val value use for init
      */
      Value(const T &val)
      {
         m_val = val;
      }

      /** cast back to const basic type */
      operator const T&() const
      {
         return m_val;
      }

      /** @return bytes in little endian const*/
      const uchar* GetBytes() const { return reinterpret_cast<const uchar*>(&m_val); }
      /** @return bytes in little endian non const */
      uchar* GetBytes() { return reinterpret_cast<uchar*>(&m_val); }
      /** @return number of bytes of value */
      uint32 GetNumBytes() const { return (uint32)sizeof(T); }

      /** assignment operator for setting values */
      Value<T>& operator=(const T &a)
      {
         m_val = a;
         return *this;
      }

   private:
      /** stores the value of */
      T m_val;  
   };

   /**
    * Class for holding byte ordered data types
    */
   template < class T, bool IS_LITTLE_ENDIAN >
   class ByteOrderedValue
   {
   protected:
      /**
       * default constructor
       * value is init with zero
       * this means the template type must have an operator to init with 0
       */
      ByteOrderedValue()
      {
         m_val = (T)0;
         SwapBytes(m_val);
      }
      /**
       * copy constructor
       * @param val to copy
       */
      ByteOrderedValue(const ByteOrderedValue<T, IS_LITTLE_ENDIAN> &val)
      {
         m_val = val.m_val;
      }
      /**
       * constructor to init with specified value
       * @param val value use for init
       */
      ByteOrderedValue(const T &val)
      {
         m_val = val;
         SwapBytes(m_val);
      }

   public:
      /** cast back to const basic type */
      operator T() const
      {
         T valOut = m_val;
         SwapBytes(valOut);
         return valOut;
      }

      /** @return bytes in little endian const*/
      const uchar* GetBytes() const { return reinterpret_cast<const uchar*>(&m_val); }
      /** @return bytes in little endian non const */
      uchar* GetBytes() { return reinterpret_cast<uchar*>(&m_val); }
      /** @return number of bytes of value */
      uint32 GetNumBytes() const { return (uint32)sizeof(T); }
      /** assign the value because of bytes */
      void SetBytes(const uchar* bytes) {m_val = *reinterpret_cast<const T*>(bytes);}
      /** assignment operator for setting values */
      ByteOrderedValue<T, IS_LITTLE_ENDIAN>& operator=(const T &a)
      {
         m_val = a;
         SwapBytes(m_val);
         return *this;
      }

   private:
      /** method which swaps bytes if needed */
      inline void SwapBytes(T &val) const
      {
         if (Util::IsLittleEndian()!=IS_LITTLE_ENDIAN)
         {
            Util::SwapBytes(val);
         }
      }

      /** stores the value of */
      T m_val;  
   };

   /**
    * Class for all kind of basic types where
    * the byte ordering is always little endian
    */
   template <class T>
   class LittleEndianValue: public ByteOrderedValue<T, true>
   {
   public:
      LittleEndianValue():ByteOrderedValue<T, true>() {}
      LittleEndianValue(const LittleEndianValue<T> &val):ByteOrderedValue<T, true>(val) {}
      LittleEndianValue(const T &val):ByteOrderedValue<T, true>(val) {}
   };

   /**
    * Class for all kind of basic types where
    * the byte ordering is always little endian
    */
   template <class T>
   class BigEndianValue: public ByteOrderedValue<T, false>
   {
   public:
      BigEndianValue():ByteOrderedValue<T, false>() {}
      BigEndianValue(const BigEndianValue<T> &val):ByteOrderedValue<T, false>(val) {}
      BigEndianValue(const T &val):ByteOrderedValue<T, false>(val) {}
   };

   /**
    * @}
    */
}

#endif // _TCVALUE_H_
