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

#include "TCMemoryStream.h"

#include <cstring>

#include "TCNewEnable.h"

namespace TC
{
   namespace Impl
   {

      MemoryStream::MemoryStream(CodecPtr codec, std::vector< uchar >& memory)
         :StreamBase(codec),
         m_memory(memory),
         m_memory_position(0)
      {
         setStreamDirection(stream_readwrite);
      }

      MemoryStream::~MemoryStream()
      {
      }

      uint32 MemoryStream::ReadBytes(uint32 num_bytes, void* bytes)
      {
         if (m_memory_position + num_bytes <= m_memory.size())
         {
            std::memcpy(bytes, &m_memory[m_memory_position], num_bytes);
            m_memory_position += num_bytes;
            return num_bytes;
         }

         return 0;
      }

      uint32 MemoryStream::WriteBytes(uint32 num_bytes, const void* bytes)
      {
         const uchar* data_to_insert = static_cast<const uchar*>(bytes);
         m_memory.resize(m_memory_position, 0);
         m_memory.insert(m_memory.end(), data_to_insert, data_to_insert + num_bytes);
         m_memory_position = m_memory.size();

         return num_bytes;
      }

      bool MemoryStream::SetPosition(sint32 pos, StreamPosition pos_mode)
      {
         switch(pos_mode)
         {
         case POSITION_SET:
            m_memory_position = pos;
            break;

         case POSITION_CURRENT:
            m_memory_position += pos;
            break;

         case POSITION_END:
            m_memory_position = m_memory.size() + pos;
            break;
         }

         return true;
      }

      uint32 MemoryStream::GetPosition() const
      {
         return uint32(m_memory_position);
      }
   }
}

