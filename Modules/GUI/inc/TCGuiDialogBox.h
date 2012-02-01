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

#ifndef _TCGUI_DIALOGBOX_H_
#define _TCGUI_DIALOGBOX_H_

#include "TCGuiWindow.h"

namespace tc
{
namespace gui
{

/**
 * @addtogroup TC_GUI
 * @{
 */

class TCGUI_API DialogBox: public Window
{
   TCGUIBASE_DECLARE(DialogBox);

public:
   /// Construct dialog which will always float over the owner window
   DialogBox(FX::FXWindow* owner, const std::string& name, sint32 w=0, sint32 h=0);

   void    Reset();                                  // reset status of dialog box
   bool IsModified();                             // check if box was modified

   bool   AutoApply() { return !hasApplyButton; } // return if applybutton is available

public:
   enum 
   {
      ID_APPLY=Window::ID_LAST,
      ID_LAST
   };

   long onCmdApply (FX::FXObject*, FX::FXSelector, void*);
   long onCmdAccept(FX::FXObject*, FX::FXSelector, void*);
   long onCmdCancel(FX::FXObject*, FX::FXSelector, void*);
   long onCmdHelp  (FX::FXObject*, FX::FXSelector, void*);

   long onUpdApply (FX::FXObject*, FX::FXSelector, void*);
   long onUpdCancel(FX::FXObject*, FX::FXSelector, void*);
   long onUpdAccept(FX::FXObject*, FX::FXSelector, void*);
   
protected:                    
   DialogBox() {};
   virtual ~DialogBox() {}
   
   virtual void backupDialogValues ();
   virtual void applyDialogValues  ();
   virtual void restoreDialogValues();
   
protected:
   bool hasApplyButton, hasOkButton, hasCancelButton, wasModified;

public:
   void AddValue(sint32    &val);
   void AddValue(uint32   &val);
   void AddValue(float  &val);
   void AddValue(double &val);
   void AddValue(std::string &val);
   
private:
   std::vector< sint32  > m_int_values;
   std::vector< sint32* > m_int_pointers;
   
   std::vector< uint32  > m_uint_values;
   std::vector< uint32* > m_uint_pointers;
   
   std::vector< float  > m_float_values;
   std::vector< float* > m_float_pointers;

   std::vector< double  > m_double_values;
   std::vector< double* > m_double_pointers;
   
   std::vector< std::string  > m_string_values;
   std::vector< std::string* > m_string_pointers;
};

/**
 * @}
 */

}
}

#endif
