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
//  $Id: TCGuiLabel.cpp 987 2010-05-17 08:28:04Z the_____tiger $
//*******************************************************************************

#include "TCGuiLabel.h"

#include "TCMLSHandler.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Gui
   {

      TCGUIBASE_IMPLEMENT(Label, FX::FXLabel, 0, 0)

      Label::Label(FX::FXComposite * p, const std::string &text_id, FX::FXIcon* ic,
                   uint32 opts, sint32 x, sint32 y, sint32 w, sint32 h,
                   sint32 pl, sint32 pr, sint32 pt, sint32 pb)
         :FX::FXLabel(p, MLS::Handler::GetText(text_id).c_str(), ic, opts, x, y,w, h,pl,pr,pt,pb)
         ,Widget(text_id)
      {
      }

      Label::Label(FX::FXComposite * p, const std::string &text_id, FX::FXIcon* ic, FX::FXObject * tgt, FX::FXSelector sel,
         uint32 opts, sint32 x, sint32 y, sint32 w, sint32 h,
         sint32 pl, sint32 pr, sint32 pt, sint32 pb)
         :FX::FXLabel(p, MLS::Handler::GetText(text_id).c_str(), ic, opts, x, y,w, h,pl,pr,pt,pb)
         ,Widget(text_id)
      {
         setTarget(tgt); 
         setSelector(sel);
      }
   }
}
