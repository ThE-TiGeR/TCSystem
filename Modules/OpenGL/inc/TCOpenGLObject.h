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
//  $Id: TCOpenGLObject.h,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#ifndef _TC_OPENGL_OBJECT_H_
#define _TC_OPENGL_OBJECT_H_

#include "TCOpenGL.h"

#include "TCOpenGLColor.h"
#include "TCOpenGLTypes.h"
#include "TCMathBox.h"
#include "TCSharedPtr.h"

#ifdef TCOS_WINDOWS
#pragma warning (push)
#pragma warning (disable: 6326)
#endif

#include <boost/dynamic_bitset.hpp>
#include <vector>

namespace tc
{
   class TCOGL_API NamedObject
   {
   public:
      explicit NamedObject(const std::string& name)
         :m_name(name)
      {
      }

      const std::string& GetName() const
      {
         return m_name;
      }

      void SetName(const std::string& name)
      {
         m_name = name;
      }
   protected:
      NamedObject() {}
   private:
      std::string m_name;
   };

   namespace open_gl
   {
      class Viewer;
      class Object;
      typedef SharedPtr<Object> ObjectPtr;
      typedef SharedPtr<const Object> ConstObjectPtr;
      typedef std::vector<ObjectPtr> ObjectPtrVector;

      class TCOGL_API Object: public FX::FXObject, 
                              public NamedObject
      {
         FXDECLARE(Object)
      public:
         enum Options
         {
            OP_ACTIVE,            ///< object is active
            OP_DRAG_ALLOWED,      ///< dragging the object is allowed
            OP_SELECTED,          ///< Object is selected
            OP_STYLE_BOUNDBOX,    ///< Draw bounding box
            OP_LAST
         };

      public:
         explicit Object(const std::string& name);
         virtual ~Object();

         bool IsActive() const {return m_options[OP_ACTIVE];}
         bool IsDragAble() const {return m_options[OP_DRAG_ALLOWED];}
         bool IsSelected() const {return m_options[OP_SELECTED];}

         void SetActive(bool sel) {m_options[OP_ACTIVE]=sel;}
         void SetSelected(bool sel) {m_options[OP_SELECTED]=sel;}

         bool GetOption(uint32_t opt) const {return m_options.size()>opt ? m_options[opt] : false;}
         void SetOption(uint32_t opt, bool state);

         virtual void Draw(Viewer& viewer) = 0;
         virtual void DrawForHit(Viewer& viewer) = 0;
         virtual void Update(Viewer& viewer, uint32_t curent_time, uint32_t delta_time) = 0;
         virtual bool Drag(Viewer& viewer, int32_t fx, int32_t fy, int32_t tx, int32_t ty) = 0;
         virtual ObjectPtr GetHitObject(const uint32_t* hitpath) = 0;
         virtual void GetBoundingBox(BoundingBox3D& bounding_box) = 0;
         virtual void ResetPosition() = 0;

         /** FOX IDs and handling methods */
         enum FXIDs
         {
            ID_FIRST=1000,    // Leaving space for Viewer
            ID_STYLE_BOUNDINGBOX,
            ID_LAST
         };

         long OnCmdBoundingBox(FX::FXObject*, FX::FXSelector, void*);
         long OnUpdBoundingBox(FX::FXObject*, FX::FXSelector, void*);

      protected:
         Object() {}
      private:
         boost::dynamic_bitset<> m_options;
      };
   }
}

#ifdef TCOS_WINDOWS
#pragma warning (pop)
#endif

#endif // _TC_OPENGL_OBJECT_H_
