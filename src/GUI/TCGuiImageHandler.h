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
//  $Id: TCGuiImageHandler.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifndef _TC_GUI_IMAGE_HANDLER_H_
#define _TC_GUI_IMAGE_HANDLER_H_

#include "TCGuiBase.h"

#include <string>

namespace TC
{
   namespace Gui
   {

      /**
      * @addtogroup TC_BASE
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of TC::GUI::ImageHandler,
      *        TC::GUI::ImageHandler::Image and TC::GUI::ImageHandler::Images
      *
      * @author Thomas Goessler
      */

      /**
      * @brief Base class for all codec
      * method fro encoding and decoding standard types
      * and prepares methods for decoding and encoding
      */

      /** Handles getting the image for an specified id */
      class TCGUI_API ImageHandler: protected NonCopyAble
      {
      public:
         class TCGUI_API Image
         {
         public:
            enum Type
            {
               BITMAP,
               GIF
            };

            Image(const char* _id, const char* _lang_id, const uchar* _image_data, Type image_type=BITMAP);

         private:
            const char* id;
            const char* lang_id;
            const uchar* image_data;
            Type type;
            FX::FXIcon* icon;

            friend class ImageHandler;
         };

         class TCGUI_API Images: protected NonCopyAble
         {
         public:
            Images(uint32 num_images, Image* images);
            ~Images();

         private:
            uint32 m_num_images;
            Image* m_images;
            Images* m_prev;

            friend class ImageHandler;
         };

         static FX::FXIcon* GetIcon(const std::string& mls_id);
         static void CleanUp();
      };

      /**
      * @}
      */
   } // namespace GUI

} // namespace TC

#endif // _TC_GUI_IMAGE_HANDLER_H_
