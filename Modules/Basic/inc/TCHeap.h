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
//  $Id$
//*******************************************************************************
#ifndef _TC_HEAP_H_
#define _TC_HEAP_H_

#include "TCException.h"

namespace tc
{
   /**
   * @addtogroup TC_BASE_DEBUG
   * @{
   */

   /**
   * @file
   * @brief This file provides the definition of the interface tc::Heap
   *
   * @author Thomas Goessler
   */

   /** 
   * @brief Heap interface definition
   */
   class Heap
   {
   public:
      /**
      * @brief Allocate a block of size bytes
      * @return The address of the block on success; std::bad_alloc on failure
      */
      virtual void* Alloc(uint32_t size) = 0;
      /**
      * @brief Allocate a block of size bytes
      * If The heap supports memory leak detection it should store for each allocation
      * file name and line number
      * @return The address of the block on success; std::bad_alloc on failure
      */
      virtual void* Alloc(uint32_t size, const char* file_name, uint32_t line_numer) = 0;
      /**
      * @brief ReAllocate a block of size bytes
      * @return The address of the block on success; std::bad_alloc on failure
      */
      virtual void* ReAlloc(void* ptr, uint32_t size) = 0;
      /**
      * @brief ReAllocate a block of size bytes
      * If The heap supports memory leak detection it should store for each allocation
      * file name and line number
      * @return The address of the block on success; std::bad_alloc on failure
      */
      virtual void* ReAlloc(void* ptr, uint32_t size, const char* file_name, uint32_t line_numer) = 0;
      /**
      * @brief Free an allocated block
      * The pointer is only freed if it belongs to this heap
      */
      virtual void Free(void* memory) = 0;

      /**
      * @brief Check if this memory pointer is from this heap
      * @return true if memory is from this heap
      */
      virtual bool IsAHeapPointer(void* memory) const = 0;

      virtual ~Heap(){}

   protected:
      class Mutex;
   };

   /**
    * @}
    */

}

#endif // _TC_HEAP_H_
