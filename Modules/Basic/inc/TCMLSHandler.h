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
#ifndef _TC_MLS_HANDLER_H_
#define _TC_MLS_HANDLER_H_

#include "TCTypes.h"

#include <string>

namespace tc
{
   /**
   * @addtogroup TC_BASE
   * @{
   */

   /**
   * @file
   * @brief This file provides the definition of tc::mls::Handler,
   *        tc::mls::Text and tc::mls::Texts
   *
   * @author Thomas Goessler
   */

   /**
   * @brief Base class for all codec
   * method fro encoding and decoding standard types
   * and prepares methods for decoding and encoding
   */
   namespace mls
   {
      /** Handles getting the text for an specified mls id */
      class TCBASE_API Handler
      {
      public:
         /** Get the text for specified multi language id */
         static std::string GetText(const std::string& mls_id);
      };

      /** Class defines a multi language string */
      class TCBASE_API Text
      {
      public:
         Text(const char* _id, const char* _lang_id, const char* _string);

      private:
         const char* id;
         const char* lang_id;
         const char* string;

         friend class Handler;
      };

      /** Class holds an array of multi language string */
      class TCBASE_API Texts
      {
      public:
         Texts(uint32 num_texts, const Text* texts);
         ~Texts();

      private:
         uint32 m_num_texts;
         const Text* m_texts;
         const Texts* m_prev;

         friend class Handler;
      };
     
   }

   /**
   * @}
   */

} // namespace tc

#endif // _TC_MLS_HANDLER_H_
