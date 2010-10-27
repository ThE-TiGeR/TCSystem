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

static bool iseol(char c)
{
   return c == '\n' || c == '\r';
}

static uint64 ReadNextValueString(Stream& stream, std::string& val)
{
   uint64 len = 0;
   char c;
   // skipp spaces and end of line
   while (stream.ReadBytes(1, &c) == 1)
   {
      len++;
      if (std::isspace(c) || iseol(c))
      {
         continue;
      }
      else
      {
         val = c;
         break;
      }
   }

   while (stream.ReadBytes(1, &c) == 1)
   {
      len++;
      if (std::isspace(c) || iseol(c))
      {
         break;
      }
      val += c;
   }

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, sint16& val)
{
   std::string strval;
   uint64 len = ReadNextValueString(stream, strval);
   val = String::ToSint16(strval);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, uint16& val)
{
   std::string strval;
   uint64 len = ReadNextValueString(stream, strval);
   val = String::ToUint16(strval);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, sint32& val)
{
   std::string strval;
   uint64 len = ReadNextValueString(stream, strval);
   val = String::ToSint32(strval);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, uint32& val)
{
   std::string strval;
   uint64 len = ReadNextValueString(stream, strval);
   val = String::ToUint32(strval);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, sint64& val)
{
   std::string strval;
   uint64 len = ReadNextValueString(stream, strval);
   val = String::ToSint64(strval);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, uint64& val)
{
   std::string strval;
   uint64 len = ReadNextValueString(stream, strval);
   val = String::ToUint64(strval);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, float& val)
{
   std::string strval;
   uint64 len = ReadNextValueString(stream, strval);
   val = String::ToFloat(strval);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, double& val)
{
   std::string strval;
   uint64 len = ReadNextValueString(stream, strval);
   val = String::ToDouble(strval);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, std::string& val)
{
   val.erase();
   uint64 len = 0;
   char c;
   // skipp end of lines
   while (stream.ReadBytes(1, &c) == 1)
   {
      len++;
      if (iseol(c))
      {
         continue;
      }
      else
      {
         val = c;
         break;
      }
   }

   while (stream.ReadBytes(1, &c) == 1)
   {
      len++;
      if (iseol(c))
      {
         break;
      }

      val += c;
   }
   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, std::wstring& val)
{
   std::string tmp;
   uint64 len = Decode(stream, tmp);
   val = WString::ToString(tmp);

   return len;
}

uint64 AsciiCodec::Decode(Stream& stream, char &val)
{
   return stream.ReadBytes(1, &val);
}

uint64 AsciiCodec::Decode(Stream& stream, uchar &val)
{
   return stream.ReadBytes(1, &val);
}


uint64 AsciiCodec::Encode(sint16 val, Stream& stream)
{
   char buffer[16];
   uint64 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%hd", val);

   return stream.WriteBytes(len, buffer);
}
uint64 AsciiCodec::Encode(uint16 val, Stream& stream)
{
   char buffer[16];
   uint64 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%hu", val);

   return stream.WriteBytes(len, buffer);
}

uint64 AsciiCodec::Encode(sint32 val, Stream& stream)
{
   char buffer[32];
   uint64 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%d", val);

   return stream.WriteBytes(len, buffer);
}

uint64 AsciiCodec::Encode(uint32 val, Stream& stream)
{
   char buffer[32];
   uint64 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%u", val);

   return stream.WriteBytes(len, buffer);
}

uint64 AsciiCodec::Encode(sint64 val, Stream& stream)
{
   char buffer[64];
   uint64 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%" TC_SINT64_FORMAT, val);

   return stream.WriteBytes(len, buffer);
}
uint64 AsciiCodec::Encode(uint64 val, Stream& stream)
{
   char buffer[64];
   uint64 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%" TC_UINT64_FORMAT, val);

   return stream.WriteBytes(len, buffer);
}

uint64 AsciiCodec::Encode(float val, Stream& stream)
{
   char buffer[32];
   uint64 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%g", val);

   return stream.WriteBytes(len, buffer);
}
uint64 AsciiCodec::Encode(double val, Stream& stream)
{
   char buffer[64];
   uint64 len = String::Snprintf(buffer, Util::ArraySize(buffer), "%lg", val);

   return stream.WriteBytes(len, buffer);
}

uint64 AsciiCodec::Encode(const std::string& val, Stream& stream)
{
   return stream.WriteBytes(val.length(), val.c_str());
}

uint64 AsciiCodec::Encode(const std::wstring& val, Stream& stream)
{
   std::string tmp = WString::ToString(val);
   return Encode(tmp, stream);
}

uint64 AsciiCodec::Encode(const char *val, Stream& stream)
{
   return stream.WriteBytes(std::strlen(val), val);
}

uint64 AsciiCodec::Encode(char val, Stream& stream)
{
   return stream.WriteBytes(1, &val);
}

uint64 AsciiCodec::Encode(uchar val, Stream& stream)
{
   return stream.WriteBytes(1, &val);
}

uint64 AsciiCodec::EncodeEndOfLine(Stream& stream)
{
   return stream.WriteBytes(1, "\n");
}

uint64 AsciiCodec::EncodeSpace(Stream& stream)
{
   return stream.WriteBytes(1, " ");
}

}
}
