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
#include "TCGzFileStream.h"

#include "TCOutput.h"

#include "TCNewEnable.h"

namespace TC
{
    namespace Impl
    {
        GzFileStream::GzFileStream(const std::string &fileName, StreamDirection direction, CodecPtr codec)
            :StreamBase(codec)
            ,m_file(0)
        {
            if (direction == stream_read)
            {
                m_file = ::gzopen(fileName.c_str(), "rb");
            }
            else if (direction == stream_write)
            {
                m_file = ::gzopen(fileName.c_str(), "wb");
            }
            else if (direction == stream_readwrite)
            {
                m_file = ::gzopen(fileName.c_str(), "wb+");
            }

            if (!m_file)
            {
                TCERROR1("TCBASE", "Error opening file '%s'", fileName.c_str());
                setStatus(error_streamopen);
            }
            setStreamDirection(direction);
        }

        GzFileStream::~GzFileStream()
        {
            CloseStream();
        }

        void GzFileStream::CloseStream()
        {
            if (m_file)
            {
                ::gzclose(m_file);
                m_file = 0;
            }

            StreamBase::CloseStream();
        }

        uint64 GzFileStream::ReadBytes(uint64 nBytes, void *bytes)
        {
            // check for an error
            if (Error())
            {
                return 0;
            }

            // check mode
            if (!isReading())
            {
                setStatus(error_streamdirection);
                return 0;
            }

            uint64 read_bytes = 0;
            while(read_bytes < nBytes)
            {
                int num = ::gzread(m_file, static_cast<uchar*>(bytes)+read_bytes, 
                    unsigned(nBytes-read_bytes));
                if (num <= 0)
                {
                    if (::gzeof(m_file))
                    {
                        setStatus(error_end_file);
                    }
                    else
                    {
                        setStatus(error_read_file);
                    }
                    break;
                }
                read_bytes += num;
            }

            return read_bytes;
        }

        uint64 GzFileStream::WriteBytes(uint64 nBytes, const void *bytes)
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
                setStatus(error_streamdirection);
                return 0;
            }

            uint64 wrote_bytes = 0;
            while(wrote_bytes < nBytes)
            {
                int num = ::gzwrite(m_file, static_cast<const uchar*>(bytes)+wrote_bytes,  
                    unsigned(nBytes-wrote_bytes));
                if (num <= 0)
                {
                    setStatus(error_write_file);
                    break;
                }
                wrote_bytes += num;
            }

            return wrote_bytes;
        }

        void GzFileStream::Flush()
        {
            // check for an error
            if (Error())
            {
                return;
            }

            // check correct stream direction
            if (isWriting())
            {
                ::gzflush(m_file, Z_PARTIAL_FLUSH);
            }
            else
            {
                setStatus(error_streamdirection);
            }
        }

        void GzFileStream::displayErrorMessage() const
        {
            switch (GetStatus())
            {
            case error_end_file:
                break;

            default:
                StreamBase::displayErrorMessage();
            }
        }

        bool GzFileStream::SetPosition(sint64 pos, StreamPosition pos_mode)
        {
            ResetStatus();

            switch(pos_mode)
            {
            case POSITION_SET:
                return ::gzseek(m_file, ssize_type(pos), SEEK_SET) == 0;

            case POSITION_CURRENT:
                return ::gzseek(m_file, ssize_type(pos), SEEK_CUR) == 0;

            case POSITION_END:
                return ::gzseek(m_file, ssize_type(pos), SEEK_END) == 0;
            }

            return false;
        }

        uint64 GzFileStream::GetPosition() const
        {
            return ::gztell(m_file);
        }

    }
}
