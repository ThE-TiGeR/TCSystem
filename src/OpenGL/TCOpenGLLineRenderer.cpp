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

#include "TCOpenGLLineRenderer.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace OpenGL
   {
      void LineRenderer::EnableAntiAliasing()
      {
         /* Turn on anti-aliasing.*/
         ::glEnable(GL_LINE_SMOOTH);
         ::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

         /*
         * In RGBA mode, blending must be enabled. 
         */
         ::glEnable(GL_BLEND);
         ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
      }

      void LineRenderer::DisableAntiAliasing()
      {
         /* Turn on anti-aliasing.*/
         ::glDisable(GL_LINE_SMOOTH);
         ::glDisable(GL_BLEND);
      }

      void LineRenderer::EnableLineStipple(uint32 stipple_pattern)
      {
         ::glEnable(GL_LINE_STIPPLE);
         ::glLineStipple(1, stipple_pattern);
      }

      void LineRenderer::DisableLineStipple()
      {
         ::glDisable(GL_LINE_STIPPLE);
      }

      void LineRenderer::RenderLine(const Vertex3DVector& vertexes,
         const Color& color, uint32 num_pixels, GLenum line_type)
      {
         ::glColor4fv(color);
         ::glLineWidth(num_pixels);

         ::glBegin(line_type);
         {
            for (uint32 i = 0; i < vertexes.size(); i++)
            {
               ::glVertex3fv(vertexes[i]);
            }
         }
         ::glEnd();

         glLineWidth(DEFAULT_WIDTH);
      }

      void LineRenderer::RenderLine(const Vertex3DVector& vertexes,
         const std::vector<Color>& colors, uint32 num_pixels, 
         GLenum line_type)
      {
         ::glLineWidth(num_pixels);

         ::glBegin(line_type);
         {
            for (uint32 i = 0; i < vertexes.size(); i++)
            {
               ::glVertex3fv(vertexes[i]);
               ::glColor4fv(colors[i]);
            }
         }
         ::glEnd();

         glLineWidth(DEFAULT_WIDTH);
      }

      void LineRenderer::RenderLines(const std::vector< std::vector< Math::Coord3D< float > > > &vertexes,
         const std::vector< std::vector<Color> >& colors, 
         uint32 num_pixels, GLenum line_type)
      {
         for (uint32 line=0; line<vertexes.size(); line++)
         {
            RenderLine(vertexes[line], colors[line], num_pixels, line_type);
         }
      }

      void LineRenderer::RenderLines(const std::vector< std::vector< Math::Coord3D< float > > > &vertexes,
         const Color& color, uint32 num_pixels,      
         GLenum line_type) 
      {
         for (uint32 line=0; line<vertexes.size(); line++)
         {
            RenderLine(vertexes[line], color, num_pixels, line_type);
         }
      }

      void LineRenderer::RenderBox(const Math::Box< Math::Coord3D< float > >& box,     
         const Color& color, uint32 num_pixels/*=DEFAULT_WIDTH*/) 
      {
         ::glColor4fv(color);
         ::glLineWidth(num_pixels);

         ::glBegin(GL_LINE_LOOP);
         {
            ::glVertex3f(box.GetMax()[0], box.GetMin()[1], box.GetMin()[2]);
            ::glVertex3f(box.GetMax()[0], box.GetMin()[1], box.GetMax()[2]);
            ::glVertex3f(box.GetMin()[0], box.GetMin()[1], box.GetMax()[2]);
            ::glVertex3f(box.GetMin()[0], box.GetMax()[1], box.GetMax()[2]);
            ::glVertex3f(box.GetMax()[0], box.GetMax()[1], box.GetMax()[2]);
            ::glVertex3f(box.GetMax()[0], box.GetMax()[1], box.GetMin()[2]);
            ::glVertex3f(box.GetMin()[0], box.GetMax()[1], box.GetMin()[2]);
            ::glVertex3f(box.GetMin()[0], box.GetMin()[1], box.GetMin()[2]);
         }
         ::glEnd();
         ::glBegin(GL_LINES);
         {
            ::glVertex3f(box.GetMin()[0], box.GetMin()[1], box.GetMin()[2]);
            ::glVertex3f(box.GetMin()[0], box.GetMin()[1], box.GetMax()[2]);
            ::glVertex3f(box.GetMin()[0], box.GetMax()[1], box.GetMin()[2]);
            ::glVertex3f(box.GetMin()[0], box.GetMax()[1], box.GetMax()[2]);
            ::glVertex3f(box.GetMax()[0], box.GetMin()[1], box.GetMin()[2]);
            ::glVertex3f(box.GetMax()[0], box.GetMax()[1], box.GetMin()[2]);
            ::glVertex3f(box.GetMax()[0], box.GetMin()[1], box.GetMax()[2]);
            ::glVertex3f(box.GetMax()[0], box.GetMax()[1], box.GetMax()[2]);
         }
         ::glEnd();

         glLineWidth(DEFAULT_WIDTH);
      }

      void LineRenderer::RenderBox(const Math::Box< Math::Coord2D< float > >& box,     
         const Color& color, uint32 num_pixels/*=DEFAULT_WIDTH*/) 
      {
         ::glColor4fv(color);
         ::glLineWidth(num_pixels);

         ::glBegin(GL_LINE_LOOP);
         {
            ::glVertex2f(box.GetMin()[0], box.GetMin()[1]);
            ::glVertex2f(box.GetMin()[0], box.GetMax()[1]);
            ::glVertex2f(box.GetMax()[0], box.GetMax()[1]);
            ::glVertex2f(box.GetMax()[0], box.GetMin()[1]);
         }
         ::glEnd();

         glLineWidth(DEFAULT_WIDTH);
      }

   }
}

