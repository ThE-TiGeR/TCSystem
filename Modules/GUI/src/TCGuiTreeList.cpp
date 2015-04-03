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
//  $Id: TCGuiTreeList.cpp,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#include "TCGuiTreeList.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {

      class TreeList: public FX::FXTreeList
      {
         friend class TreeItem;
      };

      TCGUIBASE_IMPLEMENT(TreeItem, FX::FXTreeItem, 0, 0)

      TreeItem::TreeItem(const FX::FXTreeList *treeList, const std::string& text,
         FX::FXIcon* oi,FX::FXIcon* ci,void* ptr)
         :FX::FXTreeItem(text.c_str(), oi, ci, ptr)
      {
         SetTextFont(treeList->getFont());
         SetTextColor(treeList->getTextColor());
         SetTextSelectColor(treeList->getSelTextColor());
         SetBackColor(treeList->getBackColor());
      }

      void TreeItem::draw(const FX::FXTreeList* list_in,FX::FXDC& dc,int32_t x,int32_t y,int32_t w,int32_t h) const
      {
         TreeList* list = const_cast<TreeList*>(static_cast<const TreeList*>(list_in));

         FX::FXFont* text_font         = list->getFont();
         FX::FXColor text_color        = list->getTextColor();
         FX::FXColor text_select_color = list->getSelTextColor();
         FX::FXColor back_color        = list->getBackColor();

         list->font = m_text_font;
         list->textColor = m_text_color;
         list->seltextColor = m_text_select_color;
         list->backColor = m_back_color;

         dc.setFont(m_text_font);
         FX::FXTreeItem::draw(list_in, dc, x, y, w, h);
         dc.setFont(text_font);

         list->font = text_font;
         list->textColor = text_color;
         list->seltextColor = text_select_color;
         list->backColor = back_color;
      }

      int32_t TreeItem::getHeight(const FX::FXTreeList* list_in) const
      {
         TreeList* list = const_cast<TreeList*>(static_cast<const TreeList*>(list_in));

         FX::FXFont* text_font = list->getFont();
         list->font = m_text_font;

         int32_t h = FX::FXTreeItem::getHeight(list_in);

         list->font = text_font;

         return h;
      }

      int32_t TreeItem::getWidth(const FX::FXTreeList* list_in) const
      {
         TreeList* list = const_cast<TreeList*>(static_cast<const TreeList*>(list_in));

         FX::FXFont* text_font = list->getFont();
         list->font = m_text_font;

         int32_t w = FX::FXTreeItem::getWidth(list_in);

         list->font = text_font;

         return w;
      }

      int32_t TreeItem::hitItem(const FX::FXTreeList* list_in, int32_t x, int32_t y) const
      {
         TreeList* list = const_cast<TreeList*>(static_cast<const TreeList*>(list_in));

         FX::FXFont* text_font = list->getFont();
         list->font = m_text_font;

         int32_t hit = FX::FXTreeItem::hitItem(list_in, x, y);

         list->font = text_font;

         return hit;
      }
   }
}
