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
//  $Id: TCGuiApplication.h,v c0056a2f47c7 2012/07/03 21:05:30 thomas $
//*******************************************************************************

#ifndef _TC_GUI_APPLICATION_H_
#define _TC_GUI_APPLICATION_H_

#include "TCApplication.h"
#include "TCGuiBase.h"
#include "TCHashTable.h"

namespace tc
{
   namespace gui
   {

      /**
      * @addtogroup TC_GUI
      * @{
      */

      /**
      * @brief gui Application base class
      * Provides funcions for getting icons, cursurs and texts because of ids
      */
      class TCGUI_API Application: public tc::Application
      {
      public:   
         Application();
         virtual ~Application();
         virtual bool Init(uint32_t narg, char **argv,
            const std::string &name, const std::string &version,
            const std::string &company);

         virtual bool Run();
         virtual void Exit(int32_t exitCode);
      protected:
         virtual FX::FXMainWindow* CreateMainWindow() = 0;
      private:
         FX::FXApp* m_fx_app;
         FX::FXMainWindow* m_main_window;
      };

      /**
      * @}
      */
   }
}

#endif // _TC_GUI_APPLICATION_H_
