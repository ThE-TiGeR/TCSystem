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
//  $Id: TCMTLock.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TC_MT_LOCK_H_
#define _TC_MT_LOCK_H_

#include "TCNonCopyable.h"

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
    * @brief This file provides the definition of tc::multi_threading::LockerRef and tc::multi_threading::LockerPtr
    * @author Thomas Goessler
    */

   /**
    * @short Automatic locking class for references
    * Class for automatic locking of a critical objects
    * (like tc::multi_threading::Mutex,tc::multi_threading::ClassLevelLockable, tc::multi_threading::ObjectLevelLockable...).
    * The class has to provide the methods Lock and UnLock
    *
    * When constructing a Locker it automatically locks the critical object.
    * This can be used when you have a methode which locks a critical object on
    * enter and UnLocks it when leaving a method.
    *
    * As an alternative to:
    * @code
    * void Test()
    * {
    *    critical.Lock();
    *    .....
    *    .....
    *    .....
    *    critical.UnLock();
    * }
    * @endcode
    *
    * you can use a single instance of the Locker class:
    * @code
    * void Test()
    * {
    *    tc::multi_threading::Locker<type> lock(critical);
    *    ....
    *    ....
    *    ....
    * }
    * @endcode
    *
    * This has the advantage that critical.UnLock() will be called
    * automatically when an exception is thrown or the method is left.
    *
    * @author Thomas Goessler
    */
   template <class T>
   class LockerRef: protected NonCopyAble
   {
   public:
       /**
        * @short Constructs a Locker object.
        * The critical object is automatically locked.
        *
        * @param critical The reference of the object locked by this constructor.
        */
       LockerRef(T& critical)
       :m_critical(critical)
       {
           m_critical.Lock();
       }

       /**
        *  @short Destructs the Locker object and unlocks the critical section
        *  associated with this object.
        */
       ~LockerRef()
       {
           m_critical.UnLock();
       }

   private:
       /** Reference to the critical section used by this object. */
       T& m_critical;
   };

   /**
    * @brief Locking object for Pointers
    * @see tc::multi_threading::LockRef
    */
   template <class T>
   class LockerPtr: protected NonCopyAble
   {
   public:
       /**
        * @short Constructs a Locker object.
        * The critical object is automatically locked.
        *
        * @param critical The reference of the object locked by this constructor.
        */
       LockerPtr(T critical)
       :m_critical(critical)
       {
           m_critical->Lock();
       }

       /**
        *  @short Destructs the Locker object and unlocks the critical section
        *  associated with this object.
        */
       ~LockerPtr()
       {
           m_critical->UnLock();
       }

   private:
       /** Pointer to the critical section used by this object. */
       T m_critical;
   };

   /**
    * @}
    */

} // namespace multi_threading
} // namespace tc


#endif // _TC_MT_LOCK_H_
