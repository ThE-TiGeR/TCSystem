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
//  $Id: TCStringStream.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifndef _TC_STRING_STREAM_H_
#define _TC_STRING_STREAM_H_

#include "TCStreamBase.h"

#include <string>

namespace TC
{
namespace Impl
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
    * @brief Stream class for reading or writing data from/into a string
    *
    * Just implements the writing and reading of bytes
    * the rest is done in class StreamBase
    */
   class TC_DLL_LOCAL StringStream: public StreamBase
   {
   public:
      /**
       * Construct an TC::StringStream object
       */
      StringStream(CodecPtr codec, std::string& string);
      /** destruct an TC::StringStream object */
      virtual ~StringStream();

      virtual bool SetPosition(sint32, StreamPosition pos);
      virtual uint32 GetPosition() const;

      virtual uint32 ReadBytes(uint32 nBytes, void *bytes);
      virtual uint32 WriteBytes(uint32 nBytes, const void *bytes);

   private:
      /** An list arry holding the data */
      std::string& m_string;
      /** current position where to read or write into the memory */
      std::string::size_type m_string_position;
   };

   /**
    * @}
    */

   } // namespace Impl
} // namespace TC

#endif // _TC_STRING_STREAM_H_
