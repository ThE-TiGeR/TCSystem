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
//  $Id: TCGuiWindow.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCGuiWindow.h"

#include "TCGuiApplication.h"
#include "TCGuiMessageBox.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Gui
   {

      FXDEFMAP(Window) WindowMap[] =
      {
         FXMAPFUNC(FX::SEL_COMMAND, Window::ID_CANCEL, Window::onCmdCancel),
         FXMAPFUNC(FX::SEL_COMMAND, Window::ID_HELP,   Window::onCmdHelp),
         FXMAPFUNC(FX::SEL_UPDATE, 0, Window::onUpdate),
      };
      TCGUIBASE_IMPLEMENT(Window, FX::FXDialogBox, WindowMap, ARRAYNUMBER(WindowMap))

      Window::Window(FX::FXWindow * owner, const std::string & title, sint32 w, sint32 h)
         :FX::FXDialogBox(owner, " ", FX::DECOR_TITLE | FX::DECOR_BORDER |FX::DECOR_RESIZE, 0, 0, w,  h, 0, 0, 0, 0)
      {
         SetTitle(title);

         dialogPacker = new FX::FXPacker(this, FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y|FX::LAYOUT_SIDE_BOTTOM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

         buttonPacker = new FX::FXPacker(dialogPacker, FX::LAYOUT_FILL_X | FX::LAYOUT_SIDE_BOTTOM);
         new FX::FXHorizontalSeparator(buttonPacker, FX::LAYOUT_SIDE_TOP | FX::SEPARATOR_GROOVE | FX::LAYOUT_FILL_X | FX::LAYOUT_FIX_HEIGHT, 0, 0, 0, 10, 0,
            0, 0, 0);

         autoResize = false;
         wasShown = false;
      }

      void Window::SetTitle(const std::string & titleIn)
      {
         std::string title = Application::GetInstance()->GetProgrammName() + "-" + titleIn;
         setTitle(title.c_str());
      }

      void Window::SetHelpID(const std::string & helpIDIn)
      {
         helpID = helpIDIn;
      }

      void Window::ButtonsShow()
      {
         buttonPacker->show();
      }
      void Window::ButtonsHide()
      {
         buttonPacker->hide();
      }
      bool Window::ButtonsShown()
      {
         if (buttonPacker->shown()) return true;
         return false;
      }

      long Window::onCmdHelp(FX::FXObject*, FX::FXSelector, void *)
      {
         MessageBox::Error(this, "Help on", (std::string)"Help for " + helpID + " not available");
         return 1;
      }

      long Window::onCmdCancel(FX::FXObject *sender, FX::FXSelector sel, void *ptr)
      {
         return FX::FXDialogBox::onCmdCancel(sender, sel, ptr);
      }

      long Window::onUpdate(FX::FXObject*, FX::FXSelector, void*)
      {
         if (shown() && autoResize)
            AutoResize();
         else
            return 0;

         return 1;
      }

      void Window::AutoResize()
      {
         sint32 _w = dialogPacker->getDefaultWidth();
         sint32 _h = dialogPacker->getDefaultHeight();

         if (_w + 20 != getWidth() || _h + 20 != getHeight())   
            resize(_w + 20, _h + 20);
      }

      void Window::show()
      {
         // check if we need to create window
         if (!id()) create();

         if (wasShown == true)
            FX::FXDialogBox::show();
         else
            FX::FXDialogBox::show(FX::PLACEMENT_CURSOR);

         wasShown = true;
      }




      FXDEFMAP(TopWindow) TopWindowMap[] =
      {
         FXMAPFUNC(FX::SEL_COMMAND, TopWindow::ID_HELP,   TopWindow::onCmdHelp),
         FXMAPFUNC(FX::SEL_UPDATE, 0, TopWindow::onUpdate),
      };
      TCGUIBASE_IMPLEMENT(TopWindow, FX::FXTopWindow, TopWindowMap, ARRAYNUMBER(TopWindowMap))


      TopWindow::TopWindow(FX::FXWindow* owner, const std::string& title, FX::FXIcon *ma, FX::FXIcon *mi, uint32 opts, sint32 x,sint32 y, sint32 w,sint32 h,
         sint32 pl,sint32 pr,sint32 pt,sint32 pb,sint32 hs,sint32 vs)
         :FX::FXTopWindow(owner, "",ma,mi,opts,x,y,w,h,pl,pr,pt,pb,hs,vs)
      {
         SetTitle(title);

         dialogPacker = new FX::FXPacker(this, FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y|FX::LAYOUT_SIDE_BOTTOM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

         buttonPacker = new FX::FXPacker(dialogPacker, FX::LAYOUT_FILL_X | FX::LAYOUT_SIDE_BOTTOM);
         new FX::FXHorizontalSeparator(buttonPacker, FX::LAYOUT_SIDE_TOP | FX::SEPARATOR_GROOVE | FX::LAYOUT_FILL_X | FX::LAYOUT_FIX_HEIGHT, 0, 0, 0, 10, 0,
            0, 0, 0);

         //   new TCGuiHelpButton  (buttonPacker, this, ID_HELP,  LAYOUT_BOTTOM | LAYOUT_SIDE_LEFT);

         autoResize = false;
         wasShown = false;
      }

      TopWindow::TopWindow(FX::FXApp* app, const std::string& title, FX::FXIcon *ma, FX::FXIcon *mi, uint32 opts, sint32 x,sint32 y, sint32 w,sint32 h,
         sint32 pl,sint32 pr,sint32 pt,sint32 pb,sint32 hs,sint32 vs)
         :FX::FXTopWindow(app, "",ma,mi,opts,x,y,w,h,pl,pr,pt,pb,hs,vs)
      {
         SetTitle(title);

         dialogPacker = new FX::FXPacker(this, FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y|FX::LAYOUT_SIDE_BOTTOM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

         buttonPacker = new FX::FXPacker(dialogPacker, FX::LAYOUT_FILL_X | FX::LAYOUT_SIDE_BOTTOM);
         new FX::FXHorizontalSeparator(buttonPacker, FX::LAYOUT_SIDE_TOP | FX::SEPARATOR_GROOVE | FX::LAYOUT_FILL_X | FX::LAYOUT_FIX_HEIGHT, 0, 0, 0, 10, 0,
            0, 0, 0);

         //   new TCGuiHelpButton  (buttonPacker, this, ID_HELP,  LAYOUT_BOTTOM | LAYOUT_SIDE_LEFT);

         autoResize = false;
         wasShown = false;
      }

      void TopWindow::SetTitle(const std::string & titleIn)
      {
         std::string title = Application::GetInstance()->GetProgrammName() + "-" + titleIn;
         setTitle(title.c_str());
      }

      void TopWindow::SetHelpID(const std::string & helpIDIn)
      {
         helpID = helpIDIn;
      }

      void TopWindow::ButtonsShow()
      {
         buttonPacker->show();
      }
      void TopWindow::ButtonsHide()
      {
         buttonPacker->hide();
      }
      bool TopWindow::ButtonsShown()
      {
         if (buttonPacker->shown()) return true;
         return false;
      }


      long TopWindow::onCmdHelp(FX::FXObject*, FX::FXSelector, void *)
      {
         MessageBox::Error(this, "Help on", (std::string)"Help for " + helpID + " not available");
         return 1;
      }

      long TopWindow::onUpdate(FX::FXObject*, FX::FXSelector, void*)
      {
         if (shown() && autoResize)
            AutoResize();
         else
            return 0;

         return 1;
      }

      void TopWindow::AutoResize()
      {
         sint32 _w = dialogPacker->getDefaultWidth();
         sint32 _h = dialogPacker->getDefaultHeight();

         if (_w + 20 != getWidth() || _h + 20 != getHeight())   
            resize(_w + 20, _h + 20);
      }

      void TopWindow::show()
      {
         // check if we need to create window
         if (!id()) create();

         if (wasShown == true)
            FX::FXTopWindow::show();
         else
            FX::FXTopWindow::show(FX::PLACEMENT_CURSOR);

         wasShown = true;
      }

   }
}
