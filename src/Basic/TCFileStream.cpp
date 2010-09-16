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

#include "TCFileStream.h"

#include "TCOutput.h"

#include "TCNewEnable.h"

namespace TC
{
namespace Impl
{

FileStream::FileStream(std::FILE *stream, StreamDirection direction, CodecPtr codec)
:StreamBase(codec),
m_stream_pointer(0),
m_is_stream_allocated(false)
{
   SetStream(stream, direction);
}

FileStream::FileStream(const std::string &fileName, StreamDirection direction, CodecPtr codec)
:StreamBase(codec),
m_stream_pointer(0),
m_is_stream_allocated(false)
{
   SetStream(fileName, direction);
}

FileStream::~FileStream()
{
   CloseStream();
}

void FileStream::CloseStream()
{
   if (m_is_stream_allocated == true && m_stream_pointer)
   {
      std::fclose(m_stream_pointer);
   }

   m_is_stream_allocated = false;
   m_stream_pointer     = 0;

   StreamBase::CloseStream();
}

void FileStream::SetStream(std::FILE *stream, StreamDirection direction)
{
   CloseStream();

   m_stream_pointer = stream;

   setStreamDirection(direction);
}

void FileStream::SetStream(const std::string &fileName, StreamDirection direction)
{
   CloseStream();

   FILE* file = 0;
   if (direction == stream_read)
   {
      file = std::fopen(fileName.c_str(), "rb");
   }
   else if (direction == stream_write)
   {
      file = std::fopen(fileName.c_str(), "wb");
   }
   else if (direction == stream_readwrite)
   {
      file = std::fopen(fileName.c_str(), "wb+");
   }

   if (!file)
   {
      TCERROR1("TCBASE", "Error opening file '%s'", fileName.c_str());
      setStatus(error_streamopen);
   }

   SetStream(file, direction);
   m_is_stream_allocated = true;
}


uint64 FileStream::ReadBytes(uint64 nBytes, void *bytes)
{
   // check for an error
   if (Error())
   {
      return 0;
   }

   // check mode
   if (!isReading())
   {
      setStatus(error_streamdirection);
      return 0;
   }

   uint64 read_bytes = 0;
   while(read_bytes < nBytes)
   {
       std::size_t num = std::fread(static_cast<uchar*>(bytes)+read_bytes, 1, 
           std::size_t(nBytes-read_bytes), m_stream_pointer);
       if (num <= 0)
       {
           if (std::feof(m_stream_pointer))
           {
               setStatus(error_end_file);
           }
           else
           {
               setStatus(error_read_file);
           }
           break;
       }
       read_bytes += num;
   }

   return read_bytes;
}

uint64 FileStream::WriteBytes(uint64 nBytes, const void *bytes)
{
   if (nBytes == 0)
   {
      return 0;
   }

   // check for an error
   if (Error())
   {
      return 0;
   }

   // check mode
   if (!isWriting())
   {
      setStatus(error_streamdirection);
      return 0;
   }

   uint64 wrote_bytes = 0;
   while(wrote_bytes < nBytes)
   {
       std::size_t num = std::fwrite(static_cast<const uchar*>(bytes)+wrote_bytes, 1, 
           std::size_t(nBytes-wrote_bytes), m_stream_pointer);
       if (num <= 0)
       {
           setStatus(error_write_file);
           break;
       }
       wrote_bytes += num;
   }

   return wrote_bytes;
}

void FileStream::Flush()
{
   // check for an error
   if (Error())
   {
      return;
   }

   // check correct stream direction
   if (isWriting())
   {
      std::fflush(m_stream_pointer);
   }
   else
   {
      setStatus(error_streamdirection);
   }
}

void FileStream::displayErrorMessage() const
{
   switch (GetStatus())
   {
      case error_end_file:
       break;

      default:
         StreamBase::displayErrorMessage();
   }
}

bool FileStream::SetPosition(sint64 pos, StreamPosition pos_mode)
{
   ResetStatus();

   switch(pos_mode)
   {
   case POSITION_SET:
      return std::fseek(m_stream_pointer, ssize_type(pos), SEEK_SET) == 0;

   case POSITION_CURRENT:
      return std::fseek(m_stream_pointer, ssize_type(pos), SEEK_CUR) == 0;

   case POSITION_END:
      return std::fseek(m_stream_pointer, ssize_type(pos), SEEK_END) == 0;
   }

   return false;
}

uint64 FileStream::GetPosition() const
{
   return std::ftell(m_stream_pointer);
}

}
}
