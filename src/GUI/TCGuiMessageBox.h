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
//  $Id: TCGuiMessageBox.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TC_GUI_MESSAGE_BOX_H_
#define _TC_GUI_MESSAGE_BOX_H_

#include "TCGuiBase.h"

namespace TC
{
namespace Gui
{

/**
 * @addtogroup TC_GUI
 * @{
 */

/// Message box
class TCGUI_API MessageBox: public FX::FXDialogBox
{
   TCGUIBASE_DECLARE(MessageBox);

public:
   enum
   {
      ID_CLICKED_YES=FX::FXDialogBox::ID_LAST,
      ID_CLICKED_NO,
      ID_CLICKED_OK,
      ID_CLICKED_CANCEL,
      ID_CLICKED_QUIT,
      ID_CLICKED_SAVE,
      ID_LAST
   };

   // Message box buttons
   enum
   {
      BUTTONS_OK                   = 0,
      BUTTONS_OK_CANCEL            = 1,
      BUTTONS_YES_NO               = 2,
      BUTTONS_YES_NO_CANCEL        = 3,
      BUTTONS_QUIT_CANCEL          = 4,
      BUTTONS_QUIT_SAVE_CANCEL     = 5
   };

   // Return values
   enum
   {
      CLICKED_YES      = 1,
      CLICKED_NO       = 2,
      CLICKED_OK       = 3,
      CLICKED_CANCEL   = 4,
      CLICKED_QUIT     = 5,
      CLICKED_SAVE     = 6
   };
public:
   /// Construct message box with given caption, icon, and message text
   MessageBox(FX::FXWindow* owner, const std::string &title, const std::string &message,
              FX::FXIcon* ic=0, sint32 buttons=0);

   /// Show a modal error message
   static sint32 Error(FX::FXWindow* owner, const std::string &title, const std::string &message);
   /// Show a modal warning message
   static sint32 Warning(FX::FXWindow* owner, const std::string &title, const std::string &message);
   /// Show a modal question dialog
   static sint32 Question(FX::FXWindow* owner, const std::string &title, const std::string &message);
   /// Show a modal information dialog
   static sint32 Information(FX::FXWindow* owner, const std::string &title, const std::string &message);

   long onCmdClicked(FX::FXObject*, FX::FXSelector, void*);
   
protected:
   MessageBox(){};
};

/**
 * @}
 */

}
}

#endif // _TC_GUI_MESSAGE_BOX_H_
