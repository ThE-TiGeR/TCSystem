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
//  $Id: TCOpenGLTexture2D.cpp,v 0048256f023c 2012/03/29 22:03:34 thomas $
//*******************************************************************************

#include "TCOpenGLTexture2D.h"

#include "TCOpenGLViewer.h"
#include "TCOpenGLUtil.h"
#include "TCOpenGLPatchRenderer.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace open_gl
   {
      Texture2D::Texture2D(const std::string& name)
         :Object2D(name)
         ,m_size()
         ,m_texture_id(0)
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
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, m_texture_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            DetectOpenGLerror();

            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            Vertex2D texture_size(m_size[0]/viewport[2], m_size[1]/viewport[3]);

            ::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBegin(GL_QUADS);
            glTexCoord2d(0,         1);          glVertex3f(-texture_size[0], -texture_size[1], 0);        
            glTexCoord2d(1, 1);                  glVertex3f( texture_size[0], -texture_size[1], 0);        
            glTexCoord2d(1, 0);                  glVertex3f( texture_size[0],  texture_size[1], 0);
            glTexCoord2d(0,         0);          glVertex3f(-texture_size[0],  texture_size[1], 0);
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
