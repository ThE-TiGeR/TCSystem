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
#include "TCBz2FileStream.h"

#include "TCOutput.h"

#include "TCNewEnable.h"

namespace TC
{
    namespace Impl
    {
        Bz2FileStream::Bz2FileStream(const std::string &fileName, StreamDirection direction, CodecPtr codec)
            :StreamBase(codec)
            ,m_file(0)
        {
            if (direction == stream_read)
            {
                m_file = ::BZ2_bzopen(fileName.c_str(), "rb");
            }
            else if (direction == stream_write)
            {
                m_file = ::BZ2_bzopen(fileName.c_str(), "wb");
            }
            else if (direction == stream_readwrite)
            {
                m_file = ::BZ2_bzopen(fileName.c_str(), "wb+");
            }

            if (!m_file)
            {
                TCERROR1("TCBASE", "Error opening file '%s'", fileName.c_str());
                setStatus(error_stream_open);
            }
            setStreamDirection(direction);
        }

        Bz2FileStream::~Bz2FileStream()
        {
            CloseStream();
        }

        void Bz2FileStream::CloseStream()
        {
            if (m_file)
            {
                ::BZ2_bzclose(m_file);
                m_file = 0;
            }

            StreamBase::CloseStream();
        }

        uint64 Bz2FileStream::ReadBytes(uint64 nBytes, void *bytes)
        {
            // check for an error
            if (Error())
            {
                return 0;
            }

            // check mode
            if (!isReading())
            {
                setStatus(error_stream_direction);
                return 0;
            }

            uint64 read_bytes = 0;
            while(read_bytes < nBytes)
            {
                int num = ::BZ2_bzread(m_file, static_cast<uchar*>(bytes)+read_bytes, 
                    int(nBytes-read_bytes));
                if (num <= 0)
                {
                    int error_num;
                    BZ2_bzerror(m_file, &error_num);
                    if (error_num == BZ_STREAM_END)
                    {
                        setStatus(error_end_of_stream);
                    }
                    else
                    {
                        setStatus(error_read_from_stream);
                    }
                    break;
                }
                read_bytes += num;
            }

            return read_bytes;
        }

        uint64 Bz2FileStream::WriteBytes(uint64 nBytes, const void *bytes)
        {
            if (nBytes == 0)
            {
                return 0;
            }

            // check for an error
            if (Error())
            {
                return 0;
            }

            // check mode
            if (!isWriting())
            {
                setStatus(error_stream_direction);
                return 0;
            }

            uint64 wrote_bytes = 0;
            while(wrote_bytes < nBytes)
            {
                int num = ::BZ2_bzwrite(m_file, const_cast<uchar*>(static_cast<const uchar*>(bytes)+wrote_bytes),  
                    int(nBytes-wrote_bytes));
                if (num <= 0)
                {
                    setStatus(error_write_to_stream);
                    break;
                }
                wrote_bytes += num;
            }

            return wrote_bytes;
        }

        void Bz2FileStream::Flush()
        {
            // check for an error
            if (Error())
            {
                return;
            }

            // check correct stream direction
            if (isWriting())
            {
                ::BZ2_bzflush(m_file);
            }
            else
            {
                setStatus(error_stream_direction);
            }
        }

        bool Bz2FileStream::SetPosition(sint64, StreamPosition)
        {
            setStatus(error_set_stream_position);
            return false;
        }

        uint64 Bz2FileStream::GetPosition() const
        {
            setStatus(error_set_stream_position);
            return 0;
        }

    }
}
