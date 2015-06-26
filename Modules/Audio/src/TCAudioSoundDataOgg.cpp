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
//  $Id: TCAudioSoundDataOgg.cpp,v 2d2c7b9f2f6c 2014/09/22 18:55:33 thomas $
//*******************************************************************************

#include "TCAudioSoundDataOgg.h"

#include "TCException.h"
#include "TCFactory.h"
#include "TCString.h"
#include "TCUtil.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      static const uint32_t LITTLE_ENDIAN_BYTE_PACKING = 0;
      static const uint32_t BIG_ENDIAN_BYTE_PACKING    = 1;
      static const uint32_t SAMPLE_SIZE_8              = 1;
      static const uint32_t SAMPLE_SIZE_16             = 2;
      static const uint32_t UNSIGNED_DATA              = 0;
      static const uint32_t SIGNED_DATA                = 1;

      static void CheckError(int error, const char* methode)
      {
         if (error < 0)
         {
            throw Exception(string::Print("Function call '%s' failed with error '%d'", 
               methode, error));
         }
         //          OV_FALSE      -1
         //          OV_EOF        -2
         //          OV_HOLE       -3
         // 
         //          OV_EREAD      -128
         //          OV_EFAULT     -129
         //          OV_EIMPL      -130
         //          OV_EINVAL     -131
         //          OV_ENOTVORBIS -132
         //          OV_EBADHEADER -133
         //          OV_EVERSION   -134
         //          OV_ENOTAUDIO  -135
         //          OV_EBADPACKET -136
         //          OV_EBADLINK   -137
         //          OV_ENOSEEK    -138

      }

      SoundDataOgg::SoundDataOgg(StreamPtr stream)
         :m_sound_format(),
         m_stream(stream->Clone()),
         m_vorbis_file()
      {
         // Set the proper callback functions
         ov_callbacks callbacks;
         callbacks.read_func  = &ReadCallback;
         callbacks.seek_func  = &SeekCallback;
         callbacks.close_func = 0; // File handle owned by input engine!
         callbacks.tell_func  = &TellCallback;

         CheckError(ov_open_callbacks(this, &m_vorbis_file, 0, 0, callbacks), "ov_open_callbacks");
         vorbis_info* info = ov_info(&m_vorbis_file, -1);
         if (!info)
         {
            throw Exception("ov_info failed");
         }

         m_sound_format.SetNumChannels(uint16_t(info->channels));
         m_sound_format.SetBitsPerSample(16);
         m_sound_format.SetSamplesPerSecond(uint32_t(info->rate));
      }

      SoundDataOgg::~SoundDataOgg()
      {
         CheckError(ov_clear(&m_vorbis_file), "ov_clear");
      }

      const SoundFormat& SoundDataOgg::GetFormat() const
      {
         Locker lock(this);
         return m_sound_format;
      }

      uint64_t SoundDataOgg::GetData(uint64_t num_bytes_to_read, uint8_t* buffer)
      {
         Locker lock(this);

         uint64_t bytes_read(0);
         while  (bytes_read < num_bytes_to_read)
         {
            int current_section(0);
            long n_read = ov_read(&m_vorbis_file, reinterpret_cast<char*>(buffer) + bytes_read, 
               (int)(num_bytes_to_read - bytes_read), LITTLE_ENDIAN_BYTE_PACKING, SAMPLE_SIZE_16, SIGNED_DATA, &current_section);

            switch (n_read)
            {
            case 0:
               if (IsLooping())
               {
                  SetToStart();
               }
               else
               {
                  return bytes_read;
               }
               break;

            case OV_HOLE:
            case OV_EBADLINK:
            case OV_EINVAL:
               break;

            default:
               if (n_read > 0)
               {
                  bytes_read +=n_read;
               }
               else
               {
                  CheckError(n_read, "ov_read");
               }
            }
         }

         return bytes_read;
      }

      void SoundDataOgg::SetToStart()
      {
         Locker lock(this);

         ogg_int64_t pos = 0;
         ogg_int64_t bytes_per_sample = m_sound_format.GetBytesPerSample();
         ogg_int64_t samples = pos / bytes_per_sample;
         CheckError(ov_pcm_seek(&m_vorbis_file, samples), "ov_pcm_seek");
      }

      size_t SoundDataOgg::ReadCallback(void *ptr, size_t size, size_t nmemb, void *datasource)
      {
         size_t ret = 0;
         if(size && nmemb)
         {
            SoundDataOgg* ogg = reinterpret_cast<SoundDataOgg*>(datasource);
            ret = ogg->m_stream->ReadBytes(size * nmemb, ptr);
         }
         return ret;
      }

      int SoundDataOgg::SeekCallback(void *datasource, ogg_int64_t offset64, int whence)
      {
         SoundDataOgg* ogg  = reinterpret_cast<SoundDataOgg*>(datasource);

         Stream::Position pos = Stream::Position::SET;

         switch(whence)
         {
         case 0:
            pos = Stream::Position::SET;
            break;
         case 1:
            pos =  Stream::Position::CURRENT;
            break;
         case 2:
            pos = Stream::Position::END;
            break;
         default:
            return -1;
         }

         if (!ogg->m_stream->SetPosition(offset64, pos))
         {
            return -1;
         }
         return 0;
      }

      long SoundDataOgg::TellCallback(void *datasource)
      {
         SoundDataOgg* ogg  = reinterpret_cast<SoundDataOgg*>(datasource);
         return long(ogg->m_stream->GetPosition());
      }

   }
}
