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
//  $Id: TCAudioSoundDataWav.cpp,v 0282dfff03ce 2012/10/08 20:46:40 thomas $
//*******************************************************************************

#include "TCAudioSoundDataWav.h"

#include "TCException.h"
#include "TCFactory.h"
#include "TCUtil.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      SoundDataWav::SoundDataWav(StreamPtr stream)
         :m_sound_format()
         ,m_stream(stream->Clone())
         ,m_start_of_sound_data(0)
      {
         ProcessStream();
      }

      SoundDataWav::~SoundDataWav()
      {
      }

      const SoundFormat& SoundDataWav::GetFormat() const
      {
         Locker lock(this);
         return m_sound_format;
      }

      uint64_t SoundDataWav::GetData(uint64_t num_bytes, uint8_t* buffer)
      {
         Locker lock(this);

         uint64_t n_read = m_stream->ReadBytes(num_bytes, buffer);
         if (n_read < num_bytes && IsLooping())
         {
            m_stream->SetPosition(m_start_of_sound_data, Stream::Position::SET);
            num_bytes -= n_read;
            n_read += m_stream->ReadBytes(num_bytes, buffer);
         }

         return n_read;
      }

      void SoundDataWav::SetToStart()
      {
         Locker lock(this);
         if (!m_stream->SetPosition(m_start_of_sound_data, Stream::Position::SET))
         {
            throw Exception("Error setting stream pointer");
         }
      }

      void SoundDataWav::ProcessStream()
      {
         uint32_t magic_id;
         m_stream >> magic_id; 
         util::SwapBytes(magic_id);
         if (magic_id != 'RIFF')
         {
            throw  Exception("Sound format no RIFF");
         }

         uint32_t chunk_length;
         m_stream >> chunk_length;
         m_stream >> magic_id; 
         util::SwapBytes(magic_id);
         if ( magic_id != 'WAVE')
         {
            throw  Exception("Sound format wrong magic id");
         }

         bool data_processed = false;
         while (!data_processed)
         {
            m_stream >> magic_id; 
            util::SwapBytes(magic_id);
            m_stream >> chunk_length;

            if (magic_id == 'fmt ')
            {
               ProcessFormat(chunk_length);
            }
            else if (magic_id == 'data')
            {
               ProcessData(chunk_length);
               data_processed = true;
            }
            else
            {
               if (!m_stream->SetPosition(chunk_length, Stream::Position::CURRENT))
               {
                  throw Exception("Error setting stream pointer");
               }
            }
         }
      }

      void SoundDataWav::ProcessFormat(uint32_t chunk_length)
      {
         if (chunk_length < 16)
         {
            throw  Exception("Sound format wrong chunk length");
         }

         uint16_t audio_format;      
         uint16_t num_channels;      
         uint32_t samples_per_second;
         uint32_t bytes_per_second;  
         uint16_t bytes_per_sample;  
         uint16_t bits_per_sample;   

         m_stream >> audio_format;
         m_stream >> num_channels;
         m_stream >> samples_per_second;
         m_stream >> bytes_per_second;
         m_stream >> bytes_per_sample;
         m_stream >> bits_per_sample;

         switch (audio_format)
         {
         case 1: /* PCM */
            //                codec = (m_bit_sper_sample == 8 || util::IsLittleEndian()) ?
            //                        _alutCodecLinear : _alutCodecPCM16;
            if (bits_per_sample == 8 || util::IsLittleEndian())
            {
            }
            else
            {
               throw  Exception("Unsupported audio format");
            }
            break;
         case 7: /* uLaw */
            bits_per_sample *= 2;
            //                codec = _alutCodecULaw;
            throw  Exception("Unsupported audio format");
            break;
         default:
            throw  Exception("Unsupported audio format");
         }

         if (!m_stream->SetPosition(chunk_length - 16, Stream::Position::CURRENT))
         {
            throw  Exception("Error setting stream pointer");
         }

         m_sound_format.SetNumChannels(num_channels);
         m_sound_format.SetSamplesPerSecond(samples_per_second);
         m_sound_format.SetBitsPerSample(bits_per_sample);
      }

      void SoundDataWav::ProcessData(uint32_t /*chunk_length*/)
      {
         m_start_of_sound_data = m_stream->GetPosition();
      }
   }
}
