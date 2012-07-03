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

#ifndef _TCOGLPATCHRENDERER_H_
#define _TCOGLPATCHRENDERER_H_

#include "TCOpenGL.h"
#include "TCOpenGLMaterial.h"
#include "TCOpenGLTypes.h"

namespace tc
{
   namespace open_gl
   {
       class Texture;

      /**
      * This class provides methods for drawing patches with arrays
      */
      class TCOGL_API PatchRenderer
      {
      public:
         /**@patch_type Type of patches e.g. GL_QUADS */
         PatchRenderer(GLenum patch_type);
         /** destructor */
         virtual ~PatchRenderer();

                  /** Enable shading with the current material */
         void EnableShading();
         /** Disable shading */
         void DisableShading();
         /** Enable shading with the current color */
         void EnableColoredShading();
         /**
         * Enable shading with the specified color
         * @param The color to use for colored shading
         */
         void EnableColoredShading(const Color &color);
         /** Disable colored shading */
         void DisableColoredShading();

         /**
         * Set the material for front shading
         * @param mat The material to set
         */
         void SetFrontMaterial(const Material& mat);
         /**
         * Set the material for back shading
         * @param mat The material to set
         */
         void SetBackMaterial(const Material& mat);

         /**
         * enable texturing with the specified texture
         * @parame The textur to use for texturing
         */
         void EnableTexturing(const Texture &texture);
         /** disable texturing */
         void DisableTexturing();

         /** Enable blending */
         void EnableBlending();
         /** Disable blending */
         void DisableBlending();

        /**
         * Set the depth range that which we see a little through patches for drawing lines
         */
         void EnableLineOffset();
         /**
         * Set the depth range back that we do not see through patches
         * Only if GL_EXT_polygon_offset is not defined. Ohterwise the machine can handle it in a better way
         */
         void DisableLineOffset();

         /**
         * Set the arrays which are then used by the method draw arrays for
         * rendering patches or lines or points. Method tries to copy the data into video memory
         * if possible, if not it will swap the vectors into its own members. Your input data will be lost
         */
         void SetArrays(Vertex3DVector& vertex,
                        Normal3DVector& normals,
                        ColorVector& color,
                        TextureCoordinateVector& textur_coordinates);

         /** Enables color array for next draw call */
         void EnableColorArray ();
         /** Enables normal vector array for next draw call */
         void EnableNormalArray();
         /** Enables texture array for next draw call */
         void EnableTexCoordArray();

         /**
         * Draws the specified arrays in filled mode (see method above)
         * @param type      type of the gl primitive how to draw the arrays (e.g. GL_TRIANGLE, GL_QUAD ...)
         * @param numPoints Number of vertex in the arrays
         */
         void DrawArraysFilled();
         /**
         * Draws the specified arrays in outlined mode (see method above)
         * @param type      type of the gl primitive how to draw the arrays (e.g. GL_TRIANGLE, GL_QUAD ...)
         * @param numPoints Number of vertex in the arrays
         */
         void DrawArraysOutlined();
         /**
         * Draws the specified arrays in point mode (see method above)
         * @param type      type of the gl primitive how to draw the arrays (e.g. GL_TRIANGLE, GL_QUAD ...)
         * @param numPoints Number of vertex in the arrays
         */
         void DrawArraysPoints();

      private:
         /**
         * Draws the specified arrays (see method above)
         * @param type      type of the gl primitive how to draw the arrays (e.g. GL_TRIANGLE, GL_QUAD ...)
         * @param numPoints Number of vertex in the arrays
         */
         void DrawArrays();
         /** sets the mode for drawing patches to filled */
         void SetPatchModeFilled();
         /** sets the mode for drawing patches to outlined */
         void SetPatchModeOutlined();
         /** sets the mode for drawing patches to points */
         void SetPatchModePoints();
         /** set all array variable ... to 0 */
         void ClearData();

      private:
         static void SetupVBOSupport();

      private:
         /** Type of patches */
         GLenum m_patch_type;
         /** number of vertex to draw */
         uint64_t m_num_vertex;

         // Data needed if allocated in system memory
         /** pointer to the specified vertex array */
         Vertex3DVector m_vertex_array;
         /** pointer to the specified texture coord array */
         TextureCoordinateVector m_texture_array;
         /** pointer to the specified color array */
         ColorVector m_color_array;
         /** pointer to the specified normal array */
         Normal3DVector m_normal_array;   

         // Data needed if allocated in vbo
         bool m_is_vbo_enable;
         GLuint m_vertex_buffer_id;
         /** offset in the allocated array where vertex start */
         uint64_t m_vertex_offset;
         /** offset in the allocated array where texture coordinates start */
         uint64_t m_texture_offset;
         /** offset in the allocated array where color start */
         uint64_t m_color_offset;
         /** offset in the allocated array where normals start */
         uint64_t m_normal_offset;
      };
   }
}
#endif //_TC_OPENGL_PATCHRENDERER_

