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

#include "TCOpenGLObject2D.h"

#include "TCOpenGLLineRenderer.h"
#include "TCOpenGLViewer.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace open_gl
   {
      Object2D::Object2D(const std::string& name)
         :Object(name),
         m_position(),
         m_bounding_box(),
         m_selection_color(0.5f,0.3f,0.45f),
         m_bounding_box_color(0.2f,0.7f,0.85f)
      {
      }

      Object2D::~Object2D()
      {
      }

      void Object2D::Draw(Viewer& viewer)
      {
         GLdouble matrix1[16];
         GLdouble matrix2[16];
         glGetDoublev(GL_PROJECTION_MATRIX, matrix1);
         glGetDoublev(GL_MODELVIEW_MATRIX, matrix2);

         // switch back to the device coord system
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

         ::glTranslatef(m_position[0], m_position[1], 0);

         glDisable(GL_DEPTH_TEST);

         DrawSelf(viewer);
         DrawBoundingBox();
         DrawHandles();

         glEnable(GL_DEPTH_TEST);

         ::glTranslatef(-m_position[0], -m_position[1], 0);

         glMatrixMode(GL_PROJECTION);
         glLoadMatrixd(matrix1);
         glMatrixMode(GL_MODELVIEW);
         glLoadMatrixd(matrix2);
      }

      void Object2D::DrawForHit(Viewer& /*viewer*/) 
      {
         //Draw(viewer);
      }

      ObjectPtr Object2D::GetHitObject(const uint32* /*hitpath*/)
      {
         return ObjectPtr();
      }

      void Object2D::GetBoundingBox(BoundingBox2D& bounding_box)
      {
         bounding_box = m_bounding_box;
      }

      void Object2D::GetBoundingBox(BoundingBox3D& bounding_box)
      {
         bounding_box = BoundingBox3D();
      }

      void Object2D::Update(Viewer& /*viewer*/, uint32 /*curent_time*/, uint32 /*delta_time*/) 
      {
      }

      bool Object2D::Drag(Viewer& /*viewer*/, sint32 fx, sint32 fy, sint32 tx, sint32 ty)
      {
         m_position[0] += tx - fx;
         m_position[1] += ty - fy;

         return true;
      }

      void Object2D::DrawBoundingBox()
      {
         if(!GetOption(OP_STYLE_BOUNDBOX) &&
            !GetOption(OP_SELECTED))
         {
            return;
         }

         LineRenderer renderer;
         renderer.EnableLineStipple(0x00ff);
         if (GetOption(OP_SELECTED))
         {
            renderer.RenderBox(m_bounding_box, m_selection_color, LineRenderer::DOUBLE_WIDTH);
         }
         else
         {
            renderer.RenderBox(m_bounding_box, m_bounding_box_color);
         }
         renderer.DisableLineStipple();

      }

      void Object2D::DrawHandles()
      {
         if(!GetOption(OP_SELECTED))
         {
            return;
         }

         ::glColor4fv(m_selection_color);
         ::glPointSize(5);

         ::glBegin(GL_POINTS);
         {
             ::glVertex2f(m_bounding_box.GetMin()[0], m_bounding_box.GetMin()[1]);
             ::glVertex2f(m_bounding_box.GetMin()[0], m_bounding_box.GetMax()[1]);
             ::glVertex2f(m_bounding_box.GetMax()[0], m_bounding_box.GetMin()[1]);
             ::glVertex2f(m_bounding_box.GetMax()[0], m_bounding_box.GetMax()[1]);
         }
         ::glEnd();
      }

      void Object2D::ResetPosition()
      {
         m_position = Vertex2D();
      }

      FXIMPLEMENT_ABSTRACT(Object2D, Object, 0, 0)

   }
}
