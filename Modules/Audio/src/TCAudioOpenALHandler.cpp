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
//  $Id: TCAudioOpenALHandler.cpp,v 89939db0b777 2012/09/24 21:17:18 thomas $
//*******************************************************************************

#include "TCAudioOpenALHandler.h"

#include "TCException.h"
#include "TCOutput.h"
#include "TCString.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace audio
   {
      class ExceptionOpenALError: public Exception
      {
      public:
         ExceptionOpenALError(const char* function)
             :Exception(string::Print("OpenAL error in function '%s'", function))
         {
         }

         ExceptionOpenALError(const char* function, ALenum error)
             :Exception(string::Print("OpenAL error in function '%s' error_code = %d", function, error))
         {
         }
      };

      OpenALHandler::OpenALHandler()
      {
         OpenDevice();
         CreateContext();
         InitListener();
      }

      OpenALHandler::~OpenALHandler()
      {
         if (m_context)
         {
            ::alcDestroyContext(m_context);
         }

         if (m_device)
         {
            ::alcCloseDevice(m_device);
         }
      }

      bool OpenALHandler::CheckError() const
      {
         return ::alGetError() == AL_NO_ERROR;
      }

      void OpenALHandler::CheckErrorAndThrowException(const char* function) const
      {
         ALenum error;
         if ((error = ::alGetError()) != AL_NO_ERROR)
         {
            throw ExceptionOpenALError(function, error);
         }
      }

      ALuint OpenALHandler::CreateNewSource()
      {
         ALuint source;
         // Generate the source
         ::alGenSources(1, &source);
         CheckErrorAndThrowException("alGenSources");

         return source;
      }

      ALuint OpenALHandler::GetUnusedSource()
      {
         ALuint source;
         if (m_free_sources.size() > 0)
         {
            source = m_free_sources.back();
            m_free_sources.pop_back();
         }
         else
         {
            source = CreateNewSource();
         }

         ALfloat values[3] = {0, 0, 0};
         ::alSourcefv(source, AL_POSITION, values);
         ::alSourcefv(source, AL_VELOCITY, values);
         ::alSourcefv(source, AL_DIRECTION, values);
         CheckErrorAndThrowException("alSourcefv");

         return source;
      }

      void OpenALHandler::ReleaseSource(ALuint source)
      {
         m_free_sources.push_back(source);
      }

      ALuint OpenALHandler::CreateNewBuffer()
      {
         ALuint source;
         // Generate the source
         ::alGenBuffers(1, &source);
         CheckErrorAndThrowException("alGenBuffers");

         return source;
      }

      ALuint OpenALHandler::GetUnusedBuffer()
      {
         ALuint buffer;
         if (m_free_buffers.size() > 0)
         {
            buffer = m_free_buffers.back();
            m_free_buffers.pop_back();
         }
         else
         {
            buffer = CreateNewBuffer();

         }
         return buffer;
      }

      void OpenALHandler::ReleaseBuffer(ALuint buffer)
      {
         m_free_buffers.push_back(buffer);
      }

      void OpenALHandler::OpenDevice()
      {
         m_device = ::alcOpenDevice(0);
         if (m_device == 0)
         {
            throw ExceptionOpenALError("alcOpenDevice(0)");
         }
      }

      void OpenALHandler::CreateContext()
      {
         m_context = ::alcCreateContext(m_device, 0);
         if (m_context == 0)
         {
            throw ExceptionOpenALError("alcCreateContext(m_device, 0)");
         }

         if (!::alcMakeContextCurrent(m_context))
         {
            throw ExceptionOpenALError("alcMakeContextCurrent(m_context)");
         }
      }

      void OpenALHandler::InitListener()
      {
         ALfloat values[6] = {0, 0, 0, 0, 0, 0};
         ::alListenerfv(AL_POSITION, values);
         ::alListenerfv(AL_VELOCITY, values);
         ::alListenerfv(AL_ORIENTATION, values);
         CheckErrorAndThrowException("alListenerfv");
      }
   }
}
