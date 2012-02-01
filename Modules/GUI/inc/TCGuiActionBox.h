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

#ifndef _TCGUI_ACTIONBOX_H_
#define _TCGUI_ACTIONBOX_H_

#include "TCGuiWindow.h"

namespace tc
{
namespace gui
{

/**
 * @addtogroup TC_GUI
 * @{
 */

class TCGUI_API ActionBox: public Window
{
   TCGUIBASE_DECLARE(ActionBox);

public:
   /// Construct dialog which will always float over the owner window
   ActionBox(FX::FXWindow* owner, const std::string& name, sint32 w=0, sint32 h=0);
   virtual ~ActionBox() {}

public:
   long onCmdDoAction(FX::FXObject*, FX::FXSelector, void*);
   long onUpdDoAction(FX::FXObject*, FX::FXSelector, void*);

protected:                    
   ActionBox() {};
   
   virtual void doAction() {}

protected:
   bool closeOnExecute;
   
   FX::FXButton* actionButton;
   FX::FXButton* cancelButton;
};

/**
 * @}
 */

}
}

#endif
