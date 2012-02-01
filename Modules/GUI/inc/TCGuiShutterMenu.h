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
#ifndef _TC_GUI_SHUTTER_MENU_H_
#define _TC_GUI_SHUTTER_MENU_H_

#include "TCGuiButton.h"

namespace tc
{
   namespace gui
   {
      class TCGUI_API ShutterMenu: public FX::FXScrollWindow
      {
         TCGUIBASE_DECLARE(ShutterMenu)
      public:
         ShutterMenu(FX::FXComposite *p, FX::FXObject* tgt=0, FX::FXSelector sel=0, uint32 opts=0);
         virtual ~ShutterMenu();

      protected:
         ShutterMenu(){}

      private:
         FX::FXFont* m_font_bold;
         FX::FXVerticalFrame* m_item_frame;

         friend class ShutterMenuItem;
      };


      class TCGUI_API ShutterMenuItem: public FX::FXVerticalFrame
      {
         TCGUIBASE_DECLARE(ShutterMenuItem)
      public:
         ShutterMenuItem(ShutterMenu* p, const std::string& text, FX::FXIcon* ic=0, FX::FXObject* tgt=0, FX::FXSelector sel=0, uint32 opts=0);
         virtual ~ShutterMenuItem();

         FX::FXVerticalFrame* GetContent() {return m_button_frame;};

         enum
         {
            ID_BUTTON=FX::FXVerticalFrame::ID_LAST,
            ID_LAST
         };

         long onCommand(FX::FXObject* sender, FX::FXSelector sel, void *ptr);
         long onUpdate (FX::FXObject* sender, FX::FXSelector sel, void *ptr);
         long onEnter  (FX::FXObject* sender, FX::FXSelector sel, void *ptr);
         long onLeave  (FX::FXObject* sender, FX::FXSelector sel, void *ptr);

      protected:
         ShutterMenuItem(){}

      private:
         FX::FXColor m_text_color_save;

         Button *m_button;
         FX::FXVerticalFrame *m_button_frame;

         FX::FXIcon *m_icon_up;
         FX::FXIcon *m_icon_down;

         friend class ShutterMenuButton;
      };


      class TCGUI_API ShutterMenuButton: public Button
      {
         TCGUIBASE_DECLARE(ShutterMenuButton)
      public:
         ShutterMenuButton(ShutterMenuItem* p, const std::string& text, FX::FXIcon* ic=0, FX::FXObject* tgt=0, FX::FXSelector sel=0);
         virtual ~ShutterMenuButton();

         long onEnter(FX::FXObject* sender, FX::FXSelector sel, void *ptr);
         long onLeave(FX::FXObject* sender, FX::FXSelector sel, void *ptr);
         long onPaint(FX::FXObject* sender, FX::FXSelector sel, void *ptr);

      protected:
         ShutterMenuButton(){}

      private:
         FX::FXColor m_text_color_save;
      };
   }
}

#endif //_TC_GUI_SHUTTER_MENU_H_
