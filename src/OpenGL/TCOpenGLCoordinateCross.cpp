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
//  $Id: TCOpenGLCoordinateCross.cpp 984 2010-05-12 08:33:52Z the_____tiger $
//*******************************************************************************

#include "TCOpenGLCoordinateCross.h"

#include "TCOpenGLViewer.h"
#include "TCOpenGLTextRenderer.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace OpenGL
   {
      CoordinateCross::CoordinateCross(const std::string& name)
         :Object3D(name)
      {
         SetOption(OP_DRAG_ALLOWED, false);

         m_quadric = ::gluNewQuadric();
      }

      CoordinateCross::~CoordinateCross()
      {
         ::gluDeleteQuadric(m_quadric);
      }

      void CoordinateCross::DrawForHitSelf(Viewer& /*viewer*/)
      {
      }

      void CoordinateCross::DrawSelf(Viewer& viewer) 
      {
         // save gl settings and matrix
         GLdouble matrix1[16];
         GLdouble matrix2[16];
         glPushAttrib(GL_ALL_ATTRIB_BITS);
         glGetDoublev(GL_PROJECTION_MATRIX, matrix1);
         glGetDoublev(GL_MODELVIEW_MATRIX,  matrix2);

         // switch back to the device coord system
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

         FX::FXMat4f transformation;
         transformation.eye();
         transformation.rot(viewer.getOrientation());

         Viewport viewport;
         viewer.getViewport(viewport);
         float aspect = static_cast<float>(viewport.w)/static_cast<float>(viewport.h);

         ::glShadeModel(GL_SMOOTH);
         ::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         ::glFrontFace(GL_CW);
         ::glEnable(GL_LIGHTING);
         ::glEnable(GL_COLOR_MATERIAL);
         ::glEnable(GL_NORMALIZE);

         TextRenderer::GetInstance().SetFont(viewer.getApp()->getNormalFont());

         ::glPushMatrix();
         {
            ::glTranslated(-0.9, -0.9, 0.0);
            ::glScaled(0.003, 0.003 * aspect, -0.003);
            ::glMultMatrixf(transformation);

            ::glPushMatrix();
            {
               ::glColor3ub(255, 0, 0);
               ::gluSphere(m_quadric, 4, 16, 16);
               TextRenderer::GetInstance().Render3DText("X", 
                  Vertex3D(40, 0, 0), Color(255, 0, 0));
               DrawAxisArrow();
            }
            ::glPopMatrix();

            ::glPushMatrix();
            {
               ::glColor3ub(0, 255, 0);
               ::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
               TextRenderer::GetInstance().Render3DText("Y", 
                  Vertex3D(40, 0, 0), Color(0, 255, 0));
               DrawAxisArrow();
            }
            ::glPopMatrix();

            ::glPushMatrix();
            {
               ::glColor3ub(0, 0, 255);
               ::glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
               TextRenderer::GetInstance().Render3DText("Z", 
                  Vertex3D(40, 0, 0), Color(0, 0, 255));
               DrawAxisArrow();
            }
            ::glPopMatrix();
         }
         ::glPopMatrix();

         ::glFrontFace(GL_CCW);

         // restore state and matrices
         glMatrixMode(GL_PROJECTION);
         glLoadMatrixd(matrix1);
         glMatrixMode(GL_MODELVIEW);
         glLoadMatrixd(matrix2);
         glPopAttrib();
      }

      void CoordinateCross::DrawAxisArrow()
      {
         // Cylinder
         ::glPushMatrix();
         ::glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
         ::gluCylinder(m_quadric, 1.75, 1.75, 20, 16, 1);
         ::glPopMatrix();

         // Cone
         ::glPushMatrix();
         ::glTranslatef(20, 0.0f, 0.0f);
         ::glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
         ::gluCylinder(m_quadric, 5, 0, 14, 16, 1);
         ::glPopMatrix();

         // Cone tapping
         ::glPushMatrix();
         ::glTranslatef(20, 0.0f, 0.0f);
         ::glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
         ::gluDisk(m_quadric, 1.75, 5, 16, 1);
         ::glPopMatrix();
      }

      void CoordinateCross::UpdateSelf(Viewer& /*viewer*/, uint32 /*curent_time*/, uint32 /*delta_time*/)
      {
      }
   }
}

