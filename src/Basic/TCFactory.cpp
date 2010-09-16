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

#include "TCFactory.h"

#include "TCRng69069.h"
#include "TCAsciiCodec.h"
#include "TCBinaryCodec.h"
#include "TCFileStream.h"
#ifdef TCOS_WINDOWS
#  include "TCMemoryMappedFileWin32.h"
#else
#  include "TCMemoryMappedFilePosix.h"
#endif
#include "TCMemoryStream.h"
#include "TCStringStream.h"
#include "TCTime.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Factory
   {
      CodecPtr CreateBinaryCodec()
      {
         return Impl::CreateBinaryCodec();
      }

      CodecPtr CreateLittleEndianBinaryCodec()
      {
         return Impl::CreateLittleEndianBinaryCodec();
      }

      CodecPtr CreateBigEndianBinaryCodec()
      {
         return Impl::CreateBigEndianBinaryCodec();
      }

      CodecPtr CreateAsciiCodec()
      {
         return CodecPtr(new Impl::AsciiCodec);
      }

      StreamPtr CreateFileStream(const std::string &fileName, 
                                 Stream::StreamDirection direction,
                                 CodecPtr codec)
      {
         StreamPtr stream(new Impl::FileStream(fileName, direction, codec));
         if (!stream->IsOk())
         {
            return StreamPtr();
         }
         
         return stream;
      }

      StreamPtr CreateFileStream(std::FILE* stream, 
                                 Stream::StreamDirection direction, 
                                 CodecPtr codec)
      {
         return StreamPtr(new Impl::FileStream(stream, direction, codec));
      }

      StreamPtr CreateMemoryStream(CodecPtr codec, ByteVector& memory)
      {
         return StreamPtr(new Impl::MemoryStream(codec, memory));
      }

      StreamPtr CreateStringStream(std::string& memory)
      {
         CodecPtr codec(CreateAsciiCodec());
         return StreamPtr(new Impl::StringStream(codec, memory));
      }

      StreamPtr CreateStdOutStream()
      {
         CodecPtr codec(CreateAsciiCodec());
         return CreateFileStream(stdout, Stream::stream_write, codec);
      }

      StreamPtr CreateStdErrorStream()
      {
         CodecPtr codec(CreateAsciiCodec());
         return CreateFileStream(stderr, Stream::stream_write, codec);
      }

      static Impl::Rng69069 s_internal_rng(static_cast<uint32>(Time::Now().ToMilliSeconds()));

      RngPtr Create69069Rng()
      {
         RngPtr rng(new Impl::Rng69069(s_internal_rng.GetRandomNumber()));
         return rng;
      }

      MemoryMappedFilePtr CreateMemoryMappedFile(const std::string &file_name, bool read_only, uint32 size)
      {
#ifdef TCOS_WINDOWS
         SharedPtr<MemoryMappedFileWin32> file(new MemoryMappedFileWin32);
#else
         SharedPtr<MemoryMappedFilePosix> file(new MemoryMappedFilePosix);
#endif
         if (!file->MapToMemory(file_name, read_only, size))
         {
            return MemoryMappedFilePtr();
         }

         return file;
      }

   }
}


