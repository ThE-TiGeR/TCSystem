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
//  $Id: TCGuiActionBox.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCGuiActionBox.h"

#include "TCGuiButton.h"
#include "TCUtil.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Gui
   {
      FXDEFMAP(ActionBox) TCGuiActionBoxMap[] =
      {
         FXMAPFUNC(FX::SEL_COMMAND, ActionBox::ID_CANCEL, ActionBox::onCmdCancel),
         FXMAPFUNC(FX::SEL_COMMAND, ActionBox::ID_ACCEPT, ActionBox::onCmdDoAction),
         FXMAPFUNC(FX::SEL_UPDATE,  ActionBox::ID_ACCEPT, ActionBox::onUpdDoAction),
      };

      TCGUIBASE_IMPLEMENT(ActionBox, Window, TCGuiActionBoxMap, ARRAYNUMBER(TCGuiActionBoxMap))

         ActionBox::ActionBox(FXWindow * owner, const std::string & title, sint32 w, sint32 h)
         :Window(owner, title, w,  h)
      {
         closeOnExecute = 0;
         cancelButton   = new CancelButton(buttonPacker, this, ActionBox::ID_CANCEL, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
         actionButton   = new OkButton    (buttonPacker, this, ActionBox::ID_ACCEPT, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
      }

      long ActionBox::onCmdDoAction(FX::FXObject *sender, FX::FXSelector sel, void *ptr)
      {
         doAction();
         if (closeOnExecute) 
         {
            Window::onCmdAccept(sender, sel, ptr);
         }

         return 1;
      }

      long ActionBox::onUpdDoAction(FX::FXObject *sender, FX::FXSelector, void*)
      {
         FX::FXButton *o = (FX::FXButton*)sender;
         sint32 w = o->getFont()->getTextWidth(o->getText().text(), o->getText().length());
         if (o->getWidth() != Util::Max(w + 22 + 15, 85))
         {
            o->setWidth(Util::Max(w + 22 + 15, 85));
         }

         return 1;
      }
   }
}

