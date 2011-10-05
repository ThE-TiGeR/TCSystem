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

#include "TCOpenGLCrossHair.h"

#include "TCOpenGLViewer.h"
#include "TCOpenGLTextRenderer.h"
#include "TCOpenGLLineRenderer.h"
#include "TCString.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace OpenGL
   {

      CrossHair::CrossHair(const std::string& name)
         :Object2D(name)
      {
      }

      CrossHair::~CrossHair()
      {
      }

      void CrossHair::DrawSelf(Viewer& viewer)
      {
         GLint viewport[4];
         glGetIntegerv(GL_VIEWPORT, viewport);
         float aspect = (float)viewport[2]/ (float)viewport[3];

         LineRenderer linerenderer;
         glEnable(GL_BLEND);
         glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);

         ::glColor4fv(Color(1.0, 1.0, 1.0));
         ::glLineWidth(1.0);
         glBegin(GL_LINES);
         glVertex3f(-1.0, 0.0, 0.0);
         glVertex3f(1.0, 0.0, 0.0);
         glVertex3f(0.0, -1.0, 0.0);
         glVertex3f(0.0, 1.0, 0.0);
         glEnd();

         glColor3f(0.9f, 0.9f, 0.9f);
         glBegin(GL_LINE_LOOP);
         for (uint32 i=0;i<32;i++)
         {
            float cosine = cos (i*2*Math::PI/32.0) * 0.33;
            float sine = sin(i*2*Math::PI / 32.0) * 0.33 * aspect;
            glVertex2f(cosine,sine);
         }
         glEnd();

         if (viewer.getProjection() == Viewer::PARALLEL)
         {
            double size=viewer.worldPix()*((double)viewport[2])*.33;
            Vertex3D eyepos=viewer.getEyePosition();
            Vertex3D eyevec=viewer.getEyeVector();
            double diam=viewer.getDiameter();
            double dist=viewer.getDistance();
            eyepos[0]+=(dist+diam)*.1*eyevec[0];
            eyepos[1]+=(dist+diam)*.1*eyevec[1];
            eyepos[2]+=(dist+diam)*.1*eyevec[2];

            TextRenderer::GetInstance().SetFont(viewer.getApp()->getNormalFont());

            char text[128];
            String::Snprintf(text, 128, "Diameter: %8.6g", size);
            TextRenderer::GetInstance().Render3DText(text, 
               Vertex3D(-0.60f, -0.23f, 0.0f), Color(255, 255, 255));

            String::Snprintf(text, 128, "Point : %8.6g %8.6g %8.6g", eyepos[0], eyepos[1], eyepos[2]);
            TextRenderer::GetInstance().Render3DText(text, 
               Vertex3D(0.33f, -0.23f, 0.0f), Color(255, 255, 255));

            String::Snprintf(text, 128, "Vector: %8.6g %8.6g %8.6g", eyevec[0],eyevec[1],eyevec[2]);
            TextRenderer::GetInstance().Render3DText(text, 
               Vertex3D(0.33f, -0.28f, 0.0f), Color(255, 255, 255));
         }     

         ::glDisable(GL_BLEND);
         linerenderer.DisableAntiAliasing();
      }
   }
}
