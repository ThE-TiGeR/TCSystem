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
#ifndef _TC_BZ2_FILE_STREAM_H_
#define _TC_BZ2_FILE_STREAM_H_

#include "TCStreamBase.h"

#include <cstdio>
#include <bzlib.h>

namespace TC
{
    namespace Impl
    {

        /**
        * @addtogroup TC_BASE_IO_IMPL
        * @{
        */

        /**
        * @file
        * @brief This file provides the definition of TC::Bz2FileStream
        *
        * @author Thomas Goessler
        */

        /**
        * @brief Class for reading/writing a gz file
        *
        * Just implements the writing and reading of bytes
        * the rest is done in StreamBase
        */
        class TC_DLL_LOCAL Bz2FileStream: public StreamBase
        {
        public:
            Bz2FileStream(const std::string &fileName, StreamDirection direction, CodecPtr codec);
            virtual ~Bz2FileStream();

            virtual bool SetPosition(sint64, StreamPosition pos);
            virtual uint64 GetPosition() const;

            virtual uint64 ReadBytes (uint64 nBytes, void *bytes);
            virtual uint64 WriteBytes(uint64 nBytes, const void *bytes);
            virtual void Flush();

            virtual void CloseStream();

        private:
            BZFILE* m_file;
        };

        /**
        * @}
        */

    } // namespace Impl
} // namespace TC

#endif // _TC_BZ2_FILE_STREAM_H_
