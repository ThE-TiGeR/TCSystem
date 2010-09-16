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

#include "TCStringStream.h"

#include <cstring>

#include "TCNewEnable.h"

namespace TC
{
   namespace Impl
   {

      StringStream::StringStream(CodecPtr codec, std::string& memory)
         :StreamBase(codec),
         m_string(memory),
         m_string_position(0)
      {
         setStreamDirection(stream_readwrite);
      }

      StringStream::~StringStream()
      {
      }

      uint32 StringStream::ReadBytes(uint32 nBytes, void *bytes)
      {
         std::memcpy(bytes, &m_string[ m_string_position], nBytes);

         m_string_position += nBytes;

         return nBytes;
      }

      uint32 StringStream::WriteBytes(uint32 nBytes, const void *bytes)
      {
         m_string.replace(m_string_position, nBytes, static_cast<const char*>(bytes), nBytes);

         m_string_position += nBytes;

         return nBytes;
      }

      bool StringStream::SetPosition(sint32 pos, StreamPosition pos_mode)
      {
         switch(pos_mode)
         {
         case POSITION_SET:
            m_string_position = pos;
            break;

         case POSITION_CURRENT:
            m_string_position += pos;
            break;

         case POSITION_END:
            m_string_position = m_string.size() + pos;
            break;
         }

         return true;
      }

      uint32 StringStream::GetPosition() const
      {
         return m_string_position;
      }
   }
}

