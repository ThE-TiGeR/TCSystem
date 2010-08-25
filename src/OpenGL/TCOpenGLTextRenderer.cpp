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
//  $Id: TCOpenGLTextRenderer.cpp 984 2010-05-12 08:33:52Z the_____tiger $
//*******************************************************************************

#include "TCOpenGLTextRenderer.h"

#include "TCOutput.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace OpenGL
   {
      class FXFontCompareFXFontDesc
      {
      public:
         explicit FXFontCompareFXFontDesc(const FX::FXFontDesc& desc)
            :m_desc(desc)
         {
         }

         bool operator()(const FX::FXFont* font)
         {
            FX::FXFontDesc temp_desc;
            font->getFontDesc(temp_desc);
            return memcmp(&m_desc, &temp_desc, sizeof(FX::FXFontDesc)) == 0;
         }

      private:
         FX::FXFontDesc m_desc;
      };

      TextRenderer TextRenderer::m_text_renderer;

      TextRenderer::TextRenderer()
         :m_fonts()
         ,m_font_gl_lists()
         ,m_current_font(-1)
      {
      }

      TextRenderer::~TextRenderer()
      {
         for (uint32 i=0; i<m_fonts.size(); i++)
         {
            ::glDeleteLists(m_font_gl_lists[i], m_fonts[i]->getMaxChar() + 1);
            delete m_fonts[i];
         }
         m_fonts.clear();
      }

      void TextRenderer::SetFont(const FX::FXFont *font)
      {
         FX::FXFontDesc fontdesc; font->getFontDesc(fontdesc);
         SetFont(fontdesc);
      }

      void TextRenderer::SetFont(const FX::FXFontDesc& fontdesc)
      {
         FXFontCompareFXFontDesc find_object(fontdesc);
         std::vector< FX::FXFont* >::iterator found_font;
         found_font = std::find_if(m_fonts.begin(), m_fonts.end(), find_object);
         if (found_font == m_fonts.end())
         {
            m_current_font = -1;
         }
         else
         {
            m_current_font = found_font - m_fonts.begin();
         }

         if (m_current_font == -1)
         {
            FX::FXFont *font = new FX::FXFont(FX::FXApp::instance(), fontdesc);
            font->create();
            uint32 first = font->getMinChar();
            uint32 last  = Util::Min(uint32(font->getMaxChar()), uint32(255)); // in the moment only ascii character are supported

            // generate list for all characters   
            uint32 fontList = glGenLists(last + 1);
            FX::glUseFXFont(font, first, last - first, fontList + first);

            // store the data of creted font
            m_fonts.push_back(font);
            m_font_gl_lists.push_back(fontList);

            m_current_font = static_cast<sint32>(m_fonts.size() - 1);
         }

         if (m_current_font == -1 && m_fonts.size()) m_current_font = 0;
         TC_ASSERT(m_current_font != -1);
      }

      void TextRenderer::Render3DText(const std::string &text,
         const Math::Coord3D < float >&xyzco,
         const Color & color)
      {
         TC_ASSERT(m_current_font != -1);
         if (m_current_font == -1) return;

         ::glColor4fv(color);

         glPushAttrib(GL_LIST_BIT);
         glListBase(m_font_gl_lists[m_current_font]);

         glRasterPos3f(xyzco[0], xyzco[1], xyzco[2]);
         glCallLists(static_cast<GLsizei>(text.length()), GL_UNSIGNED_BYTE, text.c_str());

         glPopAttrib();
      }

      void TextRenderer::Render2DText(const std::string &text,
         Math::Coord2D< sint32 > xyco,
         const Color &color,
         bool clip)
      {
         TC_ASSERT(m_current_font != -1);
         if (m_current_font == -1) return;

         ::glColor4fv(color);

         glPushAttrib(GL_LIST_BIT);
         glListBase(m_font_gl_lists[m_current_font]);

         std::string::size_type textLength = text.length();
         const char* textPtr = text.c_str();
         if (clip) 
         {
            //TCMatrix4x4 m;
            //glGetDoublev(GL_MODELVIEW_MATRIX, (TCDouble*)((const TCDouble*)m));
            //sint32 translationX = (sint32)(m[3][0]);
            //char currentCharacter[2];
            //currentCharacter[1] = 0;
            //FXFont* font = m_fonts[m_current_font];
            //while ((translationX+xyco[0] < 0) && (textLength > 0)) 
            //{
            //   currentCharacter[0] = *textPtr;
            //   sint32 charWidth = font->getTextWidth(currentCharacter, 1);
            //   xyco[0] += charWidth;
            //   textLength--;
            //   textPtr++;
            //}
         }

         glRasterPos2i(xyco[0], xyco[1]);
         glCallLists(static_cast<GLsizei>(textLength), GL_UNSIGNED_BYTE, textPtr);

         glPopAttrib();
      }

      void TextRenderer::Render3DTexts(const std::vector < std::string > &textArray,
         const std::vector < Math::Coord3D <float> > &xyzcoArray,
         const Color &color)
      {
         TC_ASSERT(m_current_font != -1);
         if (m_current_font == -1) return;

         ::glColor4fv(color);

         glPushAttrib(GL_LIST_BIT);
         glListBase(m_font_gl_lists[m_current_font]);

         for(uint32 i=0; i<textArray.size(); ++i)
         {   
            glRasterPos3f(xyzcoArray[i][0], xyzcoArray[i][1], xyzcoArray[i][2]);
            glCallLists(static_cast<GLsizei>(textArray[i].length()), GL_UNSIGNED_BYTE, textArray[i].c_str());
         }

         glPopAttrib();

      }
   }
}