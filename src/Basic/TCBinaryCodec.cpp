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

#include "TCBinaryCodec.h"

#include "TCStream.h"
#include "TCValue.h"
#include "TCWString.h"

#include <cstring>

#include "TCNewEnable.h"

namespace TC
{
   namespace Impl 
   {
      template < template<class T> class VALUE_CONV >
      class BinaryCodec: public Codec
      {
      public:
         BinaryCodec()
         {
         }

         ~BinaryCodec()
         {
         }

         uint64 Decode(Stream& stream, sint16 &val)
         {
            VALUE_CONV< sint16 > bval;
            if (stream.ReadBytes(bval.GetNumBytes(), bval.GetBytes()) != bval.GetNumBytes())
            {
               return 0;
            }
            val = bval;
            return bval.GetNumBytes();
         }

         uint64 Decode(Stream& stream, uint16 &val)
         {
            VALUE_CONV< uint16 > bval;
            if (stream.ReadBytes(bval.GetNumBytes(), bval.GetBytes()) != bval.GetNumBytes())
            {
               return 0;
            }
            val = bval;
            return bval.GetNumBytes();
         }

         uint64 Decode(Stream& stream, sint32 &val)
         {
            VALUE_CONV< sint32 > bval;
            if (stream.ReadBytes(bval.GetNumBytes(), bval.GetBytes()) != bval.GetNumBytes())
            {
               return 0;
            }
            val = bval;
            return bval.GetNumBytes();
         }

         uint64 Decode(Stream& stream, uint32 &val)
         {
            VALUE_CONV< uint32 > bval;
            if (stream.ReadBytes(bval.GetNumBytes(), bval.GetBytes()) != bval.GetNumBytes())
            {
               return 0;
            }
            val = bval;
            return bval.GetNumBytes();
         }

         uint64 Decode(Stream& stream, sint64 &val)
         {
            VALUE_CONV< sint64 > bval;
            if (stream.ReadBytes(bval.GetNumBytes(), bval.GetBytes()) != bval.GetNumBytes())
            {
               return 0;
            }
            val = bval;
            return bval.GetNumBytes();
         }

         uint64 Decode(Stream& stream, uint64 &val)
         {
            VALUE_CONV< uint64 > bval;
            if (stream.ReadBytes(bval.GetNumBytes(), bval.GetBytes()) != bval.GetNumBytes())
            {
               return 0;
            }
            val = bval;
            return bval.GetNumBytes();
         }

         uint64 Decode(Stream& stream, float &val)
         {
            VALUE_CONV< float > bval;
            if (stream.ReadBytes(bval.GetNumBytes(), bval.GetBytes()) != bval.GetNumBytes())
            {
               return 0;
            }
            val = bval;
            return bval.GetNumBytes();
         }

         uint64 Decode(Stream& stream, double &val)
         {
            VALUE_CONV< double > bval;
            if (stream.ReadBytes(bval.GetNumBytes(), bval.GetBytes()) != bval.GetNumBytes())
            {
               return 0;
            }
            val = bval;
            return bval.GetNumBytes();
         }

         uint64 Decode(Stream& stream, std::string& val)
         {
            // for char we have to read the len first
            uint32 nBytes;
            uint64 len = Decode(stream, nBytes);

            char* data = new char[std::size_t(nBytes)+1];
            if (stream.ReadBytes(nBytes, data) != nBytes)
            {
               delete []data;
               return 0;
            }
            data[nBytes] = 0;
            val = data;

            delete []data;
            return len + nBytes;
         }

         uint64 Decode(Stream& stream, std::wstring& val)
         {
            std::string tmp;
            uint64 len = Decode(stream, tmp);
            val = WString::ToString(tmp);

            return len;
         }

         uint64 Decode(Stream& stream, char *val)
         {
            // for char we have to read the len first
            uint32 nBytes;
            uint64 len = Decode(stream, nBytes);

            if (stream.ReadBytes(nBytes, val) != nBytes)
            {
               return 0;
            }
            val[nBytes] = 0;
            return len + nBytes;
         }

         uint64 Decode(Stream& stream, char &val)
         {
            return stream.ReadBytes(1, &val);
         }

         uint64 Decode(Stream& stream, uchar &val)
         {
            return stream.ReadBytes(1, &val);
         }


         uint64 Encode(sint16 val, Stream& stream)
         {
            VALUE_CONV< sint16 > bval(val);
            return stream.WriteBytes(bval.GetNumBytes(), bval.GetBytes());
         }

         uint64 Encode(uint16 val, Stream& stream)
         {
            VALUE_CONV< uint16 > bval(val);
            return stream.WriteBytes(bval.GetNumBytes(), bval.GetBytes());
         }

         uint64 Encode(sint32 val, Stream& stream)
         {
            VALUE_CONV< sint32 > bval(val);
            return stream.WriteBytes(bval.GetNumBytes(), bval.GetBytes());
         }

         uint64 Encode(uint32 val, Stream& stream)
         {
            VALUE_CONV< uint32 > bval(val);
            return stream.WriteBytes(bval.GetNumBytes(), bval.GetBytes());
         }

         uint64 Encode(sint64 val, Stream& stream)
         {
            VALUE_CONV< sint64 > bval(val);
            return stream.WriteBytes(bval.GetNumBytes(), bval.GetBytes());
         }

         uint64 Encode(uint64 val, Stream& stream)
         {
            VALUE_CONV< uint64 > bval(val);
            return stream.WriteBytes(bval.GetNumBytes(), bval.GetBytes());
         }

         uint64 Encode(float val, Stream& stream)
         {
            VALUE_CONV< float > bval(val);
            return stream.WriteBytes(bval.GetNumBytes(), bval.GetBytes());
         }

         uint64 Encode(double val, Stream& stream)
         {
            VALUE_CONV< double > bval(val);
            return stream.WriteBytes(bval.GetNumBytes(), bval.GetBytes());
         }

         uint64 Encode(const std::string& val, Stream& stream)
         {
            uint32 nBytes = uint32(val.length());
            uint64 len = Encode(nBytes, stream);

            return stream.WriteBytes(nBytes, val.c_str()) + len;
         }

         uint64 Encode(const std::wstring& val, Stream& stream)
         {
            std::string tmp = WString::ToString(val);
            return Encode(tmp, stream);
         }

         uint64 Encode(const char *val, Stream& stream)
         {
            uint32 nBytes = uint32(std::strlen(val));
            uint64 len = Encode(nBytes, stream);

            return stream.WriteBytes(nBytes, val) + len;
         }

         uint64 Encode(char val, Stream& stream)
         {
            return stream.WriteBytes(1, &val);
         }

         uint64 Encode(uchar val, Stream& stream)
         {
            return stream.WriteBytes(1, &val);
         }

         uint64 EncodeEndOfLine(Stream& /*stream*/)
         {
            return 0;
         }

         uint64 EncodeSpace(Stream& /*stream*/)
         {
            return 0;
         }

      };

      TC_DLL_LOCAL CodecPtr CreateBinaryCodec()
      {
         return CodecPtr(new BinaryCodec<Value>);
      }

      TC_DLL_LOCAL CodecPtr CreateLittleEndianBinaryCodec()
      {
         return CodecPtr(new BinaryCodec<LittleEndianValue>);
      }

      TC_DLL_LOCAL CodecPtr CreateBigEndianBinaryCodec()
      {
         return CodecPtr(new BinaryCodec<BigEndianValue>);
      }
   }
}

