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
//  $Id: TCGuiButton.cpp,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#include "TCGuiButton.h"

#include "TCGuiImageHandler.h"
#include "TCMLSHandler.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {

      FXDEFMAP(Button) TCGuiButtonMap[] =
      {
         FXMAPFUNC(FX::SEL_LEFTBUTTONPRESS,    0,   Button::onLeftBtnPress),
         FXMAPFUNC(FX::SEL_KEYPRESS,           0,   Button::onKeyPress),
      };
      
      TCGUIBASE_IMPLEMENT(Button,       FX::FXButton, TCGuiButtonMap, ARRAYNUMBER(TCGuiButtonMap))
      TCGUIBASE_IMPLEMENT(HelpButton,   Button, 0, 0)
      TCGUIBASE_IMPLEMENT(ApplyButton,  Button, 0, 0)
      TCGUIBASE_IMPLEMENT(CancelButton, Button, 0, 0)
      TCGUIBASE_IMPLEMENT(OkButton,     Button, 0, 0)
      TCGUIBASE_IMPLEMENT(YesButton,    Button, 0, 0)
      TCGUIBASE_IMPLEMENT(NoButton,     Button, 0, 0)

      Button::Button(FX::FXComposite * p, const std::string &text_id, FX::FXIcon* ic, FX::FXObject * tgt,
         FX::FXSelector sel, uint32_t opts,
         int32_t x, int32_t y, int32_t w, int32_t h, int32_t pl,
         int32_t pr, int32_t pt, int32_t pb)
         :FX::FXButton(p, mls::Handler::GetText(text_id).c_str(), ic, tgt, sel, opts, x, y, w, h, pl,pr,pt,pb)
         ,Widget(text_id)
      {
      }

      HelpButton::HelpButton(FX::FXComposite * p, FX::FXObject * tgt,
         FX::FXSelector sel, uint32_t opts,
         int32_t x, int32_t y, int32_t w, int32_t h, int32_t pl,
         int32_t pr, int32_t pt, int32_t pb)
         :Button(p, "TCGUI_HELP", ImageHandler::GetInstance()->GetIcon("tc_gui_help"), tgt, sel,
         opts|FX::ICON_BEFORE_TEXT|FX::LAYOUT_FIX_WIDTH|FX::LAYOUT_FIX_HEIGHT|FX::FRAME_RAISED,
         x, y,w, h,pl,pr,pt,pb)
      {
      }

      ApplyButton::ApplyButton(FX::FXComposite * p, FX::FXObject * tgt,
         FX::FXSelector sel, uint32_t opts,
         int32_t x, int32_t y, int32_t w, int32_t h, int32_t pl,
         int32_t pr, int32_t pt, int32_t pb)
         :Button(p, "TCGUI_APPLY", ImageHandler::GetInstance()->GetIcon("tc_gui_apply"), tgt, sel,
         opts|FX::FRAME_RAISED|FX::JUSTIFY_CENTER_X|FX::JUSTIFY_CENTER_Y|FX::ICON_UNDER_TEXT|FX::LAYOUT_FIX_WIDTH,
         x, y,w, h,pl,pr,pt,pb)
      {
      }

      OkButton::OkButton(FX::FXComposite * p, FX::FXObject * tgt,
         FX::FXSelector sel, uint32_t opts,
         int32_t x, int32_t y, int32_t w, int32_t h, int32_t pl,
         int32_t pr, int32_t pt, int32_t pb)
         :Button(p, "", ImageHandler::GetInstance()->GetIcon("tc_gui_ok"), tgt, sel,
         opts|FX::FRAME_RAISED|FX::JUSTIFY_CENTER_X|FX::JUSTIFY_CENTER_Y|FX::ICON_UNDER_TEXT|FX::LAYOUT_FIX_WIDTH,
         x, y,w, h,pl,pr,pt,pb)
      {
      }

      CancelButton::CancelButton(FX::FXComposite * p, FX::FXObject * tgt,
         FX::FXSelector sel, uint32_t opts,
         int32_t x, int32_t y, int32_t w, int32_t h, int32_t pl,
         int32_t pr, int32_t pt, int32_t pb)
         :Button(p, "", ImageHandler::GetInstance()->GetIcon("tc_gui_cancel"), tgt, sel,
         opts|FX::FRAME_RAISED|FX::JUSTIFY_CENTER_X|FX::JUSTIFY_CENTER_Y|FX::ICON_UNDER_TEXT|FX::LAYOUT_FIX_WIDTH,
         x, y,w, h,pl,pr,pt,pb)
      {
      }

      YesButton::YesButton(FX::FXComposite * p, FX::FXObject * tgt,
         FX::FXSelector sel, uint32_t opts,
         int32_t x, int32_t y, int32_t w, int32_t h, int32_t pl,
         int32_t pr, int32_t pt, int32_t pb)
         :Button(p, "", ImageHandler::GetInstance()->GetIcon("tc_gui_yes"), tgt, sel,
         opts|FX::FRAME_RAISED|FX::JUSTIFY_CENTER_X|FX::JUSTIFY_CENTER_Y|FX::ICON_UNDER_TEXT|FX::LAYOUT_FIX_WIDTH,
         x, y,w, h,pl,pr,pt,pb)
      {
      }

      NoButton::NoButton(FX::FXComposite * p, FX::FXObject * tgt,
         FX::FXSelector sel, uint32_t opts,
         int32_t x, int32_t y, int32_t w, int32_t h, int32_t pl,
         int32_t pr, int32_t pt, int32_t pb)
         :Button(p, "", ImageHandler::GetInstance()->GetIcon("tc_gui_no"), tgt, sel,
         opts|FX::FRAME_RAISED|FX::JUSTIFY_CENTER_X|FX::JUSTIFY_CENTER_Y|FX::ICON_UNDER_TEXT|FX::LAYOUT_FIX_WIDTH,
         x, y,w, h,pl,pr,pt,pb)
      {
      }
   }
}
