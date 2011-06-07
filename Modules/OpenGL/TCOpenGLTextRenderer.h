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

#ifndef _TC_OPENGL_TEXT_RENDERER_H_
#define _TC_OPENGL_TEXT_RENDERER_H_

#include "TCOpenGL.h"
#include "TCOpenGLTypes.h"

namespace TC
{
   namespace OpenGL
   {

      class TCOGL_API TextRenderer
      {
      public:
         static TextRenderer& GetInstance() { return m_text_renderer; }

         /**
         * set the font for following text rendering
         * @param font the font which should be use for text rendering
         */
         void SetFont(const FX::FXFont *font);
         /**
         * set the font for following text rendering
         * @param font the font which should be use for text rendering
         */
         void SetFont(const FX::FXFontDesc &font);
         /**
         * Renders one text at a 3D position.
         * <P>
         * @parma text the text which should be rendered
         * @param xyzco position where the text should be rendered
         * @param color  color in which the text should be rendered
         */
         void Render3DText(const std::string &text,
            const Vertex3D &xyzco,
            const Color &color);
         /**
         * Renders an arry of texts at 3D positions.
         * <P>
         * @parma textArray the text which should be rendered
         * @param xyzcoArray position where the text should be rendered
         * @param color  color in which the text should be rendered
         */
         void Render3DTexts(const std::vector< std::string > &textArray,
            const Vertex3DVector &xyzcoArray,
            const Color &color);
         /**
         * Renders one text at a 2D position.
         * <P>
         * @parma text the text which should be rendered
         * @param xyco position where the text should be rendered
         * @param color    color in which the text should be rendered
         * @param clip     if true, clip text to left edge of window. (Quite slow.)
         */
         void Render2DText(const std::string &text,
            Math::CoordN<sint32,2> xyco,
            const Color &color,
            bool clip = false);

      private:
         TextRenderer();
         ~TextRenderer();

      private:
         /**
         * the font created out of the descriptor
         */
         std::vector< FX::FXFont* > m_fonts;
         /**
         * the gllist di which is neede for drawing then the fonts
         */
         std::vector< uint32 > m_font_gl_lists;
         /**
         * current used font for rendering
         */
         sint32 m_current_font;

         static TextRenderer m_text_renderer;
      };
   }
}

#endif /* _TC_OPENGL_TEXT_RENDERER_H_ */
