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
//  $Id: TCGuiMessageBox.cpp,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#include "TCGuiMessageBox.h"

#include "TCGuiApplication.h"
#include "TCGuiButton.h"
#include "TCGuiImageHandler.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {

      FXDEFMAP(MessageBox) TCGuiMessageBoxMap[] =
      {
         FXMAPFUNCS(FX::SEL_COMMAND, MessageBox::ID_CLICKED_YES, MessageBox::ID_CLICKED_SAVE, MessageBox::onCmdClicked)
      };

      TCGUIBASE_IMPLEMENT(MessageBox, FX::FXDialogBox, TCGuiMessageBoxMap, ARRAYNUMBER(TCGuiMessageBoxMap))

         // Create message box
      MessageBox::MessageBox(FX::FXWindow * owner, const std::string& title,
                             const std::string& text, FX::FXIcon * ic, int32_t buttons)
         :FX::FXDialogBox(owner,(Application::GetInstance()->GetProgrammName() + "-" + title).c_str())
      {
         FX::FXVerticalFrame *content = new FX::FXVerticalFrame(this, FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y);
         FX::FXHorizontalFrame *info = new FX::FXHorizontalFrame(content, FX::LAYOUT_TOP | FX::LAYOUT_LEFT | FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y);
         new FX::FXLabel(info, "", ic, FX::ICON_BEFORE_TEXT | FX::LAYOUT_TOP | FX::LAYOUT_LEFT | FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y);
         new FX::FXLabel(info, text.c_str(), 0,FX::JUSTIFY_LEFT | FX::ICON_BEFORE_TEXT | FX::LAYOUT_TOP | FX::LAYOUT_LEFT | FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y);

         FX::FXPacker *p = new FX::FXPacker(content, FX::LAYOUT_FILL_X | FX::LAYOUT_SIDE_BOTTOM);
         //new FX::FXHorizontalSeparator(p, FX::LAYOUT_SIDE_TOP | FX::SEPARATOR_GROOVE | FX::LAYOUT_FILL_X | FX::LAYOUT_FIX_HEIGHT, 0, 0, 0, 10, 0,
         //   0, 0, 0);

         if (buttons == BUTTONS_OK) 
         {
            new OkButton(p, this, ID_CLICKED_OK, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
         }
         else if (buttons == BUTTONS_OK_CANCEL) 
         {
            new NoButton (p, this, ID_CLICKED_NO, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
            new YesButton(p, this, ID_CLICKED_YES, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
         }
         else if (buttons == BUTTONS_YES_NO_CANCEL)
         {
            new CancelButton (p, this, ID_CLICKED_CANCEL, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
            new NoButton (p, this, ID_CLICKED_NO, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
            new YesButton(p, this, ID_CLICKED_YES, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
         }
      }

      // Close dialog with a cancel
      long MessageBox::onCmdClicked(FX::FXObject *, FX::FXSelector sel, void *)
      {
         getApp()->stopModal(this, CLICKED_YES + (FXSELID(sel) - ID_CLICKED_YES));
         hide();
         return 1;
      }

      // Show a modal error message
      int32_t MessageBox::Error(FX::FXWindow * owner, const std::string & title, const std::string & message)
      {
         if (owner == 0) owner = FX::FXApp::instance()->getRootWindow();
         MessageBox box(owner, title, message, ImageHandler::GetInstance()->GetIcon("tc_gui_error_big"), BUTTONS_OK);

         return box.execute();
      }

      // Show a modal warning message
      int32_t MessageBox::Warning(FX::FXWindow * owner, const std::string & title, const std::string & message)
      {
         if (owner == 0) owner = FX::FXApp::instance()->getRootWindow();
         MessageBox box(owner, title, message, ImageHandler::GetInstance()->GetIcon("tc_gui_warning_big"), BUTTONS_OK);

         return box.execute();
      }

      // Show a modal question dialog
      int32_t MessageBox::Question(FX::FXWindow * owner, const std::string & title, const std::string & message)
      {
         if (owner == 0) owner = FX::FXApp::instance()->getRootWindow();
         MessageBox box(owner, title, message, ImageHandler::GetInstance()->GetIcon("tc_gui_question_big"), BUTTONS_OK_CANCEL);

         return box.execute();
      }

      // Show a modal information dialog
      int32_t MessageBox::Information(FX::FXWindow * owner, const std::string & title, const std::string & message)
      {
         if (owner == 0) owner = FX::FXApp::instance()->getRootWindow();
         MessageBox box(owner, title, message, ImageHandler::GetInstance()->GetIcon("tc_gui_info_big"), BUTTONS_OK);

         return box.execute();
      }

   }
}
