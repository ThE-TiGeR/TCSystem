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

#ifndef _TCGUI_WINDOW_H_
#define _TCGUI_WINDOW_H_

#include "TCGuiBase.h"

namespace tc
{
   namespace gui
   {

      /**
      * @addtogroup TC_GUI
      * @{
      */

      class TCGUI_API TopWindow: public FX::FXTopWindow
      {
         TCGUIBASE_DECLARE(TopWindow);

      public:
         TopWindow(FX::FXWindow* owner, const std::string& title, FX::FXIcon *ma=0, FX::FXIcon *mi=0,
            uint32_t opts=FX::DECOR_TITLE|FX::DECOR_BORDER|FX::DECOR_CLOSE, int32_t x=0,int32_t y=0,
            int32_t w=0,int32_t h=0, int32_t pl=0,int32_t pr=0,int32_t pt=0,int32_t pb=0,int32_t hs=0,int32_t vs=0);

         TopWindow(FX::FXApp* app, const std::string& title, FX::FXIcon *ma=0, FX::FXIcon *mi=0,
            uint32_t opts=FX::DECOR_TITLE|FX::DECOR_BORDER|FX::DECOR_CLOSE, int32_t x=0,int32_t y=0,
            int32_t w=0,int32_t h=0, int32_t pl=0,int32_t pr=0,int32_t pt=0,int32_t pb=0,int32_t hs=0,int32_t vs=0);
         virtual   ~TopWindow() {}

         void SetTitle(const std::string &name);    // set title of window
         void SetHelpID(const std::string &helpID); // set help id of dialog box

         void SetAutoResize(bool ar) { autoResize = ar;}
         void AutoResize();

         void ButtonsShow();
         void ButtonsHide();
         bool ButtonsShown();

         FX::FXPacker* GetPacker() { return dialogPacker; }    // get packer for creating somthing inside

      public:
         enum 
         {
            ID_HELP=FX::FXTopWindow::ID_LAST,
            ID_APPLY,
            ID_LAST
         };

         long onCmdHelp(FX::FXObject*, FX::FXSelector, void*);
         long onUpdate(FX::FXObject*, FX::FXSelector, void*);

         virtual void show();


      protected:
         TopWindow() {}

      protected:
         FX::FXPacker *dialogPacker, *buttonPacker;
         std::string helpID;
         bool wasShown, autoResize;
      };

      class TCGUI_API Window: public FX::FXDialogBox
      {
         TCGUIBASE_DECLARE(Window);

      public:
         /// Construct dialog which will always float over the owner window
         Window(FX::FXWindow* owner, const std::string& title, int32_t w=0, int32_t h=0);
         virtual ~Window() {}

         void      SetTitle(const std::string &name);    // set title of window
         void      SetHelpID(const std::string &helpID); // set help id of dialog box

         void      SetAutoResize(bool ar) { autoResize = ar;}
         void      AutoResize();

         void      ButtonsShow();
         void      ButtonsHide();
         bool   ButtonsShown();

         FX::FXPacker*   GetPacker() { return dialogPacker; }    // get packer for creating somthing inside

      public:
         enum 
         {
            ID_HELP=FX::FXDialogBox::ID_LAST,
            ID_APPLY,
            ID_LAST
         };

         long onCmdCancel(FX::FXObject*, FX::FXSelector, void*);
         long onCmdHelp  (FX::FXObject*, FX::FXSelector, void*);
         long onUpdate(FX::FXObject*, FX::FXSelector, void*);

         virtual void show();

      protected:                    
         Window() {};

      protected:
         FX::FXPacker *dialogPacker, *buttonPacker;
         std::string helpID;
         bool wasShown, autoResize;
      };

      /**
      * @}
      */

   }
}

#endif
