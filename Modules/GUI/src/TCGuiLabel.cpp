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
//  $Id: TCGuiLabel.cpp,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#include "TCGuiLabel.h"

#include "TCMLSHandler.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {

      TCGUIBASE_IMPLEMENT(Label, FX::FXLabel, 0, 0)

      Label::Label(FX::FXComposite * p, const std::string &text_id, FX::FXIcon* ic,
                   uint32_t opts, int32_t x, int32_t y, int32_t w, int32_t h,
                   int32_t pl, int32_t pr, int32_t pt, int32_t pb)
         :FX::FXLabel(p, mls::Handler::GetText(text_id).c_str(), ic, opts, x, y,w, h,pl,pr,pt,pb)
         ,Widget(text_id)
      {
      }

      Label::Label(FX::FXComposite * p, const std::string &text_id, FX::FXIcon* ic, FX::FXObject * tgt, FX::FXSelector sel,
         uint32_t opts, int32_t x, int32_t y, int32_t w, int32_t h,
         int32_t pl, int32_t pr, int32_t pt, int32_t pb)
         :FX::FXLabel(p, mls::Handler::GetText(text_id).c_str(), ic, opts, x, y,w, h,pl,pr,pt,pb)
         ,Widget(text_id)
      {
         setTarget(tgt); 
         setSelector(sel);
      }
   }
}
