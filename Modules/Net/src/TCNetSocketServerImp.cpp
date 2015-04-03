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
//  $Id: TCNetSocketServerImp.cpp,v 5104760d9002 2014/09/26 18:20:17 thomas $
//*******************************************************************************

#include "TCNetSocketServerImp.h"

#include "TCMTFactory.h"
#include "TCMTMessage.h"
#include "TCNetSocket.h"
#include "TCNetUtil.h"
#include "TCOutput.h"
#include "TCSystem.h"
#include "TCUtil.h"

#include "TCNetOS.h"

#include <algorithm>

#include "TCNewEnable.h"

namespace tc
{
   namespace net
   {
      namespace imp
      {

         class TC_DLL_LOCAL StartMessage: public multi_threading::Message
         {
         public:
            enum { MESSAGE_ID = multi_threading::Message::MSG_ID_USER_START+1 };
            StartMessage(bool sync)
               :multi_threading::Message(MESSAGE_ID)
               ,m_sync(sync)
            {}

            const bool m_sync;
         };

         class TC_DLL_LOCAL StopMessage: public multi_threading::Message
         {
         public:
            enum { MESSAGE_ID = multi_threading::Message::MSG_ID_USER_START+2 };
            StopMessage():multi_threading::Message(MESSAGE_ID) {}
         };

         class TC_DLL_LOCAL AddSocketMessage: public multi_threading::Message
         {
         public:
            enum { MESSAGE_ID = multi_threading::Message::MSG_ID_USER_START+3 };
            AddSocketMessage(SocketPtr socket_to_add,
               SocketServerImp::DataReceiverPtr receiver_to_add)
               :multi_threading::Message(MESSAGE_ID),
               m_socket_to_add(socket_to_add),
               m_receiver_to_add(receiver_to_add)
            {
            }
            SocketPtr m_socket_to_add;
            SocketServerImp::DataReceiverPtr m_receiver_to_add;
         };

         class TC_DLL_LOCAL RemoveSocketMessage: public multi_threading::Message
         {
         public:
            enum { MESSAGE_ID = multi_threading::Message::MSG_ID_USER_START+4 };
            RemoveSocketMessage(SocketPtr socket_to_remove)
               :multi_threading::Message(MESSAGE_ID),
               m_socket_to_remove(socket_to_remove)
            {
            }
            SocketPtr m_socket_to_remove;
         };

         class TC_DLL_LOCAL ThreadObject: public multi_threading::ThreadObject
         {
         public:
            ThreadObject(SocketServerImp* server)
               :m_server(server)
            {
            }
         protected:
            bool Run()
            {
               return m_server->Run();
            }

            SocketServerImp* m_server;
         };

         SocketServerImp::SocketServerImp()
         {
            TCTRACES("TCNET", 10, "");

            m_server_thread = multi_threading::factory::CreateThread("SocketServerImp");
            m_server_thread->Start(multi_threading::ThreadObjectPtr(new ThreadObject(this)));
         }

         SocketServerImp::~SocketServerImp()
         {
            TCTRACES("TCNET", 50, "");

            Stop(true);
         }

         bool SocketServerImp::HandleMessage(multi_threading::MessagePtr message)
         {
            TCTRACES("TCNET", 50, "(" << message->GetMessageId() << ")");

            switch(message->GetMessageId())
            {
            case StartMessage::MESSAGE_ID:
               {
                  TCTRACES("TCNET", 50, "StartMessage");
                  m_current_timeout = Time::Zero();
                  if (SharedPtr<StartMessage>::StaticCast(message)->m_sync)
                  {
                     message->GetSenderThread()->SendReplyThreadMessage(message);
                  }
                  return true;
               }

            case StopMessage::MESSAGE_ID:
               {
                  TCTRACES("TCNET", 50, "StopMessage");
                  m_running = false;
                  return true;
               }

            case AddSocketMessage::MESSAGE_ID:
               {
                  SharedPtr<AddSocketMessage> add_message = SharedPtr<AddSocketMessage>::StaticCast(message);

                  m_sockets.push_back(add_message->m_socket_to_add);
                  m_receivers.push_back(add_message->m_receiver_to_add);

                  return true;
               }

            case RemoveSocketMessage::MESSAGE_ID:
               {
                  SharedPtr<RemoveSocketMessage> remove_message = SharedPtr<RemoveSocketMessage>::StaticCast(message);

                  std::vector<SocketPtr>::iterator socket_it;
                  socket_it = std::find(m_sockets.begin(), m_sockets.end(),
                     remove_message->m_socket_to_remove);
                  if (socket_it != m_sockets.end())
                  {
                     std::vector<SocketPtr>::size_type pos = socket_it - m_sockets.begin();
                     m_sockets.erase(socket_it);
                     m_receivers.erase(m_receivers.begin() + pos);
                  }

                  return true;
               }
            };
            return false;
         }

