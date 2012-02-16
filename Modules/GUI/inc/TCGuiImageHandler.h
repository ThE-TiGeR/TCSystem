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
#ifndef _TC_GUI_IMAGE_HANDLER_H_
#define _TC_GUI_IMAGE_HANDLER_H_

#include "TCGuiBase.h"
#include "TCSharedPtr.h"

#include <string>

namespace tc
{
   namespace gui
   {

      /**
      * @addtogroup TC_BASE
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of tc::GUI::ImageHandler,
      *        tc::GUI::ImageHandler::Image and tc::GUI::ImageHandler::Images
      *
      * @author Thomas Goessler
      */

      /**
      * @brief Base class for all codec
      * method fro encoding and decoding standard types
      * and prepares methods for decoding and encoding
      */

      /** Handles getting the image for an specified id */
      class ImageHandler
      {
      public:
         typedef SharedPtr<ImageHandler> Ptr;

         enum ImageType
         {
            BITMAP,
            GIF
         };

      public:
         static TCGUI_API bool CreatedInstance();
         static TCGUI_API void DestroyInstance();
         static TCGUI_API ImageHandler::Ptr GetInstance();

         virtual void AddImageData(const char* _id, const char* _lang_id, const uchar* _image_data, ImageType image_type=BITMAP) = 0;
         virtual FX::FXIcon* GetIcon(const std::string& image_id) = 0;
      };

      /**
      * @}
      */
   } // namespace gui

} // namespace tc

#endif // _TC_GUI_IMAGE_HANDLER_H_
