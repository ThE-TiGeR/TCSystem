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
//  $Id: TCAudioManager.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCAudioManager.h"

#include "TCAudioOpenALHandler.h"
#include "TCAudioSoundImp.h"
#include "TCAudioSoundSequenceImp.h"
#include "TCAudioSoundMultitrackImp.h"
#include "TCAudioSoundDataMapper.h"
#include "TCAudioSoundDataWav.h"
#include "TCAudioSoundDataMp3.h"
#include "TCAudioStreamingThread.h"
#include "TCException.h"
#include "TCFactory.h"
#include "TCFileName.h"
#include "TCOutput.h"
#include "TCMTFactory.h"
#include "TCSystem.h"
#include "TCUtil.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Audio
   {
      class ExceptionInitError: public Exception
      {
      public:
         ExceptionInitError(const char* error_string)
            :Exception(error_string)
         {
         }
      };

      class ManagerImp: public Manager
      {
      public:
         ManagerImp();
         virtual ~ManagerImp();

         virtual SoundPtr CreateSound(const std::string&);
         virtual SoundPtr CreateSound(const std::string&, StreamPtr stream);
         virtual SoundCompoundPtr CreateSoundSequence(const std::string& name);
         virtual SoundCompoundPtr CreateSoundMultitrack(const std::string& name);
         virtual void DeleteSound(SoundPtr& sound);
         virtual void DeleteSound(SoundCompoundPtr& sound);

      private:
         MT::ThreadPtr m_streaming_thread;
         StreamingThreadPtr m_streaming_thread_object;
         SoundDataMapperPtr m_sound_data_mapper;
      };

      ManagerImp::ManagerImp()
      {
         OpenALHandler::CreateInstance();

         m_streaming_thread_object = StreamingThreadPtr(new StreamingThread);
         if (!m_streaming_thread_object)
         {
            throw ExceptionInitError("ManagerImp::ManagerImp, new TC::Audio::StreamingThread failed");
         }

         m_streaming_thread = MT::Factory::CreateThread("TCAudioStreamingThread", 0 , MT::Thread::PRIORITY_HIGH);
         if (!m_streaming_thread)
         {
            throw ExceptionInitError("ManagerImp::ManagerImp, TC::MT::Factory::CreateThread(TCAudioStreamingThread) failed");
         }

         if (!m_streaming_thread->Start(m_streaming_thread_object))
         {
            throw ExceptionInitError("ManagerImp::ManagerImp, m_streaming_thread->Start failed");
         }

         if (!m_streaming_thread_object->WaitUntilRunning())
         {
            throw ExceptionInitError("ManagerImp::ManagerImp, m_streaming_thread->WaitUntilRunning failed");
         }

         m_sound_data_mapper = SoundDataMapperPtr(new SoundDataMapper);
         if (!m_sound_data_mapper)
         {
            throw ExceptionInitError("ManagerImp::ManagerImp, new TC::Audio::SoundDataMapper failed");
         }
      }

      ManagerImp::~ManagerImp()
      {
         m_streaming_thread_object->Stop();
         m_streaming_thread->Join();
         m_streaming_thread = MT::ThreadPtr();

         OpenALHandler::DestroyInstance();
      }

      SoundPtr ManagerImp::CreateSound(const std::string& file_name)
      {
         try
         {
            CodecPtr codec = Factory::CreateLittleEndianBinaryCodec();
            StreamPtr file = Factory::CreateFileStream(file_name, Stream::stream_read, codec);
            if (!file)
            {
               TCERROR1("TCAUDIO", "ManagerImp::CreateSound open file %s failed.", file_name.c_str());
               return SoundPtr();
            }

            return CreateSound(FileName::GetName(file_name), file);
         }

         catch (std::exception& e)
         {
            TCERROR2("TCAUDIO", "from file %s failed %s", file_name.c_str(), e.what());
            return SoundPtr();
         }
      }

      SoundPtr ManagerImp::CreateSound(const std::string& name, StreamPtr stream)
      {
         try
         {
            SharedPtr<SoundDataMp3> sound_data(new TC::Audio::SoundDataMp3(stream));
            SoundPtr sound (new SoundImp(sound_data, m_streaming_thread_object));
            m_sound_data_mapper->Add(sound, sound_data);

            return sound;
         }

         catch (std::exception& e)
         {
            TCERROR2("TCAUDIO", "from stream %s failed %s", name.c_str(), e.what());
            return SoundPtr();
         }
      }

      SoundCompoundPtr ManagerImp::CreateSoundSequence(const std::string&)
      {
         try
         {
            SoundDataSequencePtr sound_data(new SoundDataSequence);
            SoundCompoundPtr sound_sequence(new SoundSequenceImp(sound_data, 
               m_streaming_thread_object, m_sound_data_mapper));

            m_sound_data_mapper->Add(sound_sequence, sound_data);

            return sound_sequence;
         }

         catch (std::exception& e)
         {
            TCERROR1("TCAUDIO", "failed %s", e.what());
            return SoundCompoundPtr();
         }
     }

      SoundCompoundPtr ManagerImp::CreateSoundMultitrack(const std::string&)
      {
         try
         {
            SoundDataMultitrackPtr sound_data(new SoundDataMultitrack);
            SoundCompoundPtr sound_multitrack(new SoundMultitrackImp(sound_data, 
               m_streaming_thread_object, m_sound_data_mapper));

            m_sound_data_mapper->Add(sound_multitrack, sound_data);

            return sound_multitrack;
         }

         catch (std::exception& e)
         {
            TCERROR1("TCAUDIO", "failed %s", e.what());
            return SoundCompoundPtr();
         }
      }

      void ManagerImp::DeleteSound(SoundPtr& sound)
      {
         if (!sound)
         {
            return;
         }

         try
         {
            sound->Stop();
            m_sound_data_mapper->Remove(sound);
         }

         catch (std::exception& e)
         {
            TCERROR1("TCAUDIO", "failed %s", e.what());
         }

         sound = SoundPtr();
      }

      void ManagerImp::DeleteSound(SoundCompoundPtr& sound_c)
      {
         SoundPtr sound = sound_c;
         DeleteSound(sound);

         sound_c = SoundCompoundPtr();
      }

      TC::Audio::ManagerPtr Manager::Create()
      {
         try
         {
            return ManagerPtr(new ManagerImp);
         }

         catch (std::exception& e)
         {
            TCERROR1("TCAUDIO", "failed %s", e.what());
            return ManagerPtr();
         }
      }
   }
}
