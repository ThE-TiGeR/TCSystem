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
//  $Id: TCOpenGLPatchRenderer.cpp,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#include "TCOpenGLPatchRenderer.h"

#include "TCOpenGLUtil.h"
#include "TCOutput.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace open_gl
   {

      static bool s_vbo_supported = true;
#ifdef TCOS_WINDOWS
      static PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
      static PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
      static PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
      static PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
      static PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
      static PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
      static PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
      static PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#endif

      PatchRenderer::PatchRenderer(GLenum patch_type)
         :m_patch_type(patch_type)
         ,m_num_vertex(0)
         ,m_is_vbo_enable(false)
         ,m_vertex_buffer_id(0)
      {
      }

      PatchRenderer::~PatchRenderer()
      {
      }

      void PatchRenderer::EnableShading()
      {
          /* required for shading */ 
          ::glEnable(GL_LIGHTING);
      }

      void PatchRenderer::DisableShading()
      {
          /* required for shading */ 
          ::glDisable(GL_LIGHTING);
      }

      void PatchRenderer::EnableColoredShading(const Color &color)
      {
          EnableColoredShading();
          /* set the shading color */
          ::glColor4fv(color);
      }

      void PatchRenderer::EnableColoredShading()
      {
          /* required for shading */ 
          ::glEnable(GL_LIGHTING);
          /* necessary for obtaining expected color */
          ::glEnable(GL_COLOR_MATERIAL);
      }

      void PatchRenderer::DisableColoredShading()
      {
          /* required for shading */ 
          ::glDisable(GL_LIGHTING);
          /* necessary for obtaining expected color */
          ::glDisable(GL_COLOR_MATERIAL);
      }

      void PatchRenderer::SetFrontMaterial(const Material & mat)
      {
          ::glMaterialfv(GL_FRONT, GL_AMBIENT,   mat.ambient);
          ::glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat.diffuse);
          ::glMaterialfv(GL_FRONT, GL_SPECULAR,  mat.specular);
          ::glMaterialfv(GL_FRONT, GL_EMISSION,  mat.emission);
          ::glMaterialf (GL_FRONT, GL_SHININESS, mat.shininess);
      }

      void PatchRenderer::SetBackMaterial(const Material & mat)
      {
          ::glMaterialfv(GL_BACK, GL_AMBIENT,   mat.ambient);
          ::glMaterialfv(GL_BACK, GL_DIFFUSE,   mat.diffuse);
          ::glMaterialfv(GL_BACK, GL_SPECULAR,  mat.specular);
          ::glMaterialfv(GL_BACK, GL_EMISSION,  mat.emission);
          ::glMaterialf (GL_BACK, GL_SHININESS, mat.shininess);
      }

      void PatchRenderer::EnableBlending()
      {
          ::glEnable(GL_BLEND);
          ::glDepthMask(GL_FALSE);

          // stop two side lightning for blending
          ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
          ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
      }

      void PatchRenderer::DisableBlending()
      {
          ::glDisable(GL_BLEND);
          ::glDepthMask(GL_TRUE);

          // start two side lightning after bleending again
          ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
      }

      void PatchRenderer::EnableTexturing(const Texture &/*texture*/)
      {
          /* like pages 359, 384 of OglPG */
          ::glEnable(GL_TEXTURE_2D);
          ::glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
          //   glBindTexture(GL_TEXTURE_2D, texture.GetOglTextureName());
      }

      void PatchRenderer::DisableTexturing()
      {
          ::glDisable(GL_TEXTURE_2D);
      }

      void PatchRenderer::DisableLineOffset()
      {
          ::glDisable(GL_POLYGON_OFFSET_FILL);
      }

      void PatchRenderer::EnableLineOffset()
      {
          ::glEnable(GL_POLYGON_OFFSET_FILL);
      }

      void PatchRenderer::SetArrays(Vertex3DVector& vertex, Normal3DVector& normals, 
         ColorVector& color,  Vertex2DVector& textur_coordinates)
      {
         SetupVBOSupport();
         ClearData();

         if (!vertex.size())
         {
            return;
         }

         uint64_t allocation_size = 0; 
         // Setting vertex buffers
         if (vertex.size() != 0)
         {
            m_vertex_array.swap(vertex);
            m_num_vertex = m_vertex_array.size();
            m_vertex_offset = allocation_size;

            allocation_size += m_num_vertex * VERTEX3D_SIZE;
         }

         // set normal buffer
         if (normals.size() != 0)
         {
            m_normal_array.swap(normals);
            m_normal_offset = allocation_size;
            allocation_size += m_num_vertex * NORMAL3D_SIZE;
         }

         // set color buffer
         if (color.size() != 0)
         {
            m_color_array.swap(color);
            m_color_offset = allocation_size;
            allocation_size += m_num_vertex * COLOR_SIZE;
         }

         // set normal buffer
         if (textur_coordinates.size() != 0)
         {
            m_texture_array.swap(textur_coordinates);
            m_texture_offset = allocation_size;
            allocation_size += m_num_vertex * TEXTURE_COORDINATE_SIZE;
         }

         if (s_vbo_supported)
         {
            // generate a new VBO and get the associated ID
            glGenBuffersARB(1, &m_vertex_buffer_id);
            if (DetectOpenGLerror())
            {
               return;
            }

            // bind VBO in order to use
            glBindBufferARB(GL_ARRAY_BUFFER, m_vertex_buffer_id);
            if (DetectOpenGLerror())
            {
               return;
            }

            // upload data to VBO
            glBufferDataARB(GL_ARRAY_BUFFER, allocation_size, 0, GL_STATIC_DRAW_ARB);
            if (DetectOpenGLerror())
            {
               m_is_vbo_enable = false;
               glDeleteBuffersARB(1, &m_vertex_buffer_id);
               return;
            }
            m_is_vbo_enable = true;

            if (m_vertex_array.size())
            {
               glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, m_vertex_offset,  m_num_vertex * VERTEX3D_SIZE, &m_vertex_array.front());
            }
            if (m_normal_array.size())
            {
               glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, m_normal_offset,  m_num_vertex * NORMAL3D_SIZE, &m_normal_array.front());
            }
            if (m_color_array.size())
            {
               glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, m_color_offset,   m_num_vertex * COLOR_SIZE, &m_color_array.front());
            }
            if (m_texture_array.size())
            {
               glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, m_texture_offset, m_num_vertex * TEXTURE_COORDINATE_SIZE, &m_texture_array.front());
            }
            util::FreeMemoryOfStlContainer(m_vertex_array);
            util::FreeMemoryOfStlContainer(m_normal_array);
            util::FreeMemoryOfStlContainer(m_color_array);
            util::FreeMemoryOfStlContainer(m_texture_array);
         }
      }

      void PatchRenderer::SetPatchModeFilled()
      {
         ::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

      void PatchRenderer::SetPatchModeOutlined()
      {
         ::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }

      void PatchRenderer::SetPatchModePoints()
      {
         ::glPointSize(4.0);
         ::glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      }

      void PatchRenderer::DrawArrays()
      {
         if (!m_num_vertex)
         {
            return;
         }

         ::glEnableClientState(GL_VERTEX_ARRAY);

         uint64_t step = m_num_vertex;
         GLint max(0);
         ::glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &max);
         ::glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &max);
         //max = 65536;
         if (m_patch_type == GL_QUADS)
         {
            step = max - max % 4;
         }
         else if (m_patch_type == GL_TRIANGLES)
         {
            step = max - max % 3;
         }

         if (m_is_vbo_enable)
         {
            // bind VBO in order to use
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertex_buffer_id);
            ::glVertexPointer(3, GL_FLOAT, 0, (const GLvoid*)m_vertex_offset);
            ::glNormalPointer(GL_FLOAT, 0, (const GLvoid*)m_normal_offset);
            ::glColorPointer(4, GL_FLOAT, 0, (const GLvoid*)m_color_offset);
            ::glTexCoordPointer(2, GL_FLOAT, 0, (const GLvoid*)m_texture_offset);
         }
         else
         {
            if (m_vertex_array.size())
            {
               ::glVertexPointer(3, GL_FLOAT, 0, &m_vertex_array.front());
            }
            if (m_color_array.size())    
            {
               ::glColorPointer(4, GL_FLOAT, 0, &m_color_array.front());
            }
            if (m_normal_array.size())   
            {
               ::glNormalPointer(GL_FLOAT, 0, &m_normal_array.front());
            }
            if (m_texture_array.size())
            {
               ::glTexCoordPointer(2, GL_FLOAT, 0, &m_texture_array.front());
            }
         }

         for (uint32_t i=0; i<m_num_vertex; i+=step)
         {
            ::glDrawArrays(m_patch_type, i, util::Min(m_num_vertex-i, step));
         }

         ::glDisableClientState(GL_VERTEX_ARRAY);
         ::glDisableClientState(GL_COLOR_ARRAY);
         ::glDisableClientState(GL_NORMAL_ARRAY);
         ::glDisableClientState(GL_TEXTURE_COORD_ARRAY);

         if (m_is_vbo_enable)
         {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
         }
      }

      void PatchRenderer::DrawArraysFilled()
      {
         SetPatchModeFilled();
         DrawArrays();
      }

      void PatchRenderer::DrawArraysOutlined()
      {
         SetPatchModeOutlined();
         DrawArrays();

         // set polygon mode back to default
         SetPatchModeFilled();
      }

      void PatchRenderer::DrawArraysPoints()
      {
         SetPatchModePoints();

         DrawArrays();

         // set polygon mode back to default
         SetPatchModeFilled();
      }

      void PatchRenderer::EnableColorArray()
      {
         ::glEnableClientState(GL_COLOR_ARRAY);
      }

      void PatchRenderer::EnableNormalArray()
      {
         ::glEnableClientState(GL_NORMAL_ARRAY);
      }

      void PatchRenderer::EnableTexCoordArray()
      {
         ::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      }

      void PatchRenderer::SetupVBOSupport()
      {
#ifdef TCOS_WINDOWS
         if (glGenBuffersARB == 0 && s_vbo_supported)
         {
            // get pointers to GL functions
            pglGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffers");
            pglBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
            pglBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
            pglBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
            pglDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
            pglGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
            pglMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
            pglUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

            // check once again VBO extension
            if(pglGenBuffersARB && pglBindBufferARB && pglBufferDataARB && pglBufferSubDataARB &&
               pglMapBufferARB && pglUnmapBufferARB && pglDeleteBuffersARB && pglGetBufferParameterivARB)
            {
               s_vbo_supported = true;
               TCINFOS("open_gl", "Video card supports GL_ARB_vertex_buffer_object.");
            }
            else
            {
               s_vbo_supported = false;
               TCINFOS("open_gl", "Video card does NOT support GL_ARB_vertex_buffer_object.");
            }
         }
#endif
      }

      void PatchRenderer::ClearData()
      {
         m_num_vertex = 0;

         util::FreeMemoryOfStlContainer(m_vertex_array);
         util::FreeMemoryOfStlContainer(m_texture_array);
         util::FreeMemoryOfStlContainer(m_color_array);
         util::FreeMemoryOfStlContainer(m_normal_array);

         if (m_vertex_buffer_id)
         {
            glDeleteBuffersARB(1, &m_vertex_buffer_id);
            m_vertex_buffer_id = 0;
         }
         m_vertex_offset = std::numeric_limits<uint64_t>::max();
         m_texture_offset = std::numeric_limits<uint64_t>::max();
         m_color_offset = std::numeric_limits<uint64_t>::max();
         m_normal_offset = std::numeric_limits<uint64_t>::max();
      }
   }
}
