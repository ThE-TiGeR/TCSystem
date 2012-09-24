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
#ifndef _TC_AUDIO_OPEN_AL_HANDLER_H_
#define _TC_AUDIO_OPEN_AL_HANDLER_H_

#include "TCAudioOpenAL.h"
#include "TCSharedPtr.h"

#include <vector>

namespace tc
{
   namespace audio
   {
      class OpenALHandler;
      typedef SharedPtr<OpenALHandler> OpenALHandlerPtr;

      class TC_DLL_LOCAL OpenALHandler
      {
      public:
         OpenALHandler();
         ~OpenALHandler();

         bool CheckError() const;
         void CheckErrorAndThrowException(const char* text) const;

         ALuint GetUnusedSource();
         void ReleaseSource(ALuint source);

         ALuint GetUnusedBuffer();
         void ReleaseBuffer(ALuint buffer);

      private:
         void OpenDevice();
         void CreateContext();
         void InitListener();
         ALuint CreateNewSource();
         ALuint CreateNewBuffer();

      private:
         ALCdevice* m_device;
         ALCcontext* m_context;
         std::vector<ALuint> m_free_buffers;
         std::vector<ALuint> m_free_sources;
      };
   }
}

#endif // _TC_AUDIO_OPEN_AL_HANDLER_H_
