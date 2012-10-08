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

#include "TCStringStream.h"

#include "TCUtil.h"

#include <cstring>
#include <limits>

#include "TCNewEnable.h"

namespace tc
{
   namespace imp
   {

      StringStream::StringStream(CodecPtr codec, std::string& memory)
         :StreamBase(codec),
         m_string(memory),
         m_string_position(0)
      {
         SetStreamDirection(STREAM_READ_WRITE);
      }

      StringStream::~StringStream()
      {
      }

      uint64_t StringStream::ReadBytes(uint64_t nBytes, void *bytes)
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

         if (nBytes > std::numeric_limits<std::string::size_type>::max())
         {
            return 0;
         }

         if (m_string_position == m_string.size())
         {
            SetStatus(ERROR_END_OF_STREAM);
            return 0;
         }

         if (m_string_position < m_string.size())
         {
            uint64_t num_bytes_to_read = util::Min(nBytes, uint64_t(m_string.size() - m_string_position));
            std::memcpy(bytes, &m_string[m_string_position], std::string::size_type(num_bytes_to_read));

            m_string_position += std::string::size_type(num_bytes_to_read);
         }

         return nBytes;
      }

      uint64_t StringStream::WriteBytes(uint64_t nBytes, const void *bytes)
      {
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

         if (nBytes > std::numeric_limits<std::string::size_type>::max())
         {
            return 0;
         }

         m_string.replace(m_string_position, std::string::size_type(nBytes), 
            std::string::const_pointer(bytes), std::string::size_type(nBytes));

         m_string_position += std::string::size_type(nBytes);

         return nBytes;
      }

      bool StringStream::SetPosition(int64_t pos, StreamPosition pos_mode)
      {
         switch(pos_mode)
         {
         case POSITION_SET:
            m_string_position = std::string::size_type(pos);
            break;

         case POSITION_CURRENT:
            if (pos > 0)
            {
               m_string_position += std::string::size_type(pos);
            }
            else
            {
               m_string_position -= std::string::size_type(util::Abs(pos));
            }
            break;

         case POSITION_END:
            if (pos > 0)
            {
               m_string_position = m_string.size() + std::string::size_type(pos);
            }
            else
            {
               m_string_position = m_string.size() - std::string::size_type(util::Abs(pos));
            }
            break;
         }

         return true;
      }

      uint64_t StringStream::GetPosition() const
      {
         return m_string_position;
      }

      StreamPtr StringStream::Clone()
      {
         return StreamPtr(new StringStream(GetCodec()->Clone(), m_string));
      }

   }
}

