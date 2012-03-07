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
#ifndef _TCASCII_CODEC_H_
#define _TCASCII_CODEC_H_

#include "TCCodec.h"

namespace tc
{
namespace imp
{
   /**
    * @addtogroup TC_BASE_IO_IMPL
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of tc::imp::AsciiCodec
    * @author Thomas Goessler
    */

   /**
    * @brief A ordinary ascii codec
    * @author Thomas Goessler
    */
   class TC_DLL_LOCAL AsciiCodec: public Codec
   {
   public:
      AsciiCodec();
      virtual ~AsciiCodec();

      virtual uint64 Decode(Stream& stream, sint16 &val);
      virtual uint64 Decode(Stream& stream, uint16 &val);
      virtual uint64 Decode(Stream& stream, sint32 &val);
      virtual uint64 Decode(Stream& stream, uint32 &val);
      virtual uint64 Decode(Stream& stream, sint64 &val);
      virtual uint64 Decode(Stream& stream, uint64 &val);
      virtual uint64 Decode(Stream& stream, float &val);
      virtual uint64 Decode(Stream& stream, double &val);
      virtual uint64 Decode(Stream& stream, std::string& val);
      virtual uint64 Decode(Stream& stream, std::wstring& val);
      virtual uint64 Decode(Stream& stream, char &val);
      virtual uint64 Decode(Stream& stream, uchar &val);
      virtual uint64 Decode(Stream& stream, bool &val);

      virtual uint64 Encode(sint16 val, Stream& stream);
      virtual uint64 Encode(uint16 val, Stream& stream);
      virtual uint64 Encode(sint32 val, Stream& stream);
      virtual uint64 Encode(uint32 val, Stream& stream);
      virtual uint64 Encode(sint64 val, Stream& stream);
      virtual uint64 Encode(uint64 val, Stream& stream);
      virtual uint64 Encode(float val, Stream& stream);
      virtual uint64 Encode(double val, Stream& stream);
      virtual uint64 Encode(const std::string& val, Stream& stream);
      virtual uint64 Encode(const std::wstring& val, Stream& stream);
      virtual uint64 Encode(const char *val, Stream& stream);
      virtual uint64 Encode(char val, Stream& stream);
      virtual uint64 Encode(uchar val, Stream& stream);
      virtual uint64 Encode(bool val, Stream& stream);

      virtual uint64 EncodeEndOfLine(Stream& stream);
      virtual uint64 EncodeSpace(Stream& stream);
   };

   /**
    * @}
    */

} // namespace imp
} // namespace tc

#endif // _TCASCII_CODEC_H_
