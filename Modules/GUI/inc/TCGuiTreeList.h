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
//  $Id: TCGuiTreeList.h,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#ifndef _TC_GUI_TREE_LIST_H_
#define _TC_GUI_TREE_LIST_H_

#include "TCGuiBase.h"

namespace tc
{
namespace gui
{

/**
 * @addtogroup TC_GUI
 * @{
 */

/**
 * @brief Tree list item
 * Extends the fox list item by allowing different font for each item and different colors for different
 * item states
 */
class TCGUI_API TreeItem : public FX::FXTreeItem
{
   TCGUIBASE_DECLARE(TreeItem);

public:
   TreeItem(const FX::FXTreeList* treeList, const std::string& text,
                 FX::FXIcon* oi=0, FX::FXIcon* ci=0, void* ptr=0);
   ~TreeItem() {};

   virtual int32_t getWidth(const FX::FXTreeList* list) const;
   virtual int32_t getHeight(const FX::FXTreeList* list) const;

   virtual void SetTextFont(FX::FXFont* text_font)
   {
      m_text_font = text_font;
   }
   void SetTextColor(FX::FXColor col)
   {
      m_text_color = col;
   }
   void SetTextSelectColor(FX::FXColor col)
   {
      m_text_select_color = col;
   }
   void SetBackColor(FX::FXColor col)
   {
      m_back_color = col;
   }

protected:
   TreeItem() {}

   virtual void draw(const FX::FXTreeList* list,FX::FXDC& dc,int32_t x,int32_t y,int32_t w,int32_t h) const;
   virtual int32_t hitItem(const FX::FXTreeList* list,int32_t x,int32_t y) const;

protected: 
   FX::FXFont*  m_text_font;
   FX::FXColor  m_text_color;
   FX::FXColor  m_text_select_color;
   FX::FXColor  m_back_color;
};

/**
 * @}
 */

}
}

#endif // _TC_GUI_TREE_LIST_H_
