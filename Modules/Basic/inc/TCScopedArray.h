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
#ifndef _TC_SCOPED_ARRAY_H_
#define _TC_SCOPED_ARRAY_H_

#include "TCTypes.h"
#include "TCDelete.h"

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * This header file provides the definition of the class tc::ScopedArray.
    */

   /**
    * A class whose instances act like arrays and delete the data when the instance
    * goes out of scope.
    *
    * tc::ScopedArray mimics a built-in pointer except that it guarantees deletion
    * of the object pointed to on destruction of the tc::ScopedArray
    * tc::ScopedArray is a simple solution for simple needs;
    * use tc::SharedPtr or std::auto_ptr if your needs are more complex.
    */
   template < class DATA_TYPE, class DELETER=CheckedArrayDelete >
   class ScopedArray
   {
   private:
      typedef ScopedArray<DATA_TYPE> ThisType;

   public:
      /** Construct the object pointing to a empty array */
      ScopedArray():m_array(0) {}
      /** Construct the object pointing to a specified array */
      explicit ScopedArray(DATA_TYPE* ptr):m_array(ptr) {}

      /** Destructor deletes the pointer */
      ~ScopedArray() { DELETER()(m_array);}

      /** Get element at specified index */
      inline const DATA_TYPE& operator[](uint32 index) const { return m_array[index];}
      /** Get element at specified index */
      inline DATA_TYPE& operator[](uint32 index) { return m_array[index]; }

      typedef DATA_TYPE* ThisType::*unspecified_bool_type;
      /** User define conversion for if like "if (pointer)" */
      operator unspecified_bool_type() const
      {
         return m_array == 0? 0: &ThisType::m_array;
      }
      /** User define conversion for if like "if (!pointer)" */
      inline bool operator !() const { return m_array == 0; }

      /** automatic conversion to pointer */
      operator DATA_TYPE* () {return m_array;}
      /** automatic conversion to const pointer */
      operator const DATA_TYPE* () const {return m_array;}
   private:
      DATA_TYPE* m_array;
   };

   /**
    *  @}
    */

} // namespace tc

#endif // _TC_SCOPED_ARRAY_H_
