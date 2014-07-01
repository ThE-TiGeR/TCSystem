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
// see https://bitbucket.org/the____tiger/tcsystem for details.
// Copyright (C) 2003 - 2014 Thomas Goessler. All Rights Reserved. 
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

#include "TCStreamBase.h"

#include "TCOutput.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

namespace tc
{
namespace imp
{

StreamBase::StreamBase(CodecPtr codec)
:m_codec(codec)
{
   ResetStatus();
   m_display_error_messages = true;

   SetStreamDirection(STREAM_DEAD);
}

StreamBase::~StreamBase()
{
}

void StreamBase::CloseStream()
{
   SetStreamDirection(STREAM_DEAD);
}

void StreamBase::EnableDisplayErrorMessages(bool displ)
{
   m_display_error_messages = displ;
}

// -----------------------------------------------------------------
// status methodes
// -----------------------------------------------------------------
void StreamBase::SetStatus(int32_t err) const
{
   m_status = err;

   // if we shoud we display an error message
   if (m_display_error_messages)
   {
      DisplayErrorMessage();
      if (GetStatus() != ERROR_NONE && system::GetLastError() !=0)
      {
         TCERRORS("TCBASE", system::GetLastErrorMessage().c_str());
      }
   }
}

void StreamBase::ResetStatus()
{
   m_status = StreamBase::ERROR_NONE;
}

void StreamBase::DisplayErrorMessage() const
{
   switch (GetStatus())
   {
      case ERROR_STREAM_OPEN:
         TCERRORS("TCBASE", "Error opening stream");
       break;

      case ERROR_STREAM_CLOSE:
         TCERRORS("TCBASE", "Error closing stream");
       break;

      case ERROR_STREAM_DIRECTION:
         TCERRORS("TCBASE", "Error wrong stream direction");
       break;

      case ERROR_READ_FROM_STREAM:
         TCERRORS("TCBASE", "Error during read from stream");
         break;

      case ERROR_WRITE_TO_STREAM:
         TCERRORS("TCBASE", "Error during write to stream");
         break;

      case ERROR_END_OF_STREAM:
         break;

      case ERROR_SET_STREAM_POSITION:
         TCERRORS("TCBASE", "Set or get position of stream not supported");
         break;

   }
}

}
}
