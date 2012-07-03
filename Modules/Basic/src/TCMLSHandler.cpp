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

#include "TCMLSHandler.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace mls
   {
      const Texts* m_last_mls_texts = 0;

      std::string Handler::GetText(const std::string& mls_id)
      {
         const Texts* texts = m_last_mls_texts;
         while (texts)
         {
            for (uint32_t i=0; i<texts->m_num_texts; i++)
            {
               if (texts->m_texts[i].id == mls_id)
               {
                  return texts->m_texts[i].string;
               }
            }
            texts = texts->m_prev;
         }

         return mls_id;
      }

      Text::Text(const char* _id, const char* _lang_id, const char* _string)
         :id(_id), lang_id(_lang_id), string(_string)
      {
      }

      Texts::Texts(uint32_t num_texts, const Text* texts)
         :m_num_texts(num_texts),
         m_texts(texts),
         m_prev(0)
      {
         m_prev = m_last_mls_texts;
         m_last_mls_texts = this;
      }

      Texts::~Texts()
      {
         if (m_last_mls_texts == this)
         {
            m_last_mls_texts = m_prev;
         }
      }

   }
}