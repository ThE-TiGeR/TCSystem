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
#ifndef _TC_WEAK_PTR_H
#define _TC_WEAK_PTR_H

#include "TCSharedPtr.h"

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * This header file provides the definition of the class tc::WeakPtr.
    *
    * @author Thomas Goessler
    */

   /**
    * @brief The WeakPtr class template stores a "weak reference" to an object that's already
    * managed by a SharedPtr.
    *
    * To access the object, a WeakPtr can be converted to a SharedPtr using the SharedPtr
    * constructor or the member function Lock. When the last SharedPtr to the object goes away
    * and the object is deleted, the attempt to obtain a SharedPtr from the WeakPtr instances
    * that refer to the deleted object will fail: WeakPtr::Lock will return an empty SharedPtr.
    *
    * The class template is parameterized on PTR, the type of the object pointed to.
    *
    * @author Thomas Goessler
    */
   template<class T>
   class WeakPtr
   {
   public:
      /** @brief null pointer construction */
      WeakPtr():m_ptr(0), m_count() {}

      /** @brief Copy constructor */
      template<class M_PTR>
      WeakPtr(const WeakPtr<M_PTR>& r):m_ptr(r.Lock().m_ptr), m_count(r.m_count) {}

      /** @brief Construct a weak pointer out of an shared pointer */
      template<class M_PTR>
      WeakPtr(const SharedPtr<M_PTR>&  r):m_ptr(r.m_ptr), m_count(r.m_count) {}

      /** @brief Get the shared pointer of this weak pointer */
      SharedPtr<T> Lock() const { return IsExpired() ? SharedPtr<T>(): SharedPtr<T>(*this); }

      /** @brief Get how many shared pointer point to the same pointer */
      long GetUseCount() const { return m_count.GetUseCount(); }

      /** @brief True if the the pointer pointing to is already deleted */
      bool IsExpired() const { return m_count.GetUseCount() == 0;}

      /** @brief Let the weak pointer point to a null pointer */
      void Reset() { WeakPtr<T> new_ptr; new_ptr.Swap(*this); }

      /** @brief Swap two weak pointers */
      void Swap(WeakPtr<T>& other) { std::swap(m_ptr, other.m_ptr); m_count.Swap(other.m_count); }

      /** @brief Check if a weak pointer is equal with another */
      template <class N_PTR>
      bool Equal(const WeakPtr<N_PTR>& src) const { return m_ptr == src.m_ptr; }

      /** @brief Check if a weak pointer is less then another */
      template<class M_PTR>
      bool Less(const WeakPtr<M_PTR>& src) const { return m_count < src.m_count; }

   private:
      T* m_ptr;                // contained pointer
      WeakPtrCount m_count;  // reference counter

      template<class M_PTR> friend class WeakPtr;
      template<class M_PTR> friend class SharedPtr;
   };

   /** equal operator with another weak pointer */
   template <class M_PTR, class N_PTR>
   bool operator==(const WeakPtr<M_PTR>& a, const WeakPtr<N_PTR>& b) { return a.Equal(b); }

   /** not equal operator with another weak pointer */
   template <class M_PTR, class N_PTR>
   bool operator!=(const WeakPtr<M_PTR>& a, const WeakPtr<N_PTR>& b) { return !a.Equal(b); }

   /** less operator with another weak pointer */
   template <class M_PTR, class N_PTR>
   bool operator<(const WeakPtr<M_PTR>& a, const WeakPtr<N_PTR>& b) { return a.Less(b); }

   /**
    * @}
    */

}

#endif  // _TC_WEAK_PTR_H
