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
//  $Id: TCBz2FileStream.cpp,v 43c905f64cb9 2014/09/05 21:36:04 thomas $
//*******************************************************************************
#ifdef HAVE_BZIP2
#include "TCBz2FileStream.h"

#include "TCOutput.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace imp
   {
      Bz2FileStream::Bz2FileStream(const std::string &fileName, Direction direction, CodecPtr codec)
         :StreamBase(codec)
         ,m_file_name(fileName)
         ,m_file(0)
      {
         if (direction == Direction::READ)
         {
            m_file = ::BZ2_bzopen(fileName.c_str(), "rb");
         }
         else if (direction == Direction::WRITE)
         {
            m_file = ::BZ2_bzopen(fileName.c_str(), "wb");
         }
         else if (direction == Direction::READ_WRITE)
         {
            m_file = ::BZ2_bzopen(fileName.c_str(), "wb+");
         }

         if (!m_file)
         {
            TCERRORS("TCBASE", "Error opening file '" << fileName << "'");
            SetStatus(Error::STREAM_OPEN);
         }
         SetStreamDirection(direction);
      }

      Bz2FileStream::~Bz2FileStream()
      {
         CloseStream();
      }

      void Bz2FileStream::CloseStream()
      {
         if (m_file)
         {
            ::BZ2_bzclose(m_file);
            m_file = 0;
         }

         StreamBase::CloseStream();
      }

      uint64_t Bz2FileStream::ReadBytes(uint64_t nBytes, void *bytes)
      {
         // check for an error
         if (HasError())
         {
            return 0;
         }

         // check mode
         if (!IsReading())
         {
            SetStatus(Error::STREAM_DIRECTION);
            return 0;
         }

         uint64_t read_bytes = 0;
         while(read_bytes < nBytes)
         {
            int num = ::BZ2_bzread(m_file, static_cast<uint8_t*>(bytes)+read_bytes, 
               int(nBytes-read_bytes));
            if (num <= 0)
            {
               int error_num;
               BZ2_bzerror(m_file, &error_num);
               if (error_num == BZ_STREAM_END)
               {
                  SetStatus(Error::END_OF_STREAM);
               }
               else
               {
                  SetStatus(Error::READ_FROM_STREAM);
               }
               break;
            }
            read_bytes += num;
         }

         return read_bytes;
      }

      uint64_t Bz2FileStream::WriteBytes(uint64_t nBytes, const void *bytes)
      {
         if (nBytes == 0)
         {
            return 0;
         }

         // check for an error
         if (HasError())
         {
            return 0;
         }

         // check mode
         if (!IsWriting())
         {
            SetStatus(Error::STREAM_DIRECTION);
            return 0;
         }

         uint64_t wrote_bytes = 0;
         while(wrote_bytes < nBytes)
         {
            int num = ::BZ2_bzwrite(m_file, const_cast<uint8_t*>(static_cast<const uint8_t*>(bytes)+wrote_bytes),  
               int(nBytes-wrote_bytes));
            if (num <= 0)
            {
               SetStatus(Error::WRITE_TO_STREAM);
               break;
            }
            wrote_bytes += num;
         }

         return wrote_bytes;
      }

      void Bz2FileStream::Flush()
      {
         // check for an error
         if (HasError())
         {
            return;
         }

         // check correct stream direction
         if (IsWriting())
         {
            ::BZ2_bzflush(m_file);
         }
         else
         {
            SetStatus(Error::STREAM_DIRECTION);
         }
      }

      bool Bz2FileStream::SetPosition(int64_t, Position)
      {
         SetStatus(Error::SET_STREAM_POSITION);
         return false;
      }

      uint64_t Bz2FileStream::GetPosition() const
      {
         SetStatus(Error::SET_STREAM_POSITION);
         return 0;
      }

      StreamPtr Bz2FileStream::Clone()
      {
         return StreamPtr(new Bz2FileStream(m_file_name, GetStreamDirection(), GetCodec()->Clone()));
      }

   }
}
#endif
