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
//  $Id$
//*******************************************************************************

#include "TCOpenGLObjectGroup.h"

#include "TCOpenGLObject3D.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace OpenGL
   {
      FXIMPLEMENT(ObjectGroup, Object, 0, 0)

      ObjectGroup::ObjectGroup(const std::string& name)
      :Object(name)
      {
      }

      ObjectGroup::~ObjectGroup()
      {
         m_objects.clear();
      }

      uint32 ObjectGroup::GetNumObjects() const
      {
         return static_cast<uint32>(m_objects.size());
      }

      void ObjectGroup::AddObject(ObjectPtr object)
      {
         m_objects.push_back(object);
      }

      void ObjectGroup::RemoveObject(const std::string& name)
      {
         ObjectPtrVector::iterator object;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            if ((*object)->GetName() == name)
            {
               m_objects.erase(object);
               return;
            }
         }
      }

      ConstObjectPtr ObjectGroup::GetObject(uint32 idx) const
      {
         return m_objects[idx];
      }
      ObjectPtr ObjectGroup::GetObject(uint32 idx)
      {
         return m_objects[idx];
      }

      ConstObjectPtr ObjectGroup::GetObject(const std::string& name) const
      {
         ObjectPtrVector::const_iterator object;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            if ((*object)->GetName() == name)
            {
               return *object;
            }
         }
         return ConstObjectPtr();
      }
      ObjectPtr ObjectGroup::GetObject(const std::string& name)
      {
         ObjectPtrVector::iterator object;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            if ((*object)->GetName() == name)
            {
               return *object;
            }
         }
         return ObjectPtr();
      }

      ObjectPtr ObjectGroup::FindObject(const std::string& name)
      {
         ObjectPtrVector::iterator object;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            if ((*object)->GetName() == name)
            {
               return *object;
            }
            else if (ObjectGroupPtr::DynamicCast(*object))
            {
               ObjectPtr found_object=ObjectGroupPtr::StaticCast(*object)->FindObject(name);
               if (found_object)
               {
                  return found_object;
               }
            }
         }
         return ObjectPtr();
      }

      void ObjectGroup::Draw(Viewer& viewer)
      {
         ObjectPtrVector::iterator object;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            if ((*object)->IsActive())
            {
               (*object)->Draw(viewer);
            }
         }
      }

      void ObjectGroup::Update(Viewer& viewer, uint32 curent_time, uint32 delta_time)
      {
         ObjectPtrVector::iterator object;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            (*object)->Update(viewer, curent_time, delta_time);
         }
      }

      void ObjectGroup::GetBoundingBox(BoundingBox3D& bounding_box)
      {
         ObjectPtrVector::iterator object;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            BoundingBox3D box;
            if ((*object)->IsActive())
            {
               (*object)->GetBoundingBox(box);
            }
            bounding_box = BoundingBox3D::Unite(box, bounding_box);
         }
      }

      void ObjectGroup::DrawForHit(Viewer& viewer)
      {
         ::glPushName(0xffffffff);
         ObjectPtrVector::iterator object;
         GLuint gl_object_name = 0;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            ::glLoadName(gl_object_name++);
            if ((*object)->IsActive())
            {
               (*object)->DrawForHit(viewer);
            }
         }
         ::glPopName();
      }

      ObjectPtr ObjectGroup::GetHitObject(const uint32* hit_path)
      {
         if (hit_path[0] >= m_objects.size())
         {
            return ObjectPtr();
         }

         ObjectPtr hit_object = m_objects[hit_path[0]]->GetHitObject(hit_path+1);
         if (!hit_object)
         {
            hit_object = m_objects[hit_path[0]];
         }

         return hit_object;
      }

      bool ObjectGroup::Drag(Viewer& /*viewer*/, sint32 /*fx*/, sint32 /*fy*/, 
                                                  sint32 /*tx*/, sint32 /*ty*/ ) 
      {
         return false;
      }

      void ObjectGroup::ResetPosition()
      {
         ObjectPtrVector::iterator object;
         for (object=m_objects.begin(); object!=m_objects.end(); object++)
         {
            (*object)->ResetPosition();
         }
      }
   }
}
