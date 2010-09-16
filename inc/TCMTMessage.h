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

#ifndef _TC_MT_MESSAGE_H_
#define _TC_MT_MESSAGE_H_

#include "TCMTApi.h"
#include "TCNonCopyable.h"
#include "TCSharedPtr.h"

namespace TC
{
namespace MT
{
   class Thread;
   typedef SharedPtr<Thread> ThreadPtr;

   /**
    * @addtogroup TC_MT
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TC::MT::Message
    * @author Thomas Goessler
    */

   /**
    * @brief Base class of a message which can be send 
    * from one thread to another thread
    */
   class TCMT_API Message: protected NonCopyAble
   {
   public:
      /** @brief Predefined message ids */
      enum MsgIDs
      {
         MSG_ID_QUIT       = 0x00000001, ///< Message id for quitting a thread
         MSG_ID_START      = 0x00000002, ///< Start of message ids for internal use
         MSG_ID_USER_START = 0x0000ffff, ///< Message id at which user specified massage ids should start
         MSG_ID_UNKNOWN    = 0xffffffff  ///< Undefined message id, for getting all messages
      };

      /** @brief Predefined timeout values for getting messages */
      enum TimeOut
      {
         TIMEOUT_NO = 0,
         TIMEOUT_INFINITE = 0xffffffff
      };

      /** @brief Return values of the Get message methodes */
      enum ReturnValue
      {
         MSG_RECEIVE_FAILED = 0,
         MSG_RECEIVE_TIMEOUT = 1,
         MSG_RECEIVED = 2,
         MSG_QUIT_RECEIVED = 3
      };

      /** @brief Create empty message */
      explicit Message(uint32 message_id);

      /** @brief Destruct message */
      virtual ~Message();
      /** @brief get the id of the message */
      inline uint32 GetMessageId() const {return m_message_id;}
      /** @brief get the id of the message */
      inline uint32 GetReplyId() const {return m_reply_id;}
      /** @brief get the sender thread of the message */
      inline ThreadPtr GetSenderThread() {return m_sender_thread;}

   private:
      /** global id of the message */
      uint32 m_message_id;
      /** id which is used for reply if sync message */
      uint32 m_reply_id;
      /** The sender thread */
      ThreadPtr m_sender_thread;
   };

   /** @brief Smart pointer to a message */
   typedef SharedPtr<Message> MessagePtr;

   /**
    * @}
    */

} // namespace MT
} // namespace TC


#endif // _TC_MT_MESSAGE_H_