         bool SocketServerImp::Run()
         {
            TCTRACES("TCNET", 50, "");   

            m_running = true;
            m_current_timeout = Time::Infinite();

            multi_threading::MessagePtr message;
            while (m_running)
            {
               if (m_server_thread->WaitThreadMessage(message, m_current_timeout) !=
                  multi_threading::Message::MSG_RECEIVE_FAILED)
               {
                  if (!HandleMessage(message))
                  {
                     TCERRORS("TCNET", "Message not handled id=" << message->GetMessageId());   
                  }
               }
               else
               {
                  Accept();
               }
            };

            TCTRACES("TCNET", 50, "done.");   
            return true;
         }

         bool SocketServerImp::Start(bool wait_started)
         {
            TCTRACES("TCNET", 50, "");

            if (m_server_thread->IsRunning())
            {
               if (wait_started)
               {
                  m_server_thread->SendSyncThreadMessage(multi_threading::MessagePtr(new StartMessage(true)));
               }
               else
               {
                  m_server_thread->SendThreadMessage(multi_threading::MessagePtr(new StartMessage(false)));
               }
            }

            return true;
         }

         void SocketServerImp::Stop(bool wait_stopped)
         {
            TCTRACES("TCNET", 50, "");

            if (m_server_thread->IsRunning())
            {
               m_server_thread->SendThreadMessage(multi_threading::MessagePtr(new StopMessage));

               if (wait_stopped)
               {
                  m_server_thread->Join();
               }
            }
         }

         bool SocketServerImp::AddSocket(SocketPtr socket,
            DataReceiverPtr data_receiver)
         {
            if (!m_server_thread->IsRunning())
            {
               return false;
            }

            return m_server_thread->SendThreadMessage(multi_threading::MessagePtr(new AddSocketMessage(socket, data_receiver)));
         }

         bool SocketServerImp::RemoveSocket(SocketPtr socket)
         {
            if (!m_server_thread->IsRunning())
            {
               return false;
            }

            return m_server_thread->SendThreadMessage(multi_threading::MessagePtr(new RemoveSocketMessage(socket)));
         }

         bool SocketServerImp::Accept()
         {
            TCTRACES("TCNET", 100, "");

            // init read set
            fd_set read_set;
            FD_ZERO(&read_set);
            fd_set write_set;
            FD_ZERO(&write_set);
            fd_set except_set;
            FD_ZERO(&except_set);

            // add all existing connections
            // and get max id
            SocketId max_id = 0;
            uint32_t i;
            for (i=0; i<m_sockets.size(); i++)
            {
               if (m_sockets[i] && m_sockets[i]->IsOpened())
               {
#ifdef _MSC_VER
                  // disable warning warning C4127: conditional expression is constant
                  // happens for FD_SET
#   pragma warning (disable: 4127)
#endif

                  FD_SET(m_sockets[i]->GetSocket(), &read_set);
                  max_id = tc::util::Max(max_id, m_sockets[i]->GetSocket());
               }
            }

            // check if we found something
            if (max_id != 0)
            {
               struct timeval timeout = {0, SELECT_TIMEOUT * 1000};

               int32_t s = ::select(static_cast<int32_t>(max_id + 1), &read_set, &write_set, &except_set, &timeout);
               if (s > 0)
               {
                  for (i=0; i<m_sockets.size(); i++)
                  {
                     if (m_sockets[i] && m_sockets[i]->IsOpened())
                     {
                        if (m_sockets[i] && FD_ISSET(m_sockets[i]->GetSocket(), &read_set))
                        {
                           m_receivers[i]->OnNewData(m_sockets[i]);
                        }
                     }
                  }
               }
               else if (s == 0)
               {
#ifndef TCOS_ANDROID
#ifdef TCOS_WINDOWS
                  int error = ::WSAGetLastError();
#else
                  int error = system::GetLastError();
#endif
                  if (error != 0)
                  {
                     TCERRORS("TCNET", "select error " << error << "(" << system::GetErrorMessage(error) << ")");
                  }
#endif
               }
               else if (s < 0)
               {
                  util::PrintSocketError("Select error", true);
                  return false;
               }
            }
            else
            {
               system::Sleep(SELECT_TIMEOUT);
            }

            return true;
         }

      }
   }
}

