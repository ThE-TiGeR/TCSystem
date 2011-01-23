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

#include "TCStreamBase.h"

#include "TCOutput.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

namespace TC
{
namespace Impl
{

StreamBase::StreamBase(CodecPtr codec)
:m_codec(codec)
{
   ResetStatus();
   m_display_error_messages = true;

   setStreamDirection(stream_dead);
}

StreamBase::~StreamBase()
{
}

void StreamBase::CloseStream()
{
   setStreamDirection(stream_dead);
}

void StreamBase::EnableDisplayErrorMessages(bool displ)
{
   m_display_error_messages = displ;
}

// -----------------------------------------------------------------
// status methodes
// -----------------------------------------------------------------
void StreamBase::setStatus(sint32 err) const
{
   m_status = err;

   // if we shoud we display an error message
   if (m_display_error_messages)
   {
      displayErrorMessage();
      if (GetStatus() != error_none && System::GetLastError() !=0)
      {
         TCERROR("TCBASE", System::GetLastErrorMessage().c_str());
      }
   }
}

void StreamBase::ResetStatus()
{
   m_status = StreamBase::error_none;
}

void StreamBase::displayErrorMessage() const
{
   switch (GetStatus())
   {
      case error_streamopen:
         TCERROR("TCBASE", "Error opening stream");
       break;

      case error_streamclose:
         TCERROR("TCBASE", "Error closing stream");
       break;

      case error_streamdirection:
         TCERROR("TCBASE", "Error wrong stream direction");
       break;
   }
}

}
}
