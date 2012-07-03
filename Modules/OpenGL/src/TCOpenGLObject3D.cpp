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

#include "TCOpenGLObject3D.h"

#include "TCOpenGLLineRenderer.h"
#include "TCOpenGLViewer.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace open_gl
   {
      Object3D::Object3D(const std::string& name)
         :Object(name),
         m_position(),
         m_bounding_box(),
         m_selection_color(0.5f,0.3f,0.45f),
         m_bounding_box_color(0.2f,0.7f,0.85f)
      {
      }

      Object3D::~Object3D()
      {
      }

      void Object3D::Draw(Viewer& viewer)
      {
         ::glTranslatef(m_position[0], m_position[1], m_position[2]);

         DrawSelf(viewer);

         if(GetOption(OP_STYLE_BOUNDBOX) || GetOption(OP_SELECTED))
         {
             DrawBoundingBox();
         }

         if(GetOption(OP_SELECTED))
         {
             DrawHandles();
         }
         

         ::glTranslatef(-m_position[0], -m_position[1], -m_position[2]);
      }

      void Object3D::DrawForHit(Viewer& viewer) 
      {
         ::glTranslatef(m_position[0], m_position[1], m_position[2]);

         DrawForHitSelf(viewer);

         ::glTranslatef(-m_position[0], -m_position[1], -m_position[2]);
      }

      ObjectPtr Object3D::GetHitObject(const uint32_t* /*hitpath*/)
      {
         return ObjectPtr();
      }

      void Object3D::GetBoundingBox(BoundingBox3D& bounding_box)
      {
         bounding_box = m_bounding_box;
      }

      void Object3D::Update(Viewer& viewer, uint32_t curent_time, uint32_t delta_time) 
      {
         UpdateSelf(viewer, curent_time, delta_time);
      }

      bool Object3D::Drag(Viewer& viewer, int32_t fx, int32_t fy, int32_t tx, int32_t ty)
      {
         float zz = viewer.worldToEyeZ(m_position);
         Vertex3D wf = viewer.eyeToWorld(viewer.screenToEye(fx, fy, zz));
         Vertex3D wt = viewer.eyeToWorld(viewer.screenToEye(tx, ty,zz));
         m_position += wt - wf;

         return true;
      }

      void Object3D::DrawBoundingBox()
      {
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

      void Object3D::DrawHandles()
      {
         ::glColor4fv(m_selection_color);
         ::glPointSize(5);

         ::glBegin(GL_POINTS);
         {
             ::glVertex3f(m_bounding_box.GetMin()[0], m_bounding_box.GetMin()[1], m_bounding_box.GetMin()[2]);
             ::glVertex3f(m_bounding_box.GetMin()[0], m_bounding_box.GetMin()[1], m_bounding_box.GetMax()[2]);
             ::glVertex3f(m_bounding_box.GetMin()[0], m_bounding_box.GetMax()[1], m_bounding_box.GetMin()[2]);
             ::glVertex3f(m_bounding_box.GetMin()[0], m_bounding_box.GetMax()[1], m_bounding_box.GetMax()[2]);
             ::glVertex3f(m_bounding_box.GetMax()[0], m_bounding_box.GetMin()[1], m_bounding_box.GetMin()[2]);
             ::glVertex3f(m_bounding_box.GetMax()[0], m_bounding_box.GetMin()[1], m_bounding_box.GetMax()[2]);
             ::glVertex3f(m_bounding_box.GetMax()[0], m_bounding_box.GetMax()[1], m_bounding_box.GetMin()[2]);
             ::glVertex3f(m_bounding_box.GetMax()[0], m_bounding_box.GetMax()[1], m_bounding_box.GetMax()[2]);
         }
         ::glEnd();
      }

      void Object3D::SetBoundingBox(const BoundingBox3D& bounding_box)
      {
         m_bounding_box = bounding_box;
      }

      void Object3D::ResetPosition()
      {
         m_position = Vertex3D();
      }

      FXIMPLEMENT_ABSTRACT(Object3D, Object, 0, 0);
   }
}
