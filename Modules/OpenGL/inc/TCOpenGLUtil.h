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

#ifndef _TC_OGL_IMPLEMENTATION_H_
#define _TC_OGL_IMPLEMENTATION_H_

#include "TCOpenGL.h"

namespace tc
{
   namespace open_gl
   {
      /**
      * tc::Ogl::Implementation provides open_gl implementation specific
      * information:
      * "The portability of open_gl applications is one of open_gl's
      *  attractive features.  However, new versions of open_gl introduce 
      *  new features, and if you use new features, you may have problems 
      *  running your code on older versions of open_gl." -pg 567
      * 
      * All page numbers in the code comments refer to the book:
      * "open_gl Programming Guide, 3rd Ed, The Official Guide to 
      *  Learning open_gl, Version 1.2" by Mason Woo, Jackie Neider,
      *  Tom Davis, and Dave Shreiner.
      *
      */
      /**
      * This method prints the open_gl extensions supported by
      * a particular vendor. 
      * "If you want to know if a specific extension is supported
      * on your implementation, first used glGetString(GL_EXTENSIONS),
      * and then use gluGetString(GLU_EXTENSIONS).  This returns a list
      * of all the extensions in the implementation, separated by
      * spaces." pg 568 of the open_gl Programming Guide, 3rd ed.
      *
      */
      void TC_DLL_LOCAL PrintOpenGLExtensions();

      /**
      * "glGetIntegerv(GL_MAX_TEXTURE_SIZE,...) tells you a lower
      * bound on the largest width or height (without borders) of
      * a texture image, typically the size of the largest square
      * texture supported.  For 3D textures, GL_MAX_3D_TEXTURE_SIZE
      * may be used to query the larges allowable dimension (width,
      * height, or depth, without borders)..."
      * pg 365 of the open_gl Programming Guide, 3rd ed.
      *
      */
      void TC_DLL_LOCAL PrintOpenGLTextureSupport();

      /**
      * Prints out open_gl implentation information about 
      * several frame buffer parameters.
      * @see pages 432-433 of the open_gl Programming Guide, 3rd ed.
      */
      void TC_DLL_LOCAL PrintOpenGLBufferInfo();

      /**
      * "The truth is, your program will make mistakes.  Use of 
      *  error-handling routines is essential during development 
      *  and is highly recommended for commercially released applications.
      * (Unless you can give a 100% guarantee your program will never 
      *  generateand open_gl error condition.  Get real!)  open_gl has 
      *  simple error-handling routies for the base GL and GLU libraries.
      *  ...It is strongly recommended that you call glGetError() at
      *  least once in each display() routine."
      *  -pgs 563-4 of open_gl Programming Guide, 3rd Edition
      * 
      * @see pg 565 of open_gl Programming Guide, 3rd Edition
      * @return TRUE if open_gl detected an error
      */
      bool TC_DLL_LOCAL DetectOpenGLerror();
   }
}
#endif /* _TCOGLIMPLEMENTATION_H_ */
