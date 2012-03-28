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

#ifndef _TC_OGL_TEXTURE_2D_H_
#define _TC_OGL_TEXTURE_2D_H_

#include "TCOpenGLObject2D.h"
#include "TCStlTypes.h"

namespace tc
{
   namespace open_gl
   {
      class TCOGL_API Texture2D: public Object2D
      {
      public:
         Texture2D(const std::string& name);
         ~Texture2D();

         bool CreateTexture(const Vertex2D& size, const Uint32Vector& data);
         void DeleteTexture();

      protected:
         virtual void DrawSelf(Viewer& viewer);

      private:
         Vertex2D m_size;
         GLuint m_texture_id;
      };

      typedef SharedPtr<Texture2D> Texture2DPtr;
   }
}

#endif // _TC_OGL_TEXTURE_2D_H_
