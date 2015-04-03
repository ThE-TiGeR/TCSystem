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
//  $Id: TCOpenGLCoordinateCross.h,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#ifndef _TC_OGL_COORDINATE_CROSS_H_
#define _TC_OGL_COORDINATE_CROSS_H_

#include "TCOpenGLObject3D.h"

namespace tc
{
   namespace open_gl
   {
      class TC_DLL_LOCAL CoordinateCross: public Object3D
      {
      public:
         explicit CoordinateCross(const std::string& name);
         virtual ~CoordinateCross();

      protected:
         virtual void DrawSelf(Viewer& viewer);
         virtual void UpdateSelf(Viewer& viewer, uint32_t curent_time, uint32_t delta_time);;
         virtual void DrawForHitSelf(Viewer& viewer);
      private:
         CoordinateCross() {}
         void DrawAxisArrow();
      private:
         GLUquadricObj* m_quadric;
      };
   }
}

#endif

