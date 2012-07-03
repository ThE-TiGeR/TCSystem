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

#include "TCGuiMenu.h"

#include "TCMLSHandler.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {
      TCGUIBASE_IMPLEMENT(MenuTitle,   FX::FXMenuTitle,   0, 0)
      TCGUIBASE_IMPLEMENT(MenuCascade, FX::FXMenuCascade, 0, 0)
      TCGUIBASE_IMPLEMENT(MenuCommand, FX::FXMenuCommand, 0, 0)

      MenuTitle::MenuTitle(FX::FXComposite* p, const std::string& text_id, FX::FXIcon* ic, FX::FXPopup* pup, uint32_t opts)
         :FX::FXMenuTitle(p, mls::Handler::GetText(text_id).c_str(), ic, pup, opts|FX::ICON_BEFORE_TEXT)
         ,Widget(text_id)
      {
      }

      MenuCascade::MenuCascade(FX::FXComposite* p,const std::string& text_id, FX::FXIcon* ic, FX::FXPopup* pup)
         :FX::FXMenuCascade(p, mls::Handler::GetText(text_id).c_str(), ic, pup)
         ,Widget(text_id)
      {
      }

      MenuCommand::MenuCommand(FX::FXComposite * p, const std::string &text_id, FX::FXIcon* ic, FX::FXObject* tgt, FX::FXSelector sel, uint32_t opts)
         :FX::FXMenuCommand(p, mls::Handler::GetText(text_id).c_str(), ic, tgt, sel, opts)
         ,Widget(text_id)
      {
      }

      MenuCommand::~MenuCommand()
      {
      }
   }
}
