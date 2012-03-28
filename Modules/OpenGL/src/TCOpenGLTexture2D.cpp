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

#include "TCOpenGLTexture2D.h"

#include "TCOpenGLViewer.h"
#include "TCOpenGLUtil.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace open_gl
   {
      Texture2D::Texture2D(const std::string& name)
         :Object2D(name)
         ,m_texture_id(0)
         ,m_size()
      {
      }

      Texture2D::~Texture2D()
      {
         DeleteTexture();
      }

      bool Texture2D::CreateTexture(const Vertex2D& size, const Uint32Vector& data)
      {
         DeleteTexture();

         if (size.Length2() && data.size())
         {
            glGenTextures(1, &m_texture_id);
            glBindTexture(GL_TEXTURE_2D, m_texture_id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            m_size = size;
            glTexImage2D(GL_TEXTURE_2D, 0, 4, m_size[0], m_size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, &data.front());
            DetectOpenGLerror();

            glBindTexture(GL_TEXTURE_2D, 0);
         }

         return true;
      }

      void Texture2D::DrawSelf(Viewer& /*viewer*/)
      {
         if (m_texture_id)
         {
            glDisable(GL_COLOR_MATERIAL);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, m_texture_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            DetectOpenGLerror();

            glBegin(GL_QUADS);
            glTexCoord2d(0,         m_size[1]/2);          glVertex2f(-m_size[0]/2, -m_size[1]/2);        
            glTexCoord2d(m_size[0]/2, m_size[1]/2);        glVertex2f( m_size[0]/2, -m_size[1]/2);        
            glTexCoord2d(m_size[0]/2, 0);                  glVertex2f( m_size[0]/2,  m_size[1]/2);
            glTexCoord2d(0,         0);                    glVertex2f(-m_size[0]/2,  m_size[1]/2);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
            DetectOpenGLerror();
         }
      }

      void Texture2D::DeleteTexture()
      {
         if (m_texture_id)
         {
            glDeleteTextures(1, &m_texture_id);
            m_texture_id = 0;
         }
      }

   }
}
