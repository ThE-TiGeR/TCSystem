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
//  $Id: TCMTLockable.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TC_MT_LOCKABLE_H_
#define _TC_MT_LOCKABLE_H_

#include "TCNonCopyable.h"
#include "TCMTFactory.h"

namespace tc
{
namespace multi_threading
{
   /**
    * @addtogroup TC_MT
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of tc::multi_threading::ObjectLevelLockable and tc::multi_threading::ClassLevelLockable
    * @author Thomas Goessler
    */

   /**
    * This template offers the basic mechanism for an object level locking mechanism.
    */
   template <class CLASS_TO_LOCK>
   class ObjectLevelLockable: protected NonCopyAble
   {
   public:
      ObjectLevelLockable()
      {
         m_mutex = factory::CreateMutex();
      }
      ~ObjectLevelLockable()
      {
         m_mutex = MutexPtr();
      }

      /** @short Lock the object */
      bool Lock() const
      {
         return m_mutex->Lock();
      }

      /** @short Unlock the object */
      bool UnLock() const
      {
         return m_mutex->UnLock();
      }

      /** @short Provides a typedef for locking this object */
      typedef LockerPtr< const CLASS_TO_LOCK* > Locker;

   private:
      mutable MutexPtr m_mutex;
   };

   /**
    * This template offers the basic mechanism for an class level locking mechanism.
    *
    */
   template <class CLASS_TO_LOCK>
   class ClassLevelLockable: protected NonCopyAble
   {
   public:
      /**
       * @short Helper class for locking the whole class
       * This nested class is used for locking and unlocking the given object by the
       * use of the constructor/destructor of the object
       */
      class Locker: protected NonCopyAble
      {
      public:
         Locker()
         {
            m_mutex->Lock();
         }
         ~Locker()
         {
            m_mutex->UnLock();
         }
      };

      /** @short Lock the class */
      bool Lock()
      {
         return m_mutex->Lock();
      }

      /** @short UnLock the class */
      bool UnLock()
      {
         return m_mutex->UnLock();
      }

   private:
      static MutexPtr m_mutex;

      friend class Locker;
   };

   template<class CLASS_TO_LOCK>
   MutexPtr ClassLevelLockable<CLASS_TO_LOCK>::m_mutex = factory::CreateMutex();

   /**
    * @}
    */
} // namespace multi_threading
} // namespace tc

#endif // _TC_MT_LOCKABLE_H_
