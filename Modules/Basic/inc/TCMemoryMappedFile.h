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
#ifndef _TC_MEMORY_MAPPED_FILE_H_
#define _TC_MEMORY_MAPPED_FILE_H_

#include "TCSharedPtr.h"

#include <string>

namespace TC
{
   /**
   * @addtogroup TC_BASE
   * @{
   */

   /**
   * @file
   * This header file provides the definition of the class TC::MemoryMappedFile.
   *
   * @author Thomas Goessler
   */

   class MemoryMappedFile;
   typedef SharedPtr<MemoryMappedFile> MemoryMappedFilePtr;

   /**
   * The class TC::MemoryMappedFile is a simple file opener by mapping the file
   * into the memory.
   *
   * @author Thomas Goessler
   */
   class MemoryMappedFile
   {
   public:
      /** @return Returns the size of the memory mapped file*/
      virtual uint32 GetSize() const = 0;

      /** @return Returns a pointer to the mapped data. If read only then 0 */
      virtual uint8* GetData() = 0;

      /** @return Pointer to the actual data if successful */
      virtual const uint8* GetReadOnlyData() const = 0;

      /** Closes the memory mapping. The data is then no more accessible */
      virtual ~MemoryMappedFile() {}
   };

   /**
   *  @}
   */
}

#endif // _TC_MEMORY_MAPPED_FILE_H_
