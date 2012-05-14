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

#ifndef _TC_MT_MESSAGE_DISPATCHER_H_
#define _TC_MT_MESSAGE_DISPATCHER_H_

#include "TCMTMessage.h"

namespace tc
{
   namespace multi_threading
   {
      /**
      * @addtogroup TC_MT
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of tc::multi_threading::MessageDispatcher
      * @author Thomas Goessler
      */

      /**
      * @brief ObjectClass dispatches incomming messages to 
      * registered callback functions
      */
      class MessageDispatcher
      {
      public:
         class CallBack
         {
         public:
            virtual bool operator()(MessagePtr message) = 0;
            virtual ~CallBack() {}
         };
         typedef SharedPtr<CallBack> CallBackPtr;

         virtual void RegisterMessageCallback(uint32 id, CallBackPtr call_back) = 0;

         template <class MessageClass, class ObjectClass, typename CallBackMethod>
         void RegisterMessageCallback(uint32 id, SharedPtr<ObjectClass> class_instance, 
            CallBackMethod call_back);

         virtual bool DispatchMessage(MessagePtr message) = 0;

         virtual ~MessageDispatcher() {}
      };

      typedef SharedPtr<MessageDispatcher> MessageDispatcherPtr;

      template <class MessageClass, class ObjectClass>
      class ClassMethodeCallBack: public MessageDispatcher::CallBack
      {
      public:
         typedef bool (ObjectClass::*Method)(SharedPtr<MessageClass>);

         ClassMethodeCallBack(SharedPtr<ObjectClass> _class_instance, Method _method)
            :m_class_instance(_class_instance),
            m_method(_method)
         {
         };

         bool operator()(MessagePtr message)
         {
            ObjectClass* _class_instance = m_class_instance.operator->();
            return (_class_instance->*m_method)(SharedPtr<MessageClass>::StaticCast(message));
         };

      private:
         SharedPtr<ObjectClass> m_class_instance;
         Method m_method;
      };

      template <class MessageClass, class ObjectClass, typename CallBackMethod>
      void MessageDispatcher::RegisterMessageCallback(uint32 id, SharedPtr<ObjectClass> class_instance, 
         CallBackMethod call_back)
      {
         RegisterMessageCallback(id, 
            CallBackPtr(new ClassMethodeCallBack<MessageClass, ObjectClass>(class_instance, call_back)));
      }

      /**
      * @}
      */

   } // namespace multi_threading
} // namespace tc


#endif // _TC_MT_MESSAGE_DISPATCHER_H_
