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
//  $Id: TCAsciiCodec.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCAsciiCodec.h"

#include "TCAssert.h"
#include "TCStream.h"
#include "TCString.h"
#include "TCUtil.h"
#include "TCWString.h"

#include <cctype>
#include <cstring>

#include "TCNewEnable.h"

namespace TC
{
namespace Impl 
{

AsciiCodec::AsciiCodec()
:Codec()
{
}

AsciiCodec::~AsciiCodec()
{
}

uint32 AsciiCodec::Decode(Stream& /* stream */, sint16& /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& /* stream */, uint16& /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& /* stream */, sint32& /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& /* stream */, uint32& /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& /* stream */, sint64& /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& /* stream */, uint64& /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& /* stream */, float& /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& /* stream */, double& /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& /* stream */, char* /* val */)
{
   TC_ASSERT(!"TC::Impl::AsciiCodec::Decode not implemented");
   return 0;
}

uint32 AsciiCodec::Decode(Stream& stream, std::string& val)
{
   val.erase();
   char c;
   while (Decode(stream, c) == 1)
   {
      if (c == '\n')
      {
         break;
      }

      val += c;
   }
   return static_cast<uint32>(val.length());
}

uint32 AsciiCodec::Decode(Stream& stream, std::wstring& val)
{
   std::string tmp;
   uint32 len = Decode(stream, tmp);
   val = WString::ToString(tmp);

   return len;
}

uint32 AsciiCodec::Decode(Stream& stream, char &val)
{
   return stream.ReadBytes(1, &val);
}

uint32 AsciiCodec::Decode(Stream& stream, uchar &val)
{
   return stream.ReadBytes(1, &val);
}


uint32 AsciiCodec::Encode(sint16 val, Stream& stream)
{
   char buffer[16];
   uint32 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%hd", val);

   return stream.WriteBytes(len, buffer);
}
uint32 AsciiCodec::Encode(uint16 val, Stream& stream)
{
   char buffer[16];
   uint32 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%hu", val);

   return stream.WriteBytes(len, buffer);
}

uint32 AsciiCodec::Encode(sint32 val, Stream& stream)
{
   char buffer[32];
   uint32 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%d", val);

   return stream.WriteBytes(len, buffer);
}

uint32 AsciiCodec::Encode(uint32 val, Stream& stream)
{
   char buffer[32];
   uint32 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%u", val);

   return stream.WriteBytes(len, buffer);
}

uint32 AsciiCodec::Encode(sint64 val, Stream& stream)
{
   char buffer[64];
   uint32 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%" TC_SINT64_FORMAT, val);

   return stream.WriteBytes(len, buffer);
}
uint32 AsciiCodec::Encode(uint64 val, Stream& stream)
{
   char buffer[64];
   uint32 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%" TC_UINT64_FORMAT, val);

   return stream.WriteBytes(len, buffer);
}

uint32 AsciiCodec::Encode(float val, Stream& stream)
{
   char buffer[32];
   uint32 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%g", val);

   return stream.WriteBytes(len, buffer);
}
uint32 AsciiCodec::Encode(double val, Stream& stream)
{
   char buffer[64];
   uint32 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%lg", val);

   return stream.WriteBytes(len, buffer);
}

uint32 AsciiCodec::Encode(const std::string& val, Stream& stream)
{
   return stream.WriteBytes(static_cast<uint32>(val.length()), val.c_str());
}

uint32 AsciiCodec::Encode(const std::wstring& val, Stream& stream)
{
   std::string tmp = WString::ToString(val);
   return Encode(tmp, stream);
}

uint32 AsciiCodec::Encode(const char *val, Stream& stream)
{
   return stream.WriteBytes(static_cast<uint32>(std::strlen(val)), val);
}

uint32 AsciiCodec::Encode(char val, Stream& stream)
{
   return stream.WriteBytes(1, &val);
}

uint32 AsciiCodec::Encode(uchar val, Stream& stream)
{
   return stream.WriteBytes(1, &val);
}

uint32 AsciiCodec::EncodeEndOfLine(Stream& stream)
{
   return stream.WriteBytes(1, "\n");
}

uint32 AsciiCodec::EncodeSpace(Stream& stream)
{
   return stream.WriteBytes(1, " ");
}

}
}
