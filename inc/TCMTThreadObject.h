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

#ifndef _TC_MT_THREAD_OBJECT_H_
#define _TC_MT_THREAD_OBJECT_H_

#include "TCNonCopyable.h"
#include "TCSharedPtr.h"

namespace TC
{
namespace MT
{
   /**
    * @addtogroup TC_MT
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TC::MT::ThreadObject
    * @author Thomas Goessler
    */

   /**
    * @brief Base class of  object which are executed by a TCMT::Thread
    * When the thread gets started it executes the methode ThreadObject::Run
    */
   class ThreadObject: protected NonCopyAble
   {
   public:
      /**
       * @brief Needs to be implemented in a derived class
       * Gets executed when the thread gets started
       */
      virtual bool Run() = 0;

      virtual ~ThreadObject() {}
   };

   /** @brief Smart pointer to a thread object */
   typedef SharedPtr< ThreadObject > ThreadObjectPtr;

   /**
    * @}
    */

} // namespace MT
} // namespace TC


#endif // _TC_MT_THREAD_OBJECT_H_
