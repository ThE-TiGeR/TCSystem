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

#ifndef _TC_MT_MESSAGE_DISPATCHER_IMP_H_
#define _TC_MT_MESSAGE_DISPATCHER_IMP_H_

#include "TCMTMessageDispatcher.h"

#include <map>

namespace TC
{
   namespace MT
   {
      namespace Impl
      {
         /**
         * @addtogroup TC_MT_IMP
         * @{
         */

         /**
         * @file
         * @brief This file provides the definition of TC::MT::Impl::MessageDispatcherImp
         * @author Thomas Goessler
         */

         class MessageDispatcherImp: public MessageDispatcher
         {
         public:
            MessageDispatcherImp()
            {
            }

            virtual void RegisterMessageCallback(uint32 id, CallBackPtr call_back)
            {
               m_callback_map[id] = call_back;
            }

            virtual bool DispatchMessage(MessagePtr message)
            {
               std::map<uint32, CallBackPtr>::iterator callback_it =
                  m_callback_map.find(message->GetMessageId());
               if (callback_it == m_callback_map.end())
               {
                  return false;
               }

               CallBackPtr m = callback_it->second;
               return m->operator()(message);
            }

         private:
            std::map<uint32, CallBackPtr> m_callback_map;
         };


         /**
         * @}
         */

      } // namespace Impl
   } // namespace MT
} // namespace TC


#endif // _TC_MT_MESSAGE_DISPATCHER_IMP_H_
