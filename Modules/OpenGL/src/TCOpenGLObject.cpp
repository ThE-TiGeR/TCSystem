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

#include "TCOpenGLObject.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace open_gl
   {
      Object::Object(const std::string& name)
         :NamedObject(name)
         ,m_options()
      {
         SetOption(OP_ACTIVE, true);
         SetOption(OP_DRAG_ALLOWED, true);
      }

      Object::~Object() 
      {

      }

      // ----------------------------------------------------------
      // FOX ID mapping and corresponding methode´s
      // ----------------------------------------------------------
      FXDEFMAP(Object) s_object_map[]=
      {
         FXMAPFUNC(FX::SEL_COMMAND, Object::ID_STYLE_BOUNDINGBOX, Object::OnCmdBoundingBox),
         FXMAPFUNC(FX::SEL_UPDATE,  Object::ID_STYLE_BOUNDINGBOX, Object::OnUpdBoundingBox)
      };
      FXIMPLEMENT_ABSTRACT(Object, FX::FXObject, s_object_map, util::ArraySize(s_object_map));


      // Drawing style toggles
      long Object::OnCmdBoundingBox(FX::FXObject*, FX::FXSelector, void*)
      {
         m_options.flip(OP_STYLE_BOUNDBOX);
         return 1;
      }


      // Update drawing style toggles
      long Object::OnUpdBoundingBox(FX::FXObject *sender, FX::FXSelector, void*)
      {
         FX::FXSelector msg= FXSEL(FX::SEL_COMMAND, FX::FXWindow::ID_UNCHECK);
         if (m_options[OP_STYLE_BOUNDBOX])
         {
            msg=FXSEL(FX::SEL_COMMAND, FX::FXWindow::ID_CHECK);
         }

         sender->handle(this, msg, 0);
         sender->handle(this, FXSEL(FX::SEL_COMMAND, FX::FXWindow::ID_ENABLE), 0);
         return 1;
      }

      void Object::SetOption(uint32_t opt, bool state)
      {
         if (m_options.size() <= opt)
         {
            uint32_t needed_size = util::Max(opt+1, uint32_t(m_options.size()*2));
            m_options.resize(needed_size, false);
         }
         m_options[opt] = state;
      }

   }
}
