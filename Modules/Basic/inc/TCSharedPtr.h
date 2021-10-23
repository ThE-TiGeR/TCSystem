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
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCSharedPtr.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifndef _TC_SHARED_PTR_H_
#define _TC_SHARED_PTR_H_

#include "TCTypes.h"
#include "TCDelete.h"
#include "TCSharedPtrImpl.h"

namespace tc
{
   template<class T> class SharedPtr;
   template<class T> class SharedPtrFromThis;
   template<class T> class WeakPtr;

   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * This header file provides the definition of the class tc::SharedPtr
    *
    * @author Thomas Goessler
    */

   template<class T> struct SharedPtrTraits { typedef T& Reference; };
   template<> struct SharedPtrTraits<void> { typedef void Reference; };
   template<> struct SharedPtrTraits<const void> { typedef void Reference; };
   template<> struct SharedPtrTraits<const volatile void> { typedef void Reference; };

   template <class T1, class T2> 
   void inline InitializeSharedFromThis(SharedPtr<T1>* ptr, SharedPtrFromThis<T2>* sft)
   {
      sft->InternalInitSharedPtrFromThis(*ptr);
   }
   void inline InitializeSharedFromThis(...)
   {
   }

   /**
    * A class whose instances act like pointers that manage their reference count automatically.
    *
    * This shared pointer avoids destruction of an object in use.
    * It can be used just like a pointer.
    * First Template argument is the data type of the pointer.
    *
    * Notes:
    * Always pass shared pointers by value. Never create two smart pointers to the same regular
    * pointer. Once a smart pointer is set to equal a pointer it owns that pointer, and will
    * delete it when no more shared pointers point to the same ref.
    *
    * @author Thomas Goessler
    */
   template <class T>
   class SharedPtr
   {
   private:
      typedef SharedPtr<T> ThisType;

   public:
      typedef T  ValueType;
      typedef T* PointerType;
      typedef typename SharedPtrTraits<T>::Reference ReferenceType;

   public:
      /** Construct the object pointing to a 0 object */
      SharedPtr():m_ptr(0), m_count() {}

      /** Construct the object pointing to a specified object */
      template <class N_PTR>
      explicit SharedPtr(N_PTR* ptr):m_ptr(ptr), m_count(ptr, CheckedDelete()) { InitializeSharedFromThis(this, ptr); }

      /** Construct the object pointing to a specified object + deleter */
      template <class N_PTR, class DELETER>
      SharedPtr(N_PTR* ptr, DELETER deleter) : m_ptr(ptr), m_count(ptr, deleter) { InitializeSharedFromThis(this, ptr); }

      /** Construct the object pointing to a specified weak pointer */
      template<class N_PTR>
      explicit SharedPtr(const WeakPtr<N_PTR>& ptr):m_ptr(ptr.m_ptr), m_count(ptr.m_count) {}

      /** automatic downcast if possible */
      template <class N_PTR>
      SharedPtr(const SharedPtr<N_PTR>& src):m_ptr(src.m_ptr), m_count(src.m_count) {}

      /** User define conversion to the const Template pointer type */
      const T* operator->() const { return m_ptr;}
      /** User define conversion to the const Template pointer type */
      T* operator->() { return m_ptr; }

      typedef T* ThisType::*unspecified_bool_type;
      /** User define conversion for if like "if ( smartPointer)" */
      operator unspecified_bool_type() const { return m_ptr == 0 ? 0 : &ThisType::m_ptr; }
      /** User define conversion for if like "if ( !smartPointer)" */
      bool operator !() const { return m_ptr == 0; }

      typedef void* unspecified_pointer_type;
      /** equal operator with a unspecified pointer type */
      friend bool operator==(const SharedPtr<T>& a, unspecified_pointer_type b) {return a.m_ptr == b;}
      /** not equal operator with a unspecified pointer type */
      friend bool operator!=(const SharedPtr<T>& a, unspecified_pointer_type b) {return a.m_ptr != b;}

      /** @brief Get how many shared pointer point to the same pointer */
      uint32_t GetUseCount() const {return m_count.GetUseCount();}

      /** @brief return true if there exists only one instance of this shared pointer */
      bool IsUnique() const {return m_count.GetUseCount() == 1;}

      /** @brief Let the shared pointer point to a null pointer */
      void Reset() { SharedPtr<T> new_ptr; new_ptr.Swap(*this); }
      /** @brief Let the shared pointer point to the new given pointer */
      template<class N_PTR>
      void Reset(N_PTR* p) { SharedPtr<N_PTR> new_ptr(p); new_ptr.Swap(*this); }
      /** @brief Let the shared pointer point to the new given pointer and its deleter*/
      template <class N_PTR, class DELETER>
      void Reset(N_PTR* p, DELETER deleter) { SharedPtr<N_PTR> new_ptr(p, deleter); new_ptr.Swap(*this); }

      /** @brief Swap two smart pointers */
      void Swap(SharedPtr<T>& a) { std::swap(m_ptr, a.m_ptr); m_count.Swap(a.m_count); }

      /** @brief Check if a smart pointer is equal with another */
      template <class N_PTR>
      bool Equal(const SharedPtr<N_PTR>& src) const { return m_ptr == src.m_ptr; }

      /** @brief Check if a smart pointer is less then another */
      template <class N_PTR>
      bool Less(const SharedPtr<N_PTR>& src) const { return m_count < src.m_count; }

#ifdef TC_SHARED_PTR_TEST
      PointerType GetPtr() {return m_ptr;}
      ReferenceType operator*() {return *m_ptr;}
#endif

      /** Dynamic cast method for Smart pointer */
      template <class N_PTR>
      static ThisType DynamicCast(const SharedPtr<N_PTR>& src)
      {
         ThisType dest;
         dest.m_ptr = dynamic_cast<T*>(src.m_ptr);
         if (dest.m_ptr)
         {
            dest.m_count = src.m_count;
         }
         return dest;
      }

      /** Static cast method for Smart pointer */
      template <class N_PTR>
      static ThisType StaticCast(const SharedPtr<N_PTR>& src)
      {
         ThisType dest;
         dest.m_ptr   = static_cast<T*>(src.m_ptr);
         dest.m_count = src.m_count;

         return dest;
      }

      /** Static cast method for Smart pointer */
      template <class N_PTR>
      static ThisType ConstCast(const SharedPtr<N_PTR>& src)
      {
         ThisType dest;
         dest.m_ptr = const_cast<T*>(src.m_ptr);
         dest.m_count = src.m_count;

         return dest;
      }

#ifndef TC_NO_FRIEND_TEMPLATES
   private:
      template <class N_PTR> friend class SharedPtr;
      template <class N_PTR> friend class WeakPtr;
#endif

      T* m_ptr;
      SharedPtrCount m_count;

   };

   /** equal operator with another smart pointer */
   template <class M_PTR, class N_PTR>
   bool operator==(const SharedPtr<M_PTR>& a, const SharedPtr<N_PTR>& b) { return a.Equal(b); }

   /** not equal operator with another smart pointer */
   template <class M_PTR, class N_PTR>
   bool operator!=(const SharedPtr<M_PTR>& a, const SharedPtr<N_PTR>& b) { return !a.Equal(b); }

   /** less operator with another smart pointer */
   template <class M_PTR, class N_PTR>
   bool operator<(const SharedPtr<M_PTR>& a, const SharedPtr<N_PTR>& b) { return a.Less(b); }

   /**
    * @}
    */

} // namespace tc

#endif // _TC_SHARED_PTR_H_

