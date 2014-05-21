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
//  $Id: $
//*******************************************************************************
#ifndef _TC_SHARED_PTR_FROM_THIS_H_
#define _TC_SHARED_PTR_FROM_THIS_H_

#include "TCWeakPtr.h"

#include "TCNewEnable.h"

namespace tc
{
   /**
   * @addtogroup TC_BASE
   * @{
   */

   /**
   * @file
   * This header file provides the definition of the class tc::SharedPtrFromThis.
   */

   /**
   * A class that provides derived classes with access to a SharedPtr to the current instance.
   * This can for example be passed to observers as an indication of the source of an event.
   *
   * @author Thomas Goessler
   */

   template<class T> class SharedPtr;
   template<class T> class WeakPtr;

   template<class T>
   class SharedPtrFromThis
   {
   public:
      /** Initializes the internal WeakPtr. Is called when the first SharedPtr to the instance is created. */
      void InternalInitSharedPtrFromThis(WeakPtr<T> p) const
      {
         if (m_this.IsExpired())
         {
            m_this = p;
         }
      }

   protected:
      /** Retrieves a valid SharedPtr to the current instance. */
      SharedPtr<T> GetSharedPtrFromThis()
      {
         SharedPtr<T> ret = m_this.Lock();
         return ret;
      }

      /** Retrieves a valid SharedPtr to the current instance if used on a const object. */
      SharedPtr<const T> GetSharedPtrFromThis() const
      {
         SharedPtr<const T> ret = m_this.Lock();
         return ret;
      }

   private:
      mutable WeakPtr<T> m_this;
   };

   /**
   * @}
   */

} // namespace tc

#include "TCNewDisable.h"

#endif
