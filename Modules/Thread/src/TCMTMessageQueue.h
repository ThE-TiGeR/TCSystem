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
// see https://bitbucket.org/the____tiger/tcsystem for details.
// Copyright (C) 2003 - 2014 Thomas Goessler. All Rights Reserved. 
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

#ifndef _TC_MT_MESSAGE_QUEUE_H_
#define _TC_MT_MESSAGE_QUEUE_H_

#include "TCMTEvent.h"
#include "TCMTLockable.h"
#include "TCMTMessage.h"
#include "TCTime.h"

#include <vector>

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {
         /**
         * @addtogroup TC_MT_IMP
         * @{
         */

         /**
         * @brief internal message queue class used by tc::multi_threading::Message and tc::multi_threading::Thread only
         * Handles receiving and waiting on messages by setting and waiting on an event object
         * Should only be used from class Message and Thread
         */
         class TC_DLL_LOCAL MessageQueue: protected ObjectLevelLockable<MessageQueue>
         {
         protected:
            MessageQueue();
            virtual ~MessageQueue();

            bool AddMessage(MessagePtr message);
            Message::ReturnValue GetMessage(uint32_t msg_id_start, uint32_t msg_id_end, 
               MessagePtr& message);
            Message::ReturnValue GetMessage(uint32_t msg_id_start, uint32_t msg_id_end, 
               MessagePtr& message, const Time& timeout);
            Message::ReturnValue TryGetMessage(uint32_t msg_id_start, uint32_t msg_id_end, 
               MessagePtr& message, bool remove);

         private:
            Message::ReturnValue GetMessageFromQueue(uint32_t msg_id_start, uint32_t msg_id_end, 
               MessagePtr& message, bool remove);
         private:
            std::vector<MessagePtr> m_messages;
            EventPtr m_new_message_event;

            friend class ThreadBase;
            friend class LockerPtr<const MessageQueue*>;
         };

         /**
         * @}
         */

      } // namespace imp
   } // namespace multi_threading
} // namespace tc

#endif // _TC_MT_MESSAGE_QUEUE_H_
