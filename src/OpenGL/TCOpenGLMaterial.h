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
//  $Id: TCOpenGLMaterial.h 984 2010-05-12 08:33:52Z the_____tiger $
//*******************************************************************************

#ifndef _TC_OPENGL_MATERIAL_H_
#define _TC_OPENGL_MATERIAL_H_

#include "TCOpenGLColor.h"
#include "TCOpenGLTypes.h"

namespace TC
{
   namespace OpenGL
   {
      /*
       * OpenGL Light Source
       */
      class Light 
      {
      public:
         Color    ambient;           // Ambient light color
         Color    diffuse;           // Diffuse light color
         Color    specular;          // Specular light color
         Color    position;          // Light position
         Vertex3D direction;         // Spot direction
         float    exponent;          // Spotlight exponent
         float    cutoff;            // Spotlight cutoff angle
         float    c_attn;            // Constant attenuation factor
         float    l_attn;            // Linear attenuation factor
         float    q_attn;            // Quadratic attenuation factor
      };

      /** 
       * class holds the OpenGL Material Description
       */
      class TCOGL_API Material
      {
      public:
         Material();
         Material(const Color &color);

      public:
         Color ambient;         /// Ambient material color
         Color diffuse;         /// Diffuse material color
         Color specular;        /// Specular material color
         Color emission;        /// Emissive material color

         float shininess;       /// Specular shininess
      };
   }
}

#endif // _TC_OPENGL_MATERIAL_H_

