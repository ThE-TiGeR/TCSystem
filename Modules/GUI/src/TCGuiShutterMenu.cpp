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

#include "TCGuiShutterMenu.h"

#include "TCGuiImageHandler.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {
      static FX::FXColor GetActiveTextColor(FX::FXColor color)
      {
         return FXRGB(FXMIN(FXREDVAL(color)   + 100, 255),
                      FXMIN(FXGREENVAL(color) + 100, 255),
                      FXMIN(FXBLUEVAL(color)  + 100, 255));

      }

      FXIMPLEMENT(ShutterMenu, FXScrollWindow, 0, 0);

      ShutterMenu::ShutterMenu(FXComposite *p, FX::FXObject* tgt, FX::FXSelector sel, uint32 opts)
         :FXScrollWindow(p, opts)
      {
         backColor = FXRGB(FXMAX(FXREDVAL(backColor)   - 60, 0),
            FXMAX(FXGREENVAL(backColor) - 60, 0),
            FXMAX(FXBLUEVAL(backColor)  - 60, 0));

         setTarget(tgt);
         setSelector(sel);

         m_item_frame = new FX::FXVerticalFrame(this, FX::LAYOUT_FILL_X|FX::LAYOUT_FILL_Y, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8);
         m_item_frame->setBackColor(backColor);

         FX::FXFont *fontNormal = getApp()->getNormalFont();
         m_font_bold = new FX::FXFont(getApp(),fontNormal->getName(),(fontNormal->getSize()/10), FX::FXFont::Bold);
         m_font_bold->create();
      }

      ShutterMenu::~ShutterMenu()
      {
         delete m_font_bold;
      }

      FXDEFMAP(ShutterMenuItem) s_shutter_menu_item_map[] =
      {
         FXMAPFUNC(FX::SEL_COMMAND, ShutterMenuItem::ID_BUTTON, ShutterMenuItem::onCommand),
         FXMAPFUNC(FX::SEL_UPDATE,  ShutterMenuItem::ID_BUTTON, ShutterMenuItem::onUpdate),
         FXMAPFUNC(FX::SEL_ENTER,   ShutterMenuItem::ID_BUTTON, ShutterMenuItem::onEnter),
         FXMAPFUNC(FX::SEL_LEAVE,   ShutterMenuItem::ID_BUTTON, ShutterMenuItem::onLeave),
         FXMAPFUNC(FX::SEL_PAINT,   0,                          ShutterMenuItem::onPaint),
      };

      FXIMPLEMENT(ShutterMenuItem, FX::FXVerticalFrame, s_shutter_menu_item_map, ARRAYNUMBER(s_shutter_menu_item_map));

      ShutterMenuItem::ShutterMenuItem(ShutterMenu* p, const std::string& text, FX::FXIcon* ic,
         FX::FXObject* tgt, FX::FXSelector sel, uint32 opts)
         :FXVerticalFrame(p->m_item_frame, opts|FX::LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0)
      {
         setTarget(tgt);
         setSelector(sel);

         backColor = FXRGB(FXMIN(FXREDVAL(backColor)   + 20, 255),
            FXMIN(FXGREENVAL(backColor) + 20, 255),
            FXMIN(FXBLUEVAL(backColor)  + 20, 255));

         m_icon_up   = ImageHandler::GetInstance()->GetIcon("TC_GUI_DOUBLE_ARROW_UP");
         m_icon_down = ImageHandler::GetInstance()->GetIcon("TC_GUI_DOUBLE_ARROW_DOWN");

         m_button = new Button(this, text, ic, this, ID_BUTTON, FX::JUSTIFY_LEFT|FX::LAYOUT_FILL_X);

         m_button_frame = new FX::FXVerticalFrame(this, FX::LAYOUT_FILL_X|FX::LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
         m_button_frame->setBackColor(backColor);

         m_button->setFont(p->m_font_bold);
         m_button->setBackColor(FXRGB(FXMAX(FXREDVAL(backColor)   - 20, 0),
            FXMAX(FXGREENVAL(backColor) - 20, 0),
            FXMAX(FXBLUEVAL(backColor)  - 20, 0)));
         m_button->setText(text.c_str());

         m_text_color_save = m_button->getTextColor();
      }

      ShutterMenuItem::~ShutterMenuItem()
      {
      }

      long ShutterMenuItem::onCommand(FX::FXObject*, FX::FXSelector, void *)
      {
         if (getTarget()) getTarget()->handle(this, MKUINT(FX::SEL_COMMAND, getSelector()), 0);

         if (m_button_frame->shown())
         {
            m_button_frame->hide();
         }
         else
         {
            m_button_frame->show();
         }
         recalc();
         update();

         return 1;
      }

      long ShutterMenuItem::onUpdate(FX::FXObject*, FX::FXSelector, void *)
      {
         if (m_button_frame->shown() && m_button->getIcon() != m_icon_up)
         {
            m_button->setIcon(m_icon_up);
         }
         else if (!m_button_frame->shown() && m_button->getIcon() != m_icon_down)
         {
            m_button->setIcon(m_icon_down);
         }

         return 1;
      }

      long ShutterMenuItem::onEnter(FX::FXObject*, FX::FXSelector, void *)
      {
         if(m_button->isEnabled())
         {
            m_button->setTextColor(GetActiveTextColor(m_text_color_save));
            //m_Button->setDefaultCursor(GuiApplication()->GetCursor("pick_cursor/cursor_link"));
            m_button->update();
         }

         return 1;
      }

      long ShutterMenuItem::onLeave(FX::FXObject*, FX::FXSelector, void *)
      {
         if(m_button->isEnabled())
         {
            m_button->setTextColor(m_text_color_save);
            //m_Button->setDefaultCursor(FXApp::instance()->getDefaultCursor(DEF_ARROW_CURSOR));
            m_button->update();
         }

         return 1;
      }

      FXDEFMAP(ShutterMenuButton) s_shutter_menu_button_map[] =
      {
         FXMAPFUNC(FX::SEL_ENTER,  0, ShutterMenuButton::onEnter),
         FXMAPFUNC(FX::SEL_LEAVE,  0, ShutterMenuButton::onLeave),
         FXMAPFUNC(FX::SEL_PAINT,  0, ShutterMenuButton::onPaint),
      };

      FXIMPLEMENT(ShutterMenuButton, Button, s_shutter_menu_button_map, ARRAYNUMBER(s_shutter_menu_button_map));

      ShutterMenuButton::ShutterMenuButton(ShutterMenuItem* p, const std::string& text, FX::FXIcon* ic,
         FX::FXObject* tgt, FX::FXSelector sel)
         :Button(p->m_button_frame, text, ic, tgt, sel, FX::JUSTIFY_LEFT|FX::LAYOUT_FIX_X|FX::LAYOUT_FILL_X, 10, 0, 0, 0, 0, 0, 0, 0)
      {
         backColor = FXRGB(FXMIN(FXREDVAL(backColor)   + 20, 255),
            FXMIN(FXGREENVAL(backColor) + 20, 255),
            FXMIN(FXBLUEVAL(backColor)  + 20, 255));

         m_text_color_save = getTextColor();
      }

      long ShutterMenuButton::onEnter(FX::FXObject* sender, FX::FXSelector sel, void *ptr)
      {
         Button::onEnter(sender, sel, ptr);
         if(isEnabled())
         {
            setTextColor(GetActiveTextColor(m_text_color_save));
            //setDefaultCursor(GuiApplication()->GetCursor("pick_cursor/cursor_link"));

            update();
         }

         return 1;
      }

      long ShutterMenuButton::onLeave(FX::FXObject* sender, FX::FXSelector sel, void *ptr)
      {
         Button::onLeave(sender, sel, ptr);
         if(isEnabled())
         {
            setTextColor(m_text_color_save);
            //setDefaultCursor(FXApp::instance()->getDefaultCursor(DEF_ARROW_CURSOR));
            update();
         }

         return 1;
      }

      long ShutterMenuButton::onPaint(FX::FXObject* sender, FX::FXSelector sel, void *ptr)
      {
         FX::FXDCWindow dc(this, (FX::FXEvent*)ptr);

         if (textColor == GetActiveTextColor(m_text_color_save))
         {
            int tw=0, th=0, tx, ty, iw=0, ih=0, ix, iy;
            // Place text & icon and drawn icon with shadow
            if(!label.empty())
            {
               tw=labelWidth(label);
               th=labelHeight(label);
            }
            if(icon)
            {
               iw=icon->getWidth();
               ih=icon->getHeight();
            }
            just_x(tx,ix,tw,iw);
            just_y(ty,iy,th,ih);

            dc.setForeground(backColor);
            dc.fillRectangle(ix-1, iy-1, iw+2, ih+2);

            if(icon)
            {
               if (this->getState() ==FX::STATE_ENGAGED)
                  dc.drawIconShaded(icon,ix,iy);
               else
               {
                  dc.drawIconShaded(icon,ix+1,iy+1);
                  dc.drawIcon      (icon,ix-1,iy-1);
               }
            }

            dc.setForeground(textColor);
            int posx, posy;
            posx = icon ? icon->getWidth()+4 : 4;
            posy = height - (height - th)/2 - 2;

            dc.drawLine(posx, posy, tw+posx, posy);
         }
         else
            Button::onPaint(sender, sel, ptr);

         if (this->getState() ==FX::STATE_ENGAGED)
            drawDoubleSunkenRectangle(dc,0,0,width-12,height);

         return 1;
      }

      ShutterMenuButton::~ShutterMenuButton()
      {
      }
   }
}
