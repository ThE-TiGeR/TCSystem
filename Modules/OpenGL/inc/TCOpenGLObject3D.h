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

#ifndef _TC_OGL_OBJECT_3D_H_
#define _TC_OGL_OBJECT_3D_H_

#include "TCOpenGLObject.h"

#include "TCMathBox.h"
#include "TCOpenGLColor.h"

namespace tc
{
   namespace open_gl
   {
      class TCOGL_API Object3D: public Object
      {
         FXDECLARE(Object3D)
      public:
         enum Options
         {
            OP_LAST = Object::OP_LAST
         };

         explicit Object3D(const std::string& name, uint32 num_options);
         virtual ~Object3D();

         virtual void Draw(Viewer& viewer);
         virtual void DrawForHit(Viewer& viewer);
         virtual bool Drag(Viewer& viewer, sint32 fx, sint32 fy, sint32 tx, sint32 ty);
         virtual void Update(Viewer& viewer, uint32 curent_time, uint32 delta_time);
         virtual ObjectPtr GetHitObject(const uint32* hitpath);
         virtual void GetBoundingBox(BoundingBox3D& bounding_box);
         virtual void ResetPosition();

         /** FOX IDs and handling methods */
         enum FXIDs
         {
            ID_LAST = Object::ID_LAST
         };

      protected:
         Object3D() {}
         virtual void DrawSelf(Viewer& viewer) = 0;
         virtual void DrawForHitSelf(Viewer& viewer) = 0;
         virtual void UpdateSelf(Viewer& viewer, uint32 curent_time, uint32 delta_time) = 0;

         void SetBoundingBox(const BoundingBox3D& bounding_box);
      private:
         void DrawBoundingBox();
         void DrawHandles();

         Vertex3D m_position;
         BoundingBox3D m_bounding_box;
         Color m_selection_color;
         Color m_bounding_box_color;

      };
   }
}

#endif
