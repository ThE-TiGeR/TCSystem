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
#ifndef _TC_FACTORY_H_
#define _TC_FACTORY_H_

#include "TCCodec.h"
#include "TCMemoryMappedFile.h"
#include "TCRng.h"
#include "TCStlTypes.h"
#include "TCStream.h"

#include <vector>
#include <cstdio>

namespace TC
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * This header file provides the definition of the namespace TC::Factory
    * @author Thomas Goessler
    */

   /**
    * @brief Factory for creating TC objects
    */
   namespace Factory
   {
      /**
       * @brief Create an ascii codec
       * @return Shared pointer to the codec
       */
      TCBASE_API CodecPtr CreateAsciiCodec();
      /**
       * @brief Create an binary codec without byte order conversion
       * @return Shared pointer to the codec
       */
      TCBASE_API CodecPtr CreateBinaryCodec();
      /**
      * @brief Create an binary codec (little endian)
      * @return Shared pointer to the codec
      */
      TCBASE_API CodecPtr CreateLittleEndianBinaryCodec();
      /**
      * @brief Create an binary codec (big endian)
      * @return Shared pointer to the codec
      */
      TCBASE_API CodecPtr CreateBigEndianBinaryCodec();

      /**
       * @brief Create an file stream
       * @param stream    Pointer to an open file
       * @param direction Direction of the stream see TC::Stream::StreamDirection
       * @param codec     Shared pointer to a codec for encoding or decoding the data
       * @return Shared pointer to the stream
       */
      TCBASE_API StreamPtr CreateFileStream(std::FILE* stream, 
                                            Stream::StreamDirection direction, 
                                            CodecPtr codec);
      /**
       * @brief Create an file stream
       * @param fileName  name of the file to open
       * @param direction Direction of the stream see TC::Stream::StreamDirection
       * @param codec     Shared pointer to a codec for encoding or decoding the data
       * @return Shared pointer to the stream
       */
      TCBASE_API StreamPtr CreateFileStream(const std::string &fileName,
                                            Stream::StreamDirection direction,
                                            CodecPtr codec);

      /**
       * @brief Create an gz compressed file stream
       * @param fileName  name of the file to open
       * @param direction Direction of the stream see TC::Stream::StreamDirection
       * @param codec     Shared pointer to a codec for encoding or decoding the data
       * @return Shared pointer to the stream
       */
      TCBASE_API StreamPtr CreateGzFileStream(const std::string &fileName,
                                            Stream::StreamDirection direction,
                                            CodecPtr codec);

      /**
       * @brief Create an bz2 compressed file stream
       * @param fileName  name of the file to open
       * @param direction Direction of the stream see TC::Stream::StreamDirection
       * @param codec     Shared pointer to a codec for encoding or decoding the data
       * @return Shared pointer to the stream
       */
      TCBASE_API StreamPtr CreateBz2FileStream(const std::string &fileName,
                                            Stream::StreamDirection direction,
                                            CodecPtr codec);

      /**
       * @brief Create an memory stream
       * @param codec Shared pointer to a codec for encoding or decoding the data
       * @param memory Buffer into which the stream should read or write
       * @return Shared pointer to the stream
       */
      TCBASE_API StreamPtr CreateMemoryStream(CodecPtr codec, ByteVector& memory);

      /**
      * @brief Create an string stream
      * @param memory Buffer into which the stream should read or write
      * @return Shared pointer to the stream
      */
      TCBASE_API StreamPtr CreateStringStream(std::string& memory);

      /**
       * @brief Create a stream which writes to standard out
       * @return Shared pointer to the stream
       */
      TCBASE_API StreamPtr CreateStdOutStream();

      /**
       * @brief Create a stream which writes to standard error
       * @return Shared pointer to the stream
       */
      TCBASE_API StreamPtr CreateStdErrorStream();

      /**
       * @brief Create an 69069 Rng object
       * @return Shared pointer to the Rng object
       */
      TCBASE_API RngPtr Create69069Rng();

      TCBASE_API MemoryMappedFilePtr CreateMemoryMappedFile(const std::string &file_name, bool read_only, uint32 size=0);
   } // namespace Factory

   /**
    * @}
    */

} // namespace TC

#endif // _TC_FACTORY_H_
