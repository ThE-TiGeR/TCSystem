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
// see https://bitbucket.org/the____tiger/tcsystem for details.
// Copyright (C) 2003 - 2014 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCScopedPtr.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifndef _TC_SCOPED_POINTER_H_
#define _TC_SCOPED_POINTER_H_

#include "TCTypes.h"
#include "TCDelete.h"

#include <memory> // auto ptr

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * This header file provides the definition of the class tc::ScopedPtr.
    */

   /**
    * A class whose instances act like pointers and delete the pointer when the instance
    * goes out of scope.
    *
    * tc::ScopedPtr mimics a built-in pointer except that it guarantees deletion
    * of the object pointed to on destruction of the tc::ScopedPtr
    * tc::ScopedPtr is a simple solution for simple needs;
    * use tc::SharedPtr or std::auto_ptr if your needs are more complex.
    */
   template < class T, class DELETER=CheckedDelete >
   class ScopedPtr
   {
   private:
      typedef ScopedPtr<T> ThisType;

   public:
      /** Construct the object pointing to a 0 object */
      ScopedPtr():m_ptr(0) {}
      /** Construct the object pointing to a specified object */
      explicit ScopedPtr(T* ptr):m_ptr(ptr) {}
      /** Construct the object pointing to a specified std::auto_ptr */
      explicit ScopedPtr(std::auto_ptr<T> ptr):m_ptr(ptr.get()) {ptr.release();}

      /** Destructor deletes the pointer */
      ~ScopedPtr() { DELETER()(m_ptr);}

      /** User define conversion to the const Template pointer type */
      inline const T* operator->() const { return m_ptr;}
      /** User define conversion to the Template pointer type */
      inline T* operator->() { return m_ptr; }

      typedef T* ThisType::*unspecified_bool_type;
      /** User define conversion for if like "if (pointer)" */
      operator unspecified_bool_type() const
      {
         return m_ptr == 0? 0: &ThisType::m_ptr;
      }

      /** User define conversion for if like "if (!pointer)" */
      inline bool operator !() const { return m_ptr == 0; }

   private:
      T*    m_ptr;
   };

   /**
    *  @}
    */

} // namespace tc

#endif // _TC_SCOPED_POINTER_H_
