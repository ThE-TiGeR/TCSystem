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
#ifndef _TC_GZ_FILE_STREAM_H_
#define _TC_GZ_FILE_STREAM_H_

#include "TCStreamBase.h"

#include <cstdio>
#include <zlib.h>

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
      * @brief This file provides the definition of tc::GzFileStream
      *
      * @author Thomas Goessler
      */

      /**
      * @brief Class for reading/writing a gz file
      *
      * Just implements the writing and reading of bytes
      * the rest is done in StreamBase
      */
      class TC_DLL_LOCAL GzFileStream: public StreamBase
      {
      public:
         GzFileStream(const std::string &fileName, StreamDirection direction, CodecPtr codec);
         virtual ~GzFileStream();

         virtual StreamPtr Clone();

         virtual bool SetPosition(int64_t, StreamPosition pos);
         virtual uint64_t GetPosition() const;

         virtual uint64_t ReadBytes (uint64_t nBytes, void *bytes);
         virtual uint64_t WriteBytes(uint64_t nBytes, const void *bytes);

         virtual void Flush();
         virtual void CloseStream();

      private:
         std::string m_file_name;
         gzFile m_file;
      };

      /**
      * @}
      */

   } // namespace imp
} // namespace tc

#endif // _TC_GZ_FILE_STREAM_H_
