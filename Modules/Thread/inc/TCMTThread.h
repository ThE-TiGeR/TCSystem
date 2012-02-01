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

#ifndef _TC_MT_THREAD_H_
#define _TC_MT_THREAD_H_

#include "TCMTMessage.h"
#include "TCMTThreadObject.h"
#include "TCNonCopyable.h"
#include "TCTime.h"

#include <string>
#include <vector>

namespace tc
{
namespace multi_threading
{
   class Message;
   typedef SharedPtr<Message> MessagePtr;

   /**
    * @addtogroup TC_MT
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of tc::multi_threading::Thread
    * @author Thomas Goessler
    */

   /**
    * @brief Interface defining thread which then can start an ThreadObject
    */
   class Thread: protected NonCopyAble
   {
   public:
      /** The priorities we can give the thread */
      enum ThreadPriority 
      {
         PRIORITY_LOWEST,
         PRIORITY_LOW,
         PRIORITY_NORMAL,
         PRIORITY_HIGH,
         PRIORITY_HIGHEST
      };

      /** enumeration the different states the thread can have */
      enum ThreadState 
      {
         /// thread object exists but thread hasn't started yet.
         STATE_NEW,        
         /// thread is running.
         STATE_RUNNING,
         /// thread has terminated but storage has not
         /// been reclaimed (i.e. waiting to be joined).
         STATE_TERMINATED  
      };

   public:
      /** */
      virtual ~Thread() {}

      /**
       * @brief Executes the Run method of the ThreadObject in the context of the thread
       * @param object_to_start Pointer to the object which to start
       * @return if successfully started
       */
      virtual bool Start(ThreadObjectPtr object_to_start) = 0;
      /**
       * @brief Join causes the calling thread to wait for the threads completion,
       * @return if successfully waited for completion
       */
      virtual bool Join() = 0;

      /** @return true if thread is running */
      virtual bool IsRunning() const = 0;
      /** @return true if thread is allready terminated */
      virtual bool IsTerminated() const = 0;

      /** @brief Set the priority of the thread. */
      virtual bool SetPriority(ThreadPriority priority) = 0;
      /** @return The thread's priority. */
      virtual ThreadPriority GetPriority() const = 0;

      /**
       * Sending Messages between threads
       * @{
       */
      /**
       * @brief Send a message to specified thread.
       * The message can be received with WaitThreadMessage
       * @param message The message to send to the thread
       * @return true if send successful
       */
      virtual bool SendThreadMessage(MessagePtr message) = 0;
      /**
       * @brief Send a sync message to specified thread.
       * The message can be received with WaitThreadMessage
       * The received message has to be sent back with SendReplyThreadMessage
       * only then this method will return
       *
       * @param message The message to send to the thread
       * @return true if send successful
       */
      virtual Message::ReturnValue SendSyncThreadMessage(MessagePtr message) = 0;
      /**
       * @brief Send the answer message to a sync message.
       *
       * @param message The message to send to the thread
       * @return true if send successful
       */
      virtual bool SendReplyThreadMessage(MessagePtr message) = 0;

      /**
       * @brief Wait for a message received in this thread
       * @param message The message received or 0 if no
       * @return true if received a message
       */
      virtual Message::ReturnValue WaitThreadMessage(MessagePtr &message) = 0;
      /**
       * @brief Wait for a message received in this thread
       * @param message The message received or 0 if no
       * @param timeout Maximum time to wait for the message in milliseconds
       * @return true if received a message
       */
      virtual Message::ReturnValue WaitThreadMessage(MessagePtr &message, const Time& timeout) = 0;
      /**
       * }@
       */
   };

   /** @brief Smart pointer to a thread */
   typedef SharedPtr< Thread > ThreadPtr;

   /**
    * @}
    */

} // namespace multi_threading
} // namespace tc


#endif // _TC_MT_THREAD_H_
