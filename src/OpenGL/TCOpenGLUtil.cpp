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
//  $Id: TCOpenGLUtil.cpp 993 2010-05-20 20:44:20Z the_____tiger $
//*******************************************************************************

#include "TCOpenGLUtil.h"

#include "TCAssert.h"
#include "TCOpenGLTypes.h"
#include "TCOutput.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace OpenGL
   {
      TC_CT_ASSERT(sizeof(Vertex3D) == 3 * sizeof(float));
      TC_CT_ASSERT(sizeof(Vertex2D) == 2 * sizeof(float));

      void PrintOpenGLExtensions()
      {
         const char* string;
         TCINFOS("OpenGL", "TCOglImplementation::PrintOpenGLextensions()");

         string = (const char*)glGetString(GL_VENDOR);
         TCINFOS("OpenGL", "      GL_VENDOR: " << string);
         string = (const char*)glGetString(GL_RENDERER);
         TCINFOS("OpenGL", "    GL_RENDERER: " << string);
         string = (const char*)glGetString(GL_VERSION);
         TCINFOS("OpenGL", "     GL_VERSION: " << string);
         //string = glGetString(GL_EXTENSIONS) ? (const char*)glGetString(GL_EXTENSIONS) : "unknown";
         //TCINFOS("OpenGL", "  GL_EXTENSIONS: " << string );
         string = glGetString(GLU_EXTENSIONS) ? (const char*)glGetString(GLU_EXTENSIONS) : "unknown";
         TCINFOS("OpenGL", " GLU_EXTENSIONS: " << string);
      }

      void PrintOpenGLTextureSupport()
      {
         GLint val;
         TCINFOS("OpenGL", "TCOglImplementation::OpenGLtextureSupport()");

         TCINFOS("OpenGL", " max 2D texture size: ");
         glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
         TCINFOS("OpenGL", val);
      }

      void PrintOpenGLBufferInfo()
      {
         GLint val;
         TCINFOS("OpenGL", "TCOglImplementation::OpenGLbufferInfo()");

         TCINFOS("OpenGL", " number of bits per R,G,B,A component in color buffer: ");
         glGetIntegerv(GL_RED_BITS, &val);
         TCINFOS("OpenGL", val);
         glGetIntegerv(GL_GREEN_BITS, &val);
         TCINFOS("OpenGL", val);
         glGetIntegerv(GL_BLUE_BITS, &val);
         TCINFOS("OpenGL", val);
         glGetIntegerv(GL_ALPHA_BITS, &val);
         TCINFOS("OpenGL", val);

         TCINFOS("OpenGL", " number of bits per index in color buffer: ");
         glGetIntegerv(GL_INDEX_BITS, &val);
         TCINFOS("OpenGL", val);

         TCINFOS("OpenGL", " number of bits per pixel in depth buffer: ");
         glGetIntegerv(GL_DEPTH_BITS, &val);
         TCINFOS("OpenGL", val);

         TCINFOS("OpenGL", " number of bits per pixel in stencil buffer: ");
         glGetIntegerv(GL_STENCIL_BITS, &val);
         TCINFOS("OpenGL", val);

         TCINFOS("OpenGL", " number of bits per R,G,B,A component in accumulation buffer: ");
         glGetIntegerv(GL_ACCUM_RED_BITS, &val);
         TCINFOS("OpenGL", val);
         glGetIntegerv(GL_ACCUM_GREEN_BITS, &val);
         TCINFOS("OpenGL", val);
         glGetIntegerv(GL_ACCUM_BLUE_BITS, &val);
         TCINFOS("OpenGL", val);
         glGetIntegerv(GL_ACCUM_ALPHA_BITS, &val);
         TCINFOS("OpenGL", val);

         TCINFOS("OpenGL", " number of auxiliary buffers present: ");
         glGetIntegerv(GL_AUX_BUFFERS, &val);
         TCINFOS("OpenGL", val);
      }

      bool DetectOpenGLerror()
      {
         GLenum errorCode;
         const GLubyte *errorString;

         errorCode = glGetError();

         if (errorCode != GL_NO_ERROR)
         {
            TCERRORS("OpenGL", "*** TCOglImplementation::DetectOpenGLerror(), OpenGL error");;
            errorString = gluErrorString(errorCode);

            if (errorString != 0)
            {
               TCERRORS("OpenGL", "     message: " << reinterpret_cast<const char*>(errorString));
            }
            TCERRORS("OpenGL", "     error_code: " << static_cast<uint32>(errorCode));
            return true;
         }
         return false;
      }
   }
}
