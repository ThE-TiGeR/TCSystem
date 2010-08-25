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
//  $Id: TCSharedPtrImpl.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifndef _TC_SHARED_PTR_IMP_H_
#define _TC_SHARED_PTR_IMP_H_

#include "TCInterlocked.h"

#include <memory>    // for std::auto_ptr
#include <algorithm> // for std::swap

#include "TCNewEnable.h"

namespace TC
{
   class WeakPtrCount;
   class SharedPtrCount;

   /**
    * @addtogroup TC_BASE_IMPL
    * @{
    */

   /**
    * @file
    * This header file provides the definition internal classes used by TC::SharedPtr.
    *
    * These classes are:
    *  - TC::Impl::SharedPtrCountBase
    *  - TC::Impl::SharedPtrCountType
    *  - TC::SharedPtrCount
    *  - TC::WeakPtrCount
    */

   /** @brief Includes implementation classes from the namespace TC */
   namespace Impl
   {
      /** Base class of an shared counter implementation used by SharedPtrCount */
      class SharedPtrCountBase
      {
      protected:
         virtual ~SharedPtrCountBase() {}
         virtual void DeletePtr() = 0;

         /** Construct reference counter with 1 */
         SharedPtrCountBase()
             :m_count(1),
            m_weak_count(1)
         {
         }

      private:
         /** @brief Add a new reference to this counter */
         inline void AddReference()
         {
            Interlocked::Increment(m_count);
         }
         /**
          * @brief Release the reference of this counter
          * If last reference it deletes itself
          */
         inline void ReleaseReference()
         {
            if (Interlocked::Decrement(m_count) == 0)
            {
               DeletePtr();
               ReleaseWeakReference();
            }
         }
         /** @brief Get current usage count */
         inline uint32 GetUseCount() const { return m_count;}

         /** @brief Add a new weak reference to this counter */
         inline void AddWeakReference()
         {
            Interlocked::Increment(m_weak_count);
         }
         /**
          * @brief Release the weak reference of this counter
          * If last reference it deletes itself
          */
         inline void ReleaseWeakReference()
         {
            if (Interlocked::Decrement(m_weak_count) == 0)
            {
               Destroy();
            }
         }

         /**
         * @brief function which destroys the counter
         * and should be called when no more references
         */
         inline void Destroy() { delete this;}

      private:
         /** @brief The counting variable */
         Interlocked::Type m_count;
         /** @brief The weak pointer counting variable */
         Interlocked::Type m_weak_count;

         friend class TC::SharedPtrCount;
         friend class TC::WeakPtrCount;
      };

      /**
      * Shared counter implementation for specified pointer type
      * Holds also the pointer which gets also deleted when the counter is deleted
      */
      template <class T, class DELETER>
      class SharedPtrCountType: protected SharedPtrCountBase
      {
      protected:
         /** Construct reference counter with 1 */
         SharedPtrCountType(T* ptr, DELETER deleter)
            :m_ptr(ptr),
            m_deleter(deleter)
         {
         }
         
         virtual void DeletePtr()
         {
            m_deleter(m_ptr);
         }
      private:
         /** @brief The real pointer */
         T* m_ptr;
         /** @brief  The delete function for deleting the pointer */
         DELETER m_deleter;

         friend class TC::SharedPtrCount;
      };

   } // namespace Impl

   /** Reference counter class for TC::SharedPtr */
   class SharedPtrCount
   {
   public:
      /** Construct without an internal counter */
      SharedPtrCount():m_count(0) {}

      /** Construct with pointer and creates an pointer specific counter */
      template <class PTR_TYPE, class DELETER>
      SharedPtrCount(PTR_TYPE* ptr, DELETER deleter)
      {
         m_count = new Impl::SharedPtrCountType<PTR_TYPE, DELETER>(ptr, deleter);
      }

      /** Construct with auto pointer and creates an pointer specific counter */
      template <class PTR_TYPE, class DELETER>
      SharedPtrCount(std::auto_ptr<PTR_TYPE>& ptr, DELETER deleter)
      {
         m_count = new Impl::SharedPtrCountType<PTR_TYPE, DELETER>(ptr.get(), deleter);
         ptr.release();
      }

