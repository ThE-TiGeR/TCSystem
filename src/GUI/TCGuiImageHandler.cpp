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
//  $Id$
//*******************************************************************************

#include "TCGuiImageHandler.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace Gui
   {
      ImageHandler::Images* m_last_image = 0;

      FX::FXIcon* ImageHandler::GetIcon(const std::string& mls_id)
      {
         const Images* images = m_last_image;
         while (images)
         {
            for (uint32 i=0; i<images->m_num_images; i++)
            {
               if (images->m_images[i].id == mls_id)
               {
                  if (images->m_images[i].icon == 0)
                  {
                     switch(images->m_images[i].type)
                     {
                     case Image::BITMAP:
                        images->m_images[i].icon = 
                           new FX::FXBMPIcon(FX::FXApp::instance(), images->m_images[i].image_data,
                           0, FX::IMAGE_NEAREST);
                        images->m_images[i].icon->blend(FX::FXApp::instance()->getBaseColor());
                        break;
                     case Image::GIF:
                        images->m_images[i].icon = 
                           new FX::FXGIFIcon(FX::FXApp::instance(), images->m_images[i].image_data,
                           0, FX::IMAGE_NEAREST);
                        break;

                     }
                  }
                  return images->m_images[i].icon;
               }
            }
            images = images->m_prev;
         }

         return 0;
      }

      void ImageHandler::CleanUp()
      {
         const Images* images = m_last_image;
         while (images)
         {
            for (uint32 i=0; i<images->m_num_images; i++)
            {
               if (images->m_images[i].icon)
               {
                  delete images->m_images[i].icon;
               }
            }
            images = images->m_prev;
         }
      }

      ImageHandler::Image::Image(const char* _id, const char* _lang_id, 
                                 const uchar* _image_data, Type _type)
         :id(_id), lang_id(_lang_id), image_data(_image_data), type(_type), icon(0)
      {
      }

      ImageHandler::Images::Images(uint32 num_images, Image* images)
         :m_num_images(num_images),
         m_images(images),
         m_prev(0)
      {
         m_prev = m_last_image;
         m_last_image = this;
      }

      ImageHandler::Images::~Images()
      {
         if (m_last_image == this)
         {
            m_last_image = m_prev;
         }
      }

   }
}