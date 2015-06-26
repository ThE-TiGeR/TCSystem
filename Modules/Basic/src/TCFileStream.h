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
//  $Id: TCFileStream.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifndef _TCFILE_STREAM_H_
#define _TCFILE_STREAM_H_

#include "TCStreamBase.h"

#include <cstdio>

namespace tc
{
namespace imp
{

   /**
    * @addtogroup TC_BASE_IO_IMPL
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TCFileStream
    *
    * @author Thomas Goessler
    */

   /**
    * @brief Class for reading/writing a file
    *
    * Just implements the writing and reading of bytes
    * the rest is done in TCStream
    */
   class TC_DLL_LOCAL FileStream: public StreamBase
   {
   public:
      /**
       * construct an TCAscciStreamObject with an istream
       * stream direction is set to stream_read
       * @param stream the pointer to the stream
       * @param direction defines how to open the file @see TCStream::stream direction
       */
      FileStream(std::FILE* stream, Direction direction, CodecPtr codec);
      /**
       * construct an TCAscciStreamObject with an filename and the stream direction
       * @param fileName name of the file which to open
       * @param direction defines how to open the file @see TCStream::stream direction
       */
      FileStream(const std::string &fileName, Direction direction, CodecPtr codec);

      virtual ~FileStream();

      /**
       * set a new stream with an istream
       * stream direction is set to stream_read
       * @param stream the pointer to the stream
       * @param direction defines how to the stream was opened @see TCStream::stream direction
       */
      void SetStream(std::FILE *stream, Direction direction);
      /**
       * set a new stream with an filename and the stream direction
       * @param fileName name of the file which to open
       * @param direction defines how to open the file @see TCStream::stream direction
       */
      void SetStream(const std::string &fileName, Direction direction);

      virtual StreamPtr Clone();
      virtual bool SetPosition(int64_t, Position pos);
      virtual uint64_t GetPosition() const;

      /**
       * methode for reading the bytes from the stream
       * @param nBytes number of bytes to read
       * @param bytes where to store the data which should be read
       */
      virtual uint64_t ReadBytes (uint64_t nBytes, void *bytes);
      /**
       * methode for writing the bytes to the stream
       * @param nBytes number of bytes to written
       * @param bytes where the bytes are stored which should be written
       */
      virtual uint64_t WriteBytes(uint64_t nBytes, const void *bytes);

      /**
       * we flush the file
       * @return the error flag of the stream
       */
      virtual void Flush();

      /**
       * closes the stream and sets the stream direction to stream_dead
       */
      virtual void CloseStream();

   protected:
      /** pointer to the stream where to read or write the data */
      std::FILE* m_stream_pointer;
      /** holds the name if the file was opened from this object */
      std::string m_file_name;
   };

   /**
    * @}
    */

} // namespace imp
} // namespace tc

#endif // _TCFILE_STREAM_H_
