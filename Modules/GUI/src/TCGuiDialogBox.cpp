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

#include "TCGuiDialogBox.h"

#include "TCGuiButton.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {

      FXDEFMAP(DialogBox) TCGuiDialogBoxMap[] =
      {
         FXMAPFUNC(FX::SEL_COMMAND, DialogBox::ID_APPLY,  DialogBox::onCmdApply),
         FXMAPFUNC(FX::SEL_COMMAND, DialogBox::ID_ACCEPT, DialogBox::onCmdAccept),
         FXMAPFUNC(FX::SEL_COMMAND, DialogBox::ID_CANCEL, DialogBox::onCmdCancel),

         FXMAPFUNC(FX::SEL_UPDATE,  DialogBox::ID_APPLY,  DialogBox::onUpdApply),
         FXMAPFUNC(FX::SEL_UPDATE,  DialogBox::ID_CANCEL, DialogBox::onUpdCancel),
         FXMAPFUNC(FX::SEL_UPDATE,  DialogBox::ID_ACCEPT, DialogBox::onUpdAccept),
      };

      TCGUIBASE_IMPLEMENT(DialogBox, Window, TCGuiDialogBoxMap, ARRAYNUMBER(TCGuiDialogBoxMap))

      DialogBox::DialogBox(FXWindow * owner, const std::string & title, sint32 w, sint32 h)
         :Window(owner, title, w,  h)
      {
         hasOkButton     = 1;
         hasCancelButton = 1;
         hasApplyButton  = 1;
         wasModified     = 0;

         new ApplyButton (buttonPacker, this, ID_APPLY,  FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
         new CancelButton(buttonPacker, this, ID_CANCEL, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);
         new OkButton    (buttonPacker, this, ID_ACCEPT, FX::LAYOUT_BOTTOM | FX::LAYOUT_SIDE_RIGHT);

         backupDialogValues();
      }

      long DialogBox::onCmdAccept(FX::FXObject *sender, FX::FXSelector sel, void *ptr)
      {
         applyDialogValues();
         wasModified = 0;
         Window::onCmdAccept(sender, sel, ptr);
         return 1;
      }

      long DialogBox::onCmdApply(FX::FXObject*, FX::FXSelector, void*)
      {
         applyDialogValues();
         return 1;
      }

      long DialogBox::onCmdCancel(FX::FXObject *sender, FX::FXSelector sel, void *ptr)
      {
         restoreDialogValues();
         wasModified = 0;
         Window::onCmdCancel(sender, sel, ptr);
         return 1;
      }

      long DialogBox::onUpdApply(FX::FXObject *sender, FX::FXSelector, void *ptr)
      {
         uint32 msg1 = AutoApply()  ? FX::FXWindow::ID_HIDE   : FX::FXWindow::ID_SHOW;
         uint32 msg2 = IsModified() ? FX::FXWindow::ID_ENABLE : FX::FXWindow::ID_DISABLE;

         sender->handle(this, MKUINT(msg1, FX::SEL_COMMAND), ptr);
         sender->handle(this, MKUINT(msg2, FX::SEL_COMMAND), ptr);

         return 1;
      }

      long DialogBox::onUpdCancel(FX::FXObject *sender, FX::FXSelector, void *ptr)
      {
         uint32 msg1 = hasCancelButton == 1 ? FX::FXWindow::ID_SHOW : FX::FXWindow::ID_HIDE;
         uint32 msg2              = FX::FXWindow::ID_DISABLE;
         if (!wasModified ) msg2 = FX::FXWindow::ID_ENABLE;
         if (IsModified()) msg2 = FX::FXWindow::ID_ENABLE;

         sender->handle(this, MKUINT(msg2, FX::SEL_COMMAND), ptr);
         sender->handle(this, MKUINT(msg1, FX::SEL_COMMAND), ptr);

         return 1;
      }

      long DialogBox::onUpdAccept(FX::FXObject *sender, FX::FXSelector, void *ptr)
      {
         uint32 msg = hasOkButton == 1 ? FX::FXWindow::ID_SHOW : FX::FXWindow::ID_HIDE;
         sender->handle(this, MKUINT(msg, FX::SEL_COMMAND), ptr);

         return 1;
      }

      bool DialogBox::IsModified()
      {
         uint32 i;
         for (i=0; i<m_int_values.size(); i++)
            if (m_int_values[i] != *(m_int_pointers[i])) return true;

         for (i=0; i<m_uint_values.size(); i++)
            if (m_uint_values[i] != *(m_uint_pointers[i])) return true;

         for (i=0; i<m_float_values.size(); i++)
            if (m_float_values[i] != *(m_float_pointers[i])) return true;

         for (i=0; i<m_double_values.size(); i++)
            if (m_double_values[i] != *(m_double_pointers[i])) return true;

         for (i=0; i<m_string_values.size(); i++)
            if (m_string_values[i] != *(m_string_pointers[i])) return true;

         return false;
      }

      void DialogBox::Reset()
      {
         m_int_values.clear();
         m_int_pointers.clear();

         m_uint_values.clear();
         m_uint_pointers.clear();

         m_float_values.clear();
         m_float_pointers.clear();

         m_double_values.clear();
         m_double_pointers.clear();

         m_string_values.clear();
         m_string_pointers.clear();
      }

      void DialogBox::AddValue(sint32    &val)
      {
         m_int_values.push_back(val);
         m_int_pointers.push_back(&val);
      }

      void DialogBox::AddValue(uint32    &val)
      {
         m_uint_values.push_back(val);
         m_uint_pointers.push_back(&val);
      }

      void DialogBox::AddValue(float  &val)
      {
         m_float_values.push_back(val);
         m_float_pointers.push_back(&val);
      }

      void DialogBox::AddValue(double  &val)
      {
         m_double_values.push_back(val);
         m_double_pointers.push_back(&val);
      }

      void DialogBox::AddValue(std::string &val)
      {
         m_string_values.push_back(val);
         m_string_pointers.push_back(&val);
      }

      void DialogBox::applyDialogValues()
      {
         if (!IsModified()) return;

         uint32 i;
         for (i=0; i<m_int_values.size(); i++)
            m_int_values[i] = *(m_int_pointers[i]);

         for (i=0; i<m_uint_values.size(); i++)
            m_uint_values[i] = *(m_uint_pointers[i]) ;

         for (i=0; i<m_float_values.size(); i++)
            m_float_values[i] = *(m_float_pointers[i]);

         for (i=0; i<m_double_values.size(); i++)
            m_double_values[i] = *(m_double_pointers[i]);

         for (i=0; i<m_string_values.size(); i++)
            m_string_values[i] = *(m_string_pointers[i]);

         wasModified = true;
      }

      void DialogBox::backupDialogValues()
      {
      }

      void DialogBox::restoreDialogValues()
      {
         if (!IsModified()) return;

         uint32 i;
         for (i=0; i<m_int_values.size(); i++)
         {
            *(m_int_pointers[i]) = m_int_values[i];
         }

         for (i=0; i<m_uint_values.size(); i++)
         {
            *(m_uint_pointers[i]) = m_uint_values[i];
         }

         for (i=0; i<m_float_values.size(); i++)
         {
            *(m_float_pointers[i]) = m_float_values[i];
         }

         for (i=0; i<m_double_values.size(); i++)
         {
            *(m_double_pointers[i]) = m_double_values[i];
         }

         for (i=0; i<m_string_values.size(); i++)
         {
            *(m_string_pointers[i]) = m_string_values[i];
         }
      }
   }
}
