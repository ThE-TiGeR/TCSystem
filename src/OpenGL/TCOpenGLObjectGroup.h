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
//  $Id: TCOpenGLObjectGroup.h 1003 2010-07-20 21:29:39Z the_____tiger $
//*******************************************************************************

#ifndef _TC_OPENGL_OBJECT_GROUP_H_
#define _TC_OPENGL_OBJECT_GROUP_H_

#include "TCOpenGLObject.h"

namespace TC
{
   namespace OpenGL
   {
      class TCOGL_API ObjectGroup: public Object
      {
         FXDECLARE(ObjectGroup)
      public:
         explicit ObjectGroup(const std::string& name);
         virtual ~ObjectGroup();

         uint32 GetNumObjects() const;
         void AddObject(ObjectPtr object);
         void RemoveObject(const std::string& name);

         ConstObjectPtr GetObject(uint32 idx) const;
         ObjectPtr GetObject(uint32 idx);
         ConstObjectPtr GetObject(const std::string& name) const;
         ObjectPtr GetObject(const std::string& name);

         ObjectPtr FindObject(const std::string& name);

         virtual void Draw(Viewer& viewer);
         virtual void Update(Viewer& viewer, uint32 curent_time, uint32 delta_time);
         virtual void DrawForHit(Viewer& viewer);
         virtual bool Drag(Viewer& viewer, sint32 fx, sint32 fy, sint32 tx, sint32 ty);
         virtual ObjectPtr GetHitObject(const uint32* hitpath);
         virtual void GetBoundingBox(BoundingBox3D& bounding_box);
         virtual void ResetPosition();

      protected:
         ObjectGroup() {}
      private:
         std::vector<ObjectPtr> m_objects;
     };

      typedef SharedPtr<ObjectGroup> ObjectGroupPtr;
   }
}

#endif // _TC_OPENGL_OBJECT_GROUP_H_