      /** Copy constructor */
      explicit SharedPtrCount(const SharedPtrCount& count)
         :m_count(count.m_count)
      {
         if (m_count) m_count->AddReference();
      }

      /** Copy constructor with a weak ptr count*/
      explicit SharedPtrCount(const WeakPtrCount& count);

      /** Destructor releases the internal counter */
      ~SharedPtrCount()
      {
         if (m_count)
         {
            m_count->ReleaseReference();
            m_count = 0;
         }
      }

      /**
      * Assignment operator releases the old internal
      * counter and adds a new reference to the new counter
      */
      SharedPtrCount& operator=(const SharedPtrCount& count)
      {
         if (count.m_count != m_count)
         {
            if (count.m_count) count.m_count->AddReference();
            if (m_count) m_count->ReleaseReference();
            m_count = count.m_count;
         }

         return *this;
      }

      /** @brief Get current use count */
      uint32 GetUseCount() const
      {
         return m_count ? m_count->GetUseCount() : 0;
      }

      /** @brief Swap two shared counters */
      void Swap(SharedPtrCount& a)
      {
         std::swap(m_count, a.m_count);
      }

      /** @brief Compare two counters if equal */
      friend inline bool operator==(const SharedPtrCount& a, const SharedPtrCount& b)
      {
         return a.m_count == b.m_count;
      }

      /** @brief Compare two counters if less */
      friend inline bool operator<(const SharedPtrCount& a, const SharedPtrCount& b)
      {
         return a.m_count < b.m_count;;
      }

   private:
      /** @brief Pointer to the counting object */
      Impl::SharedPtrCountBase* m_count;
      friend class TC::WeakPtrCount;
   };

   /** Reference counter class for TCWeakPtr */
   class WeakPtrCount
   {
   public:
      WeakPtrCount() :m_count(0) {}

      /** @brief Copy constructor */
      explicit WeakPtrCount(const WeakPtrCount& r) :m_count(r.m_count)
      {
         if (m_count != 0) m_count->AddWeakReference();
      }

      /** @brief Construct a WeakPtrCount out of an SharedPtrCount */
      explicit WeakPtrCount(const SharedPtrCount& r) :m_count(r.m_count)
      {
         if (m_count != 0) m_count->AddWeakReference();
      }

      /** Destructor releases the internal weak counter */
      ~WeakPtrCount()
      {
         if (m_count != 0)
         {
            m_count->ReleaseWeakReference();
            m_count = 0;
         }
      }

      /** @brief Assignment operator with an SharedPtrCount */
      WeakPtrCount& operator=(const SharedPtrCount& r)
      {
         if (r.m_count != 0) r.m_count->AddWeakReference();
         if (m_count   != 0) m_count->ReleaseWeakReference();
         m_count = r.m_count;

         return *this;
      }

      /** @brief Assignment operator with an other WeakPtrCount */
      WeakPtrCount& operator=(const WeakPtrCount& r)
      {
         if (r.m_count != 0) r.m_count->AddWeakReference();
         if (m_count   != 0) m_count->ReleaseWeakReference();
         m_count = r.m_count;

         return *this;
      }

      /** @brief Get current use count */
      uint32 GetUseCount() const
      {
         return m_count != 0 ? m_count->GetUseCount(): 0;
      }

      /** @brief Swapt two weak counters */
      void Swap(WeakPtrCount& a)
      {
         std::swap(m_count, a.m_count);
      }

      /** @brief Compare two counters if equal */
      friend inline bool operator==(const WeakPtrCount& a, const WeakPtrCount& b)
      {
         return a.m_count == b.m_count;
      }

      /** @brief Compare two counters if less */
      friend inline bool operator<(const WeakPtrCount& a, const WeakPtrCount& b)
      {
         return a.m_count < b.m_count;
      }

   private:
      /** @brief Pointer to the counting object */
      Impl::SharedPtrCountBase* m_count;
      friend class TC::SharedPtrCount;
   };


   /**
    * @}
    */

   inline SharedPtrCount::SharedPtrCount(const WeakPtrCount& count)
      :m_count(count.m_count)
   {
      if (m_count) m_count->AddReference();
   }

} // namespace TC

#include "TCNewDisable.h"

#endif // _TC_SHARED_PTR_IMP_H_

