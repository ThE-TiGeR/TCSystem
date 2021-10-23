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
//  $Id: TCStringStream.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifndef _TC_STRING_STREAM_H_
#define _TC_STRING_STREAM_H_

#include "TCNonCopyable.h"
#include "TCStreamBase.h"

#include <string>

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
    * @brief Stream class for reading or writing data from/into a string
    *
    * Just implements the writing and reading of bytes
    * the rest is done in class StreamBase
    */
   class TC_DLL_LOCAL StringStream: public StreamBase, protected NonCopyAble
   {
   public:
      /**
       * Construct an tc::StringStream object
       */
      StringStream(CodecPtr codec, std::string& string);
      /** destruct an tc::StringStream object */
      virtual ~StringStream();

      virtual StreamPtr Clone();

      virtual bool SetPosition(int64_t, Position pos);
      virtual uint64_t GetPosition() const;

      virtual uint64_t ReadBytes(uint64_t nBytes, void *bytes);
      virtual uint64_t WriteBytes(uint64_t nBytes, const void *bytes);

   private:
      /** A string holding the data */
      std::string& m_string;
      /** current position where to read or write into the memory */
      std::string::size_type m_string_position;
   };

   /**
    * @}
    */

   } // namespace imp
} // namespace tc

#endif // _TC_STRING_STREAM_H_
