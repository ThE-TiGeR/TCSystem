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
//  $Id: TCAudioSoundDataMp3.cpp,v 0282dfff03ce 2012/10/08 20:46:40 thomas $
//*******************************************************************************

#include "TCAudioSoundDataMp3.h"

#include "TCException.h"
#include "TCFactory.h"
#include "TCString.h"
#include "TCUtil.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      static void CheckError(int error, const char* methode)
      {
         if (error < MPG123_OK)
         {
            throw Exception(string::Print("Function call '%s' failed with error '%s'", 
               methode, ::mpg123_plain_strerror(error)));
         }
      }

      class TC_DLL_LOCAL Mp3Initializer
      {
      public:
         Mp3Initializer()
         {
            CheckError(::mpg123_init(), "mpg123_init");
         }

         ~Mp3Initializer()
         {
            ::mpg123_exit();
         }
      };

      SoundDataMp3::SoundDataMp3(StreamPtr stream)
         :m_sound_format(),
         m_stream(stream->Clone()),
         m_mp3_handle(0)
      {
         static Mp3Initializer s_mp3_init;

         int ret;
         m_mp3_handle = ::mpg123_new(0, &ret);
         CheckError(ret, "mpg123_new");
         CheckError(::mpg123_param(m_mp3_handle, MPG123_RESYNC_LIMIT, -1, 0), "mpg123_param");
         CheckError(::mpg123_replace_reader_handle(m_mp3_handle, Mp3Read, Mp3Seek, Mp3Cleanup), "mpg123_replace_reader");
         CheckError(::mpg123_open_handle(m_mp3_handle, this), "mpg123_open_fd");

         long rate;
         int channels;
         int encoding;
         CheckError(::mpg123_getformat(m_mp3_handle, &rate, &channels, &encoding), "mpg123_getformat");

         if (encoding & MPG123_ENC_8)
         {
             m_sound_format.SetBitsPerSample(8);
         }
         else if (encoding & MPG123_ENC_16)
         {
             m_sound_format.SetBitsPerSample(6);
         }
         else if (encoding & MPG123_ENC_32)
         {
             m_sound_format.SetBitsPerSample(32);
         }
         else
         {
            throw Exception("Unknown MP3 encoding");
         }

         m_sound_format.SetNumChannels(uint16_t(channels));
         m_sound_format.SetSamplesPerSecond(rate);
      }

      SoundDataMp3::~SoundDataMp3()
      {
         if (m_mp3_handle)
         {
            ::mpg123_delete(m_mp3_handle);
         }
      }

      const SoundFormat& SoundDataMp3::GetFormat() const
      {
         Locker lock(this);
         return m_sound_format;
      }

      uint64_t SoundDataMp3::GetData(uint64_t num_bytes_to_read, uint8_t* buffer)
      {
         Locker lock(this);

         uint64_t bytes_read(0);
         while  (bytes_read < num_bytes_to_read)
         {
            ::size_t n_read(0);
            CheckError(::mpg123_read(m_mp3_handle, buffer + bytes_read, ::size_t(num_bytes_to_read - bytes_read), &n_read), "mpg123_read"); 

            if (n_read == 0) // EOF reached
            {
               if (IsLooping())
               {
                  SetToStart();
               }
               else
               {
                  break;
               }
            }

            bytes_read += n_read;
         }

         return bytes_read;
      }

      void SoundDataMp3::SetToStart()
      {
         Locker lock(this);
         if (::mpg123_seek(m_mp3_handle, 0, SEEK_SET) < 0)
         {
             throw Exception("mpg123_seek error");
         }
      }

      void SoundDataMp3::Mp3Cleanup(void* /*h*/)
      {
      }

      ::ssize_t SoundDataMp3::Mp3Read(void* h, void *buf, ::size_t cnt)
      {
         SoundDataMp3* mp3_data = (SoundDataMp3*)h;
         StreamPtr stream = mp3_data->m_stream;
         return ::ssize_t(stream->ReadBytes(cnt, buf));
      }

      ::off_t SoundDataMp3::Mp3Seek(void* h, ::off_t of, int origin)
      {
         SoundDataMp3* mp3_data = (SoundDataMp3*)h;
         StreamPtr stream = mp3_data->m_stream;
         Stream::StreamPosition pos = Stream::POSITION_CURRENT;
         switch(origin)
         {
         case SEEK_SET:
            pos = Stream::POSITION_SET;
            break;
         case SEEK_CUR:
            pos = Stream::POSITION_CURRENT;
            break;
         case SEEK_END:
            pos = Stream::POSITION_END;
            break;
         }

         stream->SetPosition(of, pos);
         return off_t(stream->GetPosition());
      }
   }
}
