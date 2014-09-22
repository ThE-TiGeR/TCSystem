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
#ifdef HAVE_ZLIB
#include "TCGzFileStream.h"

#include "TCOutput.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace imp
   {
      GzFileStream::GzFileStream(const std::string &fileName, StreamDirection direction, CodecPtr codec)
         :StreamBase(codec)
         ,m_file_name(fileName)
         ,m_file(0)
      {
         if (direction == STREAM_READ)
         {
            m_file = ::gzopen(fileName.c_str(), "rb");
         }
         else if (direction == STREAM_WRITE)
         {
            m_file = ::gzopen(fileName.c_str(), "wb");
         }
         else if (direction == STREAM_READ_WRITE)
         {
            m_file = ::gzopen(fileName.c_str(), "wb+");
         }

         if (!m_file)
         {
            TCERRORS("TCBASE", "Error opening file '" << fileName << "'");
            SetStatus(ERROR_STREAM_OPEN);
         }
         SetStreamDirection(direction);
      }

      GzFileStream::~GzFileStream()
      {
         CloseStream();
      }

      void GzFileStream::CloseStream()
      {
         if (m_file)
         {
            ::gzclose(m_file);
            m_file = 0;
         }

         StreamBase::CloseStream();
      }

      uint64_t GzFileStream::ReadBytes(uint64_t nBytes, void *bytes)
      {
         // check for an error
         if (Error())
         {
            return 0;
         }

         // check mode
         if (!IsReading())
         {
            SetStatus(ERROR_STREAM_DIRECTION);
            return 0;
         }

         uint64_t read_bytes = 0;
         while(read_bytes < nBytes)
         {
            int num = ::gzread(m_file, static_cast<uint8_t*>(bytes)+read_bytes, 
               unsigned(nBytes-read_bytes));
            if (num <= 0)
            {
               if (::gzeof(m_file))
               {
                  SetStatus(ERROR_END_OF_STREAM);
               }
               else
               {
                  SetStatus(ERROR_READ_FROM_STREAM);
               }
               break;
            }
            read_bytes += num;
         }

         return read_bytes;
      }

      uint64_t GzFileStream::WriteBytes(uint64_t nBytes, const void *bytes)
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
         if (!IsWriting())
         {
            SetStatus(ERROR_STREAM_DIRECTION);
            return 0;
         }

         uint64_t wrote_bytes = 0;
         while(wrote_bytes < nBytes)
         {
            int num = ::gzwrite(m_file, static_cast<const uint8_t*>(bytes)+wrote_bytes,  
               unsigned(nBytes-wrote_bytes));
            if (num <= 0)
            {
               SetStatus(ERROR_WRITE_TO_STREAM);
               break;
            }
            wrote_bytes += num;
         }

         return wrote_bytes;
      }

      void GzFileStream::Flush()
      {
         // check for an error
         if (Error())
         {
            return;
         }

         // check correct stream direction
         if (IsWriting())
         {
            ::gzflush(m_file, Z_PARTIAL_FLUSH);
         }
         else
         {
            SetStatus(ERROR_STREAM_DIRECTION);
         }
      }

      bool GzFileStream::SetPosition(int64_t pos, StreamPosition pos_mode)
      {
         ResetStatus();

         switch(pos_mode)
         {
         case POSITION_SET:
            return ::gzseek(m_file, z_off_t(pos), SEEK_SET) == 0;

         case POSITION_CURRENT:
            return ::gzseek(m_file, z_off_t(pos), SEEK_CUR) == 0;

         case POSITION_END:
            return ::gzseek(m_file, z_off_t(pos), SEEK_END) == 0;
         }

         return false;
      }

      uint64_t GzFileStream::GetPosition() const
      {
         return ::gztell(m_file);
      }

      StreamPtr GzFileStream::Clone()
      {
         return StreamPtr(new GzFileStream(m_file_name, GetStreamDirection(), GetCodec()->Clone()));
      }

   }
}
#endif
