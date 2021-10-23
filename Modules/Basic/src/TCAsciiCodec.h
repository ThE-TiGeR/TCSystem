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
//  $Id: TCAsciiCodec.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
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

      virtual CodecPtr Clone();

      virtual uint64_t Decode(Stream& stream, int8_t &val);
      virtual uint64_t Decode(Stream& stream, uint8_t &val);
      virtual uint64_t Decode(Stream& stream, int16_t &val);
      virtual uint64_t Decode(Stream& stream, uint16_t &val);
      virtual uint64_t Decode(Stream& stream, int32_t &val);
      virtual uint64_t Decode(Stream& stream, uint32_t &val);
      virtual uint64_t Decode(Stream& stream, int64_t &val);
      virtual uint64_t Decode(Stream& stream, uint64_t &val);
      virtual uint64_t Decode(Stream& stream, float &val);
      virtual uint64_t Decode(Stream& stream, double &val);
      virtual uint64_t Decode(Stream& stream, std::string& val);
      virtual uint64_t Decode(Stream& stream, std::wstring& val);
      virtual uint64_t Decode(Stream& stream, char &val);
      virtual uint64_t Decode(Stream& stream, bool &val);

      virtual uint64_t Encode(int8_t val, Stream& stream);
      virtual uint64_t Encode(uint8_t val, Stream& stream);
      virtual uint64_t Encode(int16_t val, Stream& stream);
      virtual uint64_t Encode(uint16_t val, Stream& stream);
      virtual uint64_t Encode(int32_t val, Stream& stream);
      virtual uint64_t Encode(uint32_t val, Stream& stream);
      virtual uint64_t Encode(int64_t val, Stream& stream);
      virtual uint64_t Encode(uint64_t val, Stream& stream);
      virtual uint64_t Encode(float val, Stream& stream);
      virtual uint64_t Encode(double val, Stream& stream);
      virtual uint64_t Encode(const std::string& val, Stream& stream);
      virtual uint64_t Encode(const std::wstring& val, Stream& stream);
      virtual uint64_t Encode(const char *val, Stream& stream);
      virtual uint64_t Encode(char val, Stream& stream);
      virtual uint64_t Encode(bool val, Stream& stream);

      virtual uint64_t EncodeEndOfLine(Stream& stream);
      virtual uint64_t EncodeSpace(Stream& stream);
   };

   /**
    * @}
    */

} // namespace imp
} // namespace tc

#endif // _TCASCII_CODEC_H_
