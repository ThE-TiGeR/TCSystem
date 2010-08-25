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
//  $Id: TCGuiTreeList.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TC_GUI_TREE_LIST_H_
#define _TC_GUI_TREE_LIST_H_

#include "TCGuiBase.h"

namespace TC
{
namespace Gui
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

   virtual sint32 getWidth(const FX::FXTreeList* list) const;
   virtual sint32 getHeight(const FX::FXTreeList* list) const;

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

   virtual void draw(const FX::FXTreeList* list,FX::FXDC& dc,sint32 x,sint32 y,sint32 w,sint32 h) const;
   virtual sint32 hitItem(const FX::FXTreeList* list,sint32 x,sint32 y) const;

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
