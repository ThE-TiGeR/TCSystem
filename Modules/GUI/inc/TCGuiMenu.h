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
//  $Id: TCGuiMenu.h,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************
#ifndef _TC_GUI_MENU_H_
#define _TC_GUI_MENU_H_

#include "TCGuiWidget.h"

namespace tc
{
namespace gui
{

/**
 * @addtogroup TC_GUI
 * @{
 */

/**
 * @brief A Menu title class
 * A menu title is a child of a menu bar which is responsible
 * for popping up a pull down menu.
 * Extents the fox menu title to allow setting multi language string ids 
 * instead of directly setting the text
 */
class TCGUI_API MenuTitle: public FX::FXMenuTitle, public Widget
{
   TCGUIBASE_DECLARE(MenuTitle);

public:
   MenuTitle(FX::FXComposite* p, const std::string& text,
                  FX::FXIcon* ic=0, FX::FXPopup* pup=0, uint32_t opts=0);
   virtual ~MenuTitle() {};
  
protected:
   MenuTitle() {};
};

/**
 * @brief A Menu command class
 * The menu command widget is used to invoke a command in the
 * application from a menu.  Menu commands may reflect
 * the state of the application by graying out, or becoming hidden.
 * When activated, a menu command sends a SEL_COMMAND to its target.
 * Extents the fox menu command to allow setting multi language string ids 
 * instead of directly setting the text
 */
class TCGUI_API MenuCommand: public FX::FXMenuCommand, public Widget
{
   TCGUIBASE_DECLARE(MenuCommand);

public:
   MenuCommand(FX::FXComposite* p, const std::string& text,
                    FX::FXIcon* ic=0, FX::FXObject* tgt=0, FX::FXSelector sel=0, uint32_t opts=0);
   virtual ~MenuCommand();

protected:
   MenuCommand() {};
};

/**
 * @brief A Menu cascade class
 * The cascade menu widget is used to bring up a sub menu from a
 * pull down menu.
 * Extents the fox menu cascade to allow setting multi language string ids 
 * instead of directly setting the text
 */
class TCGUI_API MenuCascade: public FX::FXMenuCascade, public Widget
{
   TCGUIBASE_DECLARE(MenuCascade);

public:
   MenuCascade(FX::FXComposite* p, const std::string& text,
      FX::FXIcon* ic=0, FX::FXPopup* pup=0);
   virtual ~MenuCascade() {};

protected:
   MenuCascade() {};
};

/**
 * @}
 */

}
}

#endif // _TC_GUI_MENU_H_
