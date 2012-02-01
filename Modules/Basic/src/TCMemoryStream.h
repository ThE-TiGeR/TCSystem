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
#ifndef _TCMEMORY_STREAM_H_
#define _TCMEMORY_STREAM_H_

#include "TCStlTypes.h"
#include "TCStreamBase.h"

#include <vector>

namespace tc
{
namespace imp
{

   /**
    * @addtogroup TC_BASE_IO
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TCMemoryStream
    *
    * @author Thomas Goessler
    */

   /**
    * @brief Stream class for reading or writing data from/into memory
    *
    * Just implements the writing and reading of bytes
    * the rest is done in class TCStream
    */
   class TC_DLL_LOCAL MemoryStream: public StreamBase
   {
   public:
      /**
       * Construct an TCMemoryStream object
       */
      MemoryStream(CodecPtr codec, ByteVector& memory);
      /** destruct an TCMemoryStream object */
      virtual ~MemoryStream();

      virtual bool SetPosition(sint64, StreamPosition pos);
      virtual uint64 GetPosition() const;

      /**
       * methode for reading the bytes from the stream
       * @param nBytes number of bytes to read
       * @param bytes where to store the data which should be read
       */
      virtual uint64 ReadBytes(uint64 nBytes, void *bytes);
      /**
       * methode for writing the bytes to the stream
       * @param nBytes number of bytes to written
       * @param bytes where the bytes are stored which should be written
       */
      virtual uint64 WriteBytes(uint64 nBytes, const void *bytes);

   private:
      /** An list arry holding the data */
      ByteVector& m_memory;
      /** current position where to read or write into the memory */
      ByteVector::size_type m_memory_position;
   };

   /**
    * @}
    */

   } // namespace imp
} // namespace tc

#endif // _TCMEMORY_STREAM_H_
