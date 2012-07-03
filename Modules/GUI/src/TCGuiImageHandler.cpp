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

#include "TCGuiImageHandler.h"

#include "TCUtil.h"

#include <map>

#include "TCNewEnable.h"

namespace tc
{
   namespace gui
   {
      class Image
      {
      public:
         Image(const char* _id, const char* _lang_id, const uint8_t* _image_data, ImageHandler::ImageType _type=ImageHandler::BITMAP)
         :id(_id), lang_id(_lang_id), image_data(_image_data), type(_type), icon(0)
         {
         }
         ~Image()
         {
            delete icon;
         }

      private:
         std::string id;
         std::string lang_id;
         const uint8_t* image_data;
         ImageHandler::ImageType type;
         FX::FXIcon* icon;

         friend class ImageHandlerImp;
      };

      class ImageHandlerImp: public ImageHandler
      {
      public:
         ImageHandlerImp()
         {
         }

         ~ImageHandlerImp()
         {
            util::FreeMemoryOfStlContainer(m_images);
         }

         virtual void AddImageData(const char* _id, const char* _lang_id, const uint8_t* _image_data, ImageType image_type=BITMAP)
         {
            SharedPtr<Image> image(new Image(_id, _lang_id, _image_data, image_type));
            m_images.insert(std::make_pair(std::string(_id), image));
         }

         virtual FX::FXIcon* GetIcon(const std::string& image_id)
         {
            ImageMap::iterator image_it1 = m_images.lower_bound(image_id);
            ImageMap::iterator image_it2 = m_images.upper_bound(image_id);
            if (image_it1 == m_images.end() || image_it1 == image_it2)
            {
               return 0;
            }

            ImageMap::iterator image_it = image_it1;
            if (image_it->second->icon == 0)
            {
               switch(image_it->second->type)
               {
               case BITMAP:
                  image_it->second->icon = 
                     new FX::FXBMPIcon(FX::FXApp::instance(), image_it->second->image_data,
                     0, FX::IMAGE_NEAREST);
                  image_it->second->icon->blend(FX::FXApp::instance()->getBaseColor());
                  break;
               case GIF:
                  image_it->second->icon = 
                     new FX::FXGIFIcon(FX::FXApp::instance(), image_it->second->image_data,
                     0, FX::IMAGE_NEAREST);
                  break;

               }
            }

            return image_it->second->icon;
         }

      private:
         typedef std::multimap<std::string, SharedPtr<Image> > ImageMap;
         ImageMap m_images;
      };

      static ImageHandler::Ptr s_image_handler;
      ImageHandler::Ptr ImageHandler::GetInstance()
      {
         return s_image_handler;
      }

      bool ImageHandler::CreatedInstance()
      {
         s_image_handler = ImageHandler::Ptr(new ImageHandlerImp);
         return s_image_handler;
      }

      void ImageHandler::DestroyInstance()
      {
         s_image_handler.Reset();
      }

   }
}