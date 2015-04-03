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
//  $Id: TCGuiMLS.cpp,v a33a555a5a40 2012/02/01 22:48:18 Thomas $
//*******************************************************************************

#include "TCMLSHandler.h"

#include "TCUtil.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {
      mls::Text m_gui_texts_array[] =
      {
         mls::Text("TCGUI_HELP",           "en", "\tOpen help on current dialog box\tOpen help on current dialog box"),
         mls::Text("TCGUI_OK",             "en", "&OK\tAccept values of current dialog box\tAccept values of current dialog box"),
         mls::Text("TCGUI_APPLY",          "en", "&Apply\tAccept values of current dialog box\tAccept values of current dialog box"),
         mls::Text("TCGUI_CANCEL",         "en", "&Cancel\tRestore values of current dialog box to status before opened\tRestore values of current dialog box to status before opened"),
         mls::Text("TCGUI_YES",            "en", "&Yes"),
         mls::Text("TCGUI_YESALL",         "en", "Yes, all"),
         mls::Text("TCGUI_NO",             "en", "&No"),
         mls::Text("TCGUI_NOALL",          "en", "No, all"),
      };

      mls::Texts m_gui_texts(util::ArraySize(m_gui_texts_array), m_gui_texts_array);
   }
}