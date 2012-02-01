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

#ifndef _TC_OPENGL_LINE_RENDERER_H
#define _TC_OPENGL_LINE_RENDERER_H

#include "TCOpenGL.h"
#include "TCOpenGLTypes.h"

namespace tc
{
   namespace open_gl
   {
      class TCOGL_API LineRenderer
      {
      public:
         /** Line thickness constatnst */
         enum LineWidthConstant
         {
            /** the default width of a line (1 pixel) */
            DEFAULT_WIDTH = 1,
            /** a line that is 2 pixels wide */
            DOUBLE_WIDTH = 2,
            /** a line that is 3 pixels wide */
            TRIPLE_WIDTH = 3,
            /** a line that is 5 pixels wide */
            QUADRUPLE_WIDTH = 4
         };

         /** Enable anti aliasing for lines */
         void EnableAntiAliasing();
         /** Disable anti aliasing for lines */
         void DisableAntiAliasing();

         /**
         * This method enables rendering lines with specified sipple pattern.
         * Stipple: <BR>
         * (1) : to engrave by means of dots and flicks
         * (2)a : to make by small short touches (as of paint or ink) that together
         *        produce an even or softly graded shadow 
         * (2)b : to apply (as paint) by repeated small touches
         * (3) : SPECKLE, FLECK
         * @see www.m-w.com
         * 
         * To make stippled (dotted or dashed) lines, you use the command
         * glLineStipple() to define the stipple pattern, and then you enable 
         * line stippling with glEnable().
         * <PRE>
         *  glLineStipple(1, 0x3F07);
         *  glEnable(GL_LINE_STIPPLE);
         *
         *  void glLineStipple(GLint factor, GLushort pattern);
         * </PRE>
         * "Sets the current stippling pattern for lines. The pattern argument
         * is a 16-bit series of 0s and 1s, and it's repeated as necessary to
         * stipple a given line. A 1 indicates that drawing occurs, and 0
         * that it does not, on a pixel-by-pixel basis, beginning with the
         * low-order bit of the pattern. The pattern can be stretched out by
         * using factor, which multiplies each subseries of consecutive 1s
         * and 0s. Thus, if three consecutive 1s appear in the pattern,
         * they're stretched to six if factor is 2. factor is clamped to lie
         * between 1 and 255. Line stippling must be enabled by passing
         * GL_LINE_STIPPLE to glEnable(); it's disabled by passing the same
         * argument to glDisable().
         * <P>
         * With the preceding example and the pattern 0x3F07 (which translates to
         * 0011111100000111 in binary), a line would be drawn with 3 pixels on, 
         * then 5 off, 6 on, and 2 off. (If this seems backward, remember that the
         * low-order bit is used first.) If factor had been 2, the pattern would 
         * have been elongated: 6 pixels on, 10 off, 12 on, and 4 off.
         * (Use glDisable() with GL_LINE_STIPPLE to disable stippling.)
         * Note that stippling can be used in combination with wide lines to produce
         * wide stippled lines.
         * <P>
         * This method will eventually be part of a more object-oriented design of
         * the open_gl viewer.  The design cannot be implemented until
         * I can check out the OglViewer shared library.  I've been
         * waiting for 2 weeks now (19 Sep 2001) to add new objects and new 
         * files to this library, but it is always checked out (by someone else).
         * 
         * @see  http://www.opengl.org/developers/faqs/technical/transparency.htm
         * @param stipple_pattern  The pattern to use for rendering lines
         */
         void EnableLineStipple(uint32 stipple_pattern);
         /** Disable rendering lines with specified stipple pattern */
         void DisableLineStipple();

         /**
         * This method renders an Open GL line because of specified line type.
         *
         * @param vertexes   The 3D vertexes of the line to be rendered
         * @param color      The color of the line including alpha value
         * @param num_pixels The thickness of the line in pixel (default = 1)
         * @param line_type  The type of the lines - GL_LINE_STRIP (default)
         *                                         - GL_LINES
         *                                         - GL_LINE_LOOP
         */
         void RenderLine(const Vertex3DVector& vertexes,
                         const Color& color, uint32 num_pixels=DEFAULT_WIDTH, 
                         GLenum line_type=GL_LINE_STRIP);
         /**
         * This method renders an Open GL line because of specified line type.
         *
         * @param vertexes   The 3D vertexes of the line to be rendered
         * @param colors     The color of the line vertex including alpha value
         * @param num_pixels The thickness of the line in pixel (default = 1)
         * @param line_type  The type of the lines - GL_LINE_STRIP (default)
         *                                         - GL_LINES
         *                                         - GL_LINE_LOOP
         */
         void RenderLine(const Vertex3DVector& vertexes,
                         const std::vector<Color>& colors, uint32 num_pixels=DEFAULT_WIDTH, 
                         GLenum line_type=GL_LINE_STRIP);

         /**
         * This method renders a set of lines with specified number of points
         *
         * @param vertexes        The 3D vertexes of all lines to be rendered
         * @param color         The color of the all lines
         */
         void RenderLines(const std::vector< Vertex3DVector > &vertexes,
                          const Color& color, uint32 num_pixels=DEFAULT_WIDTH, 
                          GLenum line_type=GL_LINE_STRIP);
         void RenderLines(const std::vector< Vertex3DVector > &vertexes,
                          const std::vector< std::vector<Color> >& colors, 
                          uint32 num_pixels=DEFAULT_WIDTH, GLenum line_type=GL_LINE_STRIP);

         /**
          * @brief Render specified box in specified color and linewith
          *
          * @param box        The box to render
          * @param colors     The color of the line vertex including alpha value
          * @param num_pixels The thickness of the line in pixel (default = 1)
          */
         void RenderBox(const math::Box< Vertex3D >& box,
                        const Color& color, uint32 num_pixels=DEFAULT_WIDTH);

         /**
          * @brief Render specified box in specified color and linewith
          *
          * @param box        The box to render
          * @param colors     The color of the line vertex including alpha value
          * @param num_pixels The thickness of the line in pixel (default = 1)
          */
         void RenderBox(const math::Box< Vertex2D >& box,
                        const Color& color, uint32 num_pixels=DEFAULT_WIDTH);
      };
   }
}

#endif // _TC_OPENGL_LINE_RENDERER_H
