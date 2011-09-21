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

#ifndef _TC_UTIL_H_
#define _TC_UTIL_H_

#include "TCTypes.h"

namespace TC
{
   /**
   * @addtogroup TC_BASE
   * @{
   */

   /**
   * @file 
   * This header file provides the definition of the namespace TC::Util. 
   * 
   * @author Thomas Goessler 
   */

   /**
   * @brief Defines basic functions like Min, Max, Swap ...
   */
   namespace Util
   {
      /** @return the absolute value of the given value */
      template <class T>
      inline T Abs(const T& x)
      {
         return (x < 0) ? -x : x;
      }

      /** @return maximum of two values */
      template <class T>
      inline const T& Max(const T& x, const T& y)
      {
         return (x > y) ? x : y;
      }
      /** @return maximum of three values */
      template <class T>
      inline const T& Max(const T& x, const T& y, const T& z)
      {
         return Max(x, Max(y, z));
      }

      /** @return minimum of two values */
      template <class T>
      inline const T& Min(const T& x, const T& y)
      {
         return (x < y) ? x : y;
      }
      /** @return minimum of three values */
      template <class T>
      inline const T& Min(const T& x, const T& y, const T& z)
      {
         return Min(x, Min(y, z));
      }


      /** @return Swap the two variables */
      template <class T>
      inline void Swap(T& x, T& y)
      {
         T tmp = x;
         x     = y;
         y     = tmp;
      }

      /** @brief Really frees memory of vector */
      template <class T>
      inline void FreeMemoryOfStlContainer(T& container)
      {
         T tmp;
         container.swap(tmp);
      }

      /** @brief Minimizes the memory usage of a vector */
      template <class T>
      inline void MinimizeMemoryOfStlContainer(T& container)
      {
         T tmp(container);
         container.swap(tmp);
      }

      /**
      * Function template for safe memory pointer deletion
      * @param mem memory pointer to delete
      */
      template <class T>
      inline void SafeRelease(T& mem)
      {
         if (mem != 0)
         {
            delete mem;
            mem = 0;
         }
      }

      /**
      * Function template for safe memory pointer deletion of an allocated array
      * @param mem memory pointer to delete
      */
      template <class T>
      inline void SafeReleaseArray(T& mem)
      {
         if (mem != 0)
         {
            delete[] mem;
            mem = 0;
         }
      }

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4100)
#endif
      /** @return The number of elements of an C array */
      template <class T>
      uint32 ArraySize(const T& array)
      {
         return sizeof(array)/sizeof(array[0]);
      }
#ifdef _MSC_VER
#pragma warning (pop)
#endif

      /** @brief Swaps the bytes of an uint8 value */
      inline void SwapBytes(uint8&)
      {
      }
      /** @brief Swaps the bytes of an uint16 value */
      inline void SwapBytes(uint16& val)
      {
          val = (val >> 8) | (val << 8);
      }
      /** @brief Swaps the bytes of an uint32 value */
      inline void SwapBytes(uint32& val)
      {
          val = (val >> 24) | ((val & 0x00ff0000) >> 8) | ((val & 0x0000ff00) << 8) | (val << 24);
      }
      /** @brief Swaps the bytes of an uint64 value */
      inline void SwapBytes(uint64& val)
      {
         uint32 *val_ptr = (uint32 *)&val;
         uint32 tmp1 = val_ptr[0];
         uint32 tmp2 = val_ptr[1];
         SwapBytes(tmp1);
         SwapBytes(tmp2);
         val_ptr[0] = tmp2;
         val_ptr[1] = tmp1;
      }

      /** @brief Swaps the bytes of an sint8 value */
      inline void SwapBytes(sint8&)
      {
      }
      /** @brief Swaps the bytes of an sint16 value */
      inline void SwapBytes(sint16& val)
      {
          SwapBytes((uint16&)val);
      }
      /** @brief Swaps the bytes of an sint32 value */
      inline void SwapBytes(sint32& val)
      {
          SwapBytes((uint32&)val);
      }
      /** @brief Swaps the bytes of an sint64 value */
      inline void SwapBytes(sint64& val)
      {
          SwapBytes((uint64&)val);
      }
      /** @brief Swaps the bytes of an float value */
      inline void SwapBytes(float& val)
      {
          SwapBytes((uint32&)val);
      }
      /** @brief Swaps the bytes of an double value */
      inline void SwapBytes(double& val)
      {
          SwapBytes((uint64&)val);
      }

      /** @brief Swaps the bytes of any type */
      template <class T>
      void SwapBytes(T &val)
      {
         uchar *buffer = (uchar*)&val;
         for (uint32 i = 0; i<sizeof(val)/2; i++)
         {
            uchar b = buffer[i];

            buffer[i] = buffer[sizeof(val)/2 - i - 1];
            buffer[sizeof(val)/2 - i - 1] = b;
         }
      }
      /** @return if byte ordering is big endian */
      inline bool IsBigEndian()
      {
         union
         {
            sint32 int_val;
            char bytes[4];
         } data;
         data.int_val = 1;

         return (data.bytes[3] == 1);
      }
      /** @return if byte ordering is little endian */
      inline bool IsLittleEndian()
      {
         return !IsBigEndian();
      }

   }

   /**
   * @}
   */
}

#endif // _TC_UTIL_H_
