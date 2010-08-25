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
//  $Id: TCOpenGLFace.h 982 2010-05-10 20:13:55Z the_____tiger $
//*******************************************************************************

#ifndef _TC_OGL_FACE_H_
#define _TC_OGL_FACE_H_

#include "TCOpenGLObject3D.h"
#include "TCOpenGLMaterial.h"
#include "TCOpenGLPatchRenderer.h"

namespace TC
{
   namespace OpenGL
   {
      class TCOGL_API Face: public Object3D
      {
         FXDECLARE_ABSTRACT(Face)
      public:
         enum Options
         {
            OP_SURFACE_DUALSIDED = Object3D::OP_LAST, ///< Dual-sided surface
            OP_FACECULLING_ON,                        ///< Cull backward facing surfaces

            OP_SURFACE,                               ///< Draw filled surfaces
            OP_SURFACE_COLORED,                       ///< Separate color for each vertex should be used
                                                      ///< when drawing surface
            OP_SHADING_SMOOTH,                        ///< Smooth shaded
            OP_SHADING_FLAT,                          ///< Flag shaded

            OP_WIREFRAME,                             ///< Draw wire frame
            OP_WIREFRAME_COLORED,                     ///< Separate color for each vertex should be used
                                                      ///< when drawing wire frame
            OP_POINTS,                                ///< Draw as points
            OP_POINTS_COLORED,                        ///< Separate color for each vertex should be used
                                                      ///< when drawing points

            OP_LAST
         };


         explicit Face(const std::string& name, GLenum patch_type);
         virtual ~Face();

         /** FOX IDs and handling methods */
         enum FXIDs
         {
            ID_TOGGLE_SIDED = Object3D::ID_LAST,
            ID_TOGGLE_CULLING,

            FXID_SURFACE,
            FXID_SURFACE_COLORED,
            FXID_SHADE_SMOOTH,
            FXID_SHADE_FLAT,
            FXID_SHADE_OFF,
            FXID_WIREFRAME,
            FXID_WIREFRAME_COLORED,
            FXID_POINTS,
            FXID_POINTS_COLORED,

            ID_FRONT_MATERIAL,
            ID_BACK_MATERIAL,
            ID_LAST
         };

         long OnDNDDrop(FX::FXObject*,FX::FXSelector,void*);
         long OnDNDMotion(FX::FXObject*,FX::FXSelector,void*);
         long OnQueryTip(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdShadeOff(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdShadeOff(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdFrontMaterial(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdFrontMaterial(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdBackMaterial(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdBackMaterial(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdBoundingBox(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdDrawingStyle(FX::FXObject*,FX::FXSelector,void*);

      protected:
         Face();
         virtual void DrawSelf(OpenGL::Viewer& viewer);
         virtual void DrawForHitSelf(OpenGL::Viewer& viewer);

         PatchRenderer& GetRenderer();
      private:
         void RenderWireFrame();
         void RenderSurface();
         void RenderPoints();
         void SetMaterials();

      private:
         Material m_material[2];
         PatchRenderer m_renderer;
      };
   }
}

#endif
