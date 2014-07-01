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
#ifndef _TC_DEBUG_HEAP_H_
#define _TC_DEBUG_HEAP_H_

#include "TCHeap.h"
#include "TCNonCopyable.h"
#include "TCStream.h"

namespace tc
{
   /**
   * @addtogroup TC_BASE_DEBUG
   * @{
   */

   /**
   * @file
   * @brief This file provides the definition of the interface tc::DebugHeap
   *
   * @author Thomas Goessler
   */

    class DebugHeap: public Heap, protected NonCopyAble
    {
    public:
        DebugHeap(Heap& heap);
        ~DebugHeap();

        virtual void* Alloc(uint32_t size);
        virtual void* Alloc(uint32_t size, const char* file_name, uint32_t line_numer);
        virtual void* ReAlloc(void* memory, uint32_t size);
        virtual void* ReAlloc(void* memory, uint32_t size, const char* file_name, uint32_t line_numer);
        virtual void Free(void* memory);
        virtual bool IsAHeapPointer(void* memory) const;


        bool PrintMemoryLeaks(std::ostream& stream) const;

    private:
        Heap& m_heap;
         
        class AllocList;
        AllocList* m_alloc_list;
    };

    /** @} */
}

#endif // _TC_DEBUG_HEAP_H_
