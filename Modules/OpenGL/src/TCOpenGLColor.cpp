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

#include "TCOpenGLColor.h"

#include "TCOpenGL.h"
#include "TCString.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace OpenGL
   {
      TC_CT_ASSERT(sizeof(Color) == 4 * sizeof(float));

      const Color Color::RED(DEFAULT_RED, 0, 0, DEFAULT_ALPHA);
      const Color Color::GREEN(0, DEFAULT_GREEN, 0, DEFAULT_ALPHA);
      const Color Color::BLUE(0, 0, DEFAULT_BLUE, DEFAULT_ALPHA);
      const Color Color::VIOLET(0.5f, 0, DEFAULT_BLUE, DEFAULT_ALPHA);
      const Color Color::YELLOW(DEFAULT_RED, DEFAULT_GREEN, 0, DEFAULT_ALPHA);
      const Color Color::ORANGE(DEFAULT_RED, 0.5f, 0, DEFAULT_ALPHA);
      const Color Color::WHITE(MAX_COLOR_COMPONENT, MAX_COLOR_COMPONENT, MAX_COLOR_COMPONENT, DEFAULT_ALPHA);
      const Color Color::BLACK(0, 0, 0, DEFAULT_ALPHA);
      const Color Color::GRAY(0.5f, 0.5f, 0.5f, DEFAULT_ALPHA);

      Color::Color(uint32 color)
      {
         FromUnsignedInt(color);
      }

      uint32 Color::ToUnsignedInt() const
      {
         return FXRGBA(255.0 * m_red, 255.0 * m_green, 255.0 * m_blue, 255.0 * m_alpha);
      }

      void Color::FromUnsignedInt(uint32 color)
      {
         m_red   = 0.003921568627f * FXREDVAL(color);
         m_green = 0.003921568627f * FXGREENVAL(color);
         m_blue  = 0.003921568627f * FXBLUEVAL(color);
         m_alpha = 0.003921568627f * FXALPHAVAL(color);
      }

      StreamPtr operator>>(StreamPtr stream, Color& color)
      {
         return stream >> color[0] >> color[1] >> color[2] >> color[3];
      }

      StreamPtr operator<<(StreamPtr stream, const Color& color)
      {
         return stream << color[0] << space << color[1] << space << color[2] << space << color[3];
      }
   }
}

