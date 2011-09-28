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

namespace TC
{
namespace Net
{
namespace Impl
{

class TC_DLL_LOCAL StartMessage: public MT::Message
{
public:
   enum { MESSAGE_ID = MT::Message::MSG_ID_USER_START+1 };
   StartMessage():MT::Message(MESSAGE_ID) {}
};

class TC_DLL_LOCAL StopMessage: public MT::Message
{
public:
   enum { MESSAGE_ID = MT::Message::MSG_ID_USER_START+2 };
   StopMessage():MT::Message(MESSAGE_ID) {}
};

class TC_DLL_LOCAL AddSocketMessage: public MT::Message
{
public:
   enum { MESSAGE_ID = MT::Message::MSG_ID_USER_START+3 };
   AddSocketMessage(SocketPtr socket_to_add,
      SocketServerImp::DataReceiverPtr receiver_to_add)
      :MT::Message(MESSAGE_ID),
      m_socket_to_add(socket_to_add),
      m_receiver_to_add(receiver_to_add)
   {
   }
   SocketPtr m_socket_to_add;
   SocketServerImp::DataReceiverPtr m_receiver_to_add;
};

class TC_DLL_LOCAL RemoveSocketMessage: public MT::Message
{
public:
   enum { MESSAGE_ID = MT::Message::MSG_ID_USER_START+4 };
   RemoveSocketMessage(SocketPtr socket_to_remove)
      :MT::Message(MESSAGE_ID),
      m_socket_to_remove(socket_to_remove)
   {
   }
   SocketPtr m_socket_to_remove;
};

class TC_DLL_LOCAL ThreadObject: public MT::ThreadObject
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
   TCTRACEF("TCNET", 10);

   m_server_thread = MT::Factory::CreateThread("SocketServerImp");
   m_server_thread->Start(MT::ThreadObjectPtr(new ThreadObject(this)));
}

SocketServerImp::~SocketServerImp()
{
   TCTRACEF("TCNET", 50);

   Stop(true);
}

bool SocketServerImp::HandleMessage(MT::MessagePtr message)
{
   TCTRACE1("TCNET", 50, "(%d)", message->GetMessageId());

   switch(message->GetMessageId())
   {
   case StartMessage::MESSAGE_ID:
      {
         TCTRACE("TCNET", 50, "StartMessage");
         m_current_timeout = Time::Zero();
         return true;
      }

   case StopMessage::MESSAGE_ID:
      {
         TCTRACE("TCNET", 50, "StopMessage");
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
   TCTRACEF("TCNET", 50);   

   m_running = true;
   m_current_timeout = Time::Infinite();

   MT::MessagePtr message;
   while (m_running)
   {
      if (m_server_thread->WaitThreadMessage(message, m_current_timeout) !=
         MT::Message::MSG_RECEIVE_FAILED)
      {
         if (!HandleMessage(message))
         {
            TCERROR1("TCNET", "Message not handled id=%d", message->GetMessageId());   
         }
      }
      else
      {
         Accept();
      }
   };

   TCTRACE("TCNET", 50, "done.");   
   return true;
}

void SocketServerImp::Start(bool wait_started)
{
   TCTRACEF("TCNET", 50);

   if (m_server_thread->IsRunning())
   {
      m_server_thread->SendThreadMessage(MT::MessagePtr(new StartMessage));

      if (wait_started)
      {
      }
   }
}

void SocketServerImp::Stop(bool wait_stopped)
{
   TCTRACEF("TCNET", 50);
   
   if (m_server_thread->IsRunning())
   {
      m_server_thread->SendThreadMessage(MT::MessagePtr(new StopMessage));

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

   return m_server_thread->SendThreadMessage(MT::MessagePtr(new AddSocketMessage(socket, data_receiver)));
}

bool SocketServerImp::RemoveSocket(SocketPtr socket)
{
   if (!m_server_thread->IsRunning())
   {
      return false;
   }

   return m_server_thread->SendThreadMessage(MT::MessagePtr(new RemoveSocketMessage(socket)));
}

bool SocketServerImp::Accept()
{
   TCTRACEF("TCNET", 100);

   // init read set
   fd_set read_set;
   FD_ZERO(&read_set);

   // add all existing connections
   // and get max id
   SocketId max_id = 0;
   uint32 i;
   for (i=0; i<m_sockets.size(); i++)
   {
      if (m_sockets[i] &&
          m_sockets[i]->IsOpened())
      {
#ifdef _MSC_VER
         // disable warning warning C4127: conditional expression is constant
         // happens for FD_SET
#   pragma warning (disable: 4127)
#endif

         FD_SET(m_sockets[i]->GetSocket(), &read_set);
         max_id = TC::Util::Max(max_id, m_sockets[i]->GetSocket());
      }
   }

   // check if we found something
   if (max_id != 0)
   {
      struct timeval timeout = {0, SELECT_TIMEOUT * 1000};

      sint32 s = ::select(static_cast<sint32>(max_id + 1),
         &read_set, 0, 0, &timeout);
      if (s > 0)
      {
         for (i=0; i<m_sockets.size(); i++)
         {
            if (m_sockets[i] &&
               m_sockets[i]->IsOpened())
            {
               if (m_sockets[i] &&
                  FD_ISSET(m_sockets[i]->GetSocket(), &read_set))
               {
                  //tcdebug << "read set " << m_sockets[i]->GetSocket() << endl;
                  m_receivers[i]->OnNewData();
               }
            }
         }
      }
      else if (s == 0)
      {
#ifdef TCOS_WINDOWS
         int error = ::WSAGetLastError();
         std::string error_str = System::GetLastErrorMessage();
#else
         int error = System::GetLastError();
         std::string error_str = System::GetLastErrorMessage();
#endif
         if (error != 0)
         {
            TCERROR1("TCNET", "select error %s", error_str.c_str());
         }
      }
      else if (s < 0)
      {
         TCERROR1("TCNET", "select error %s", 
                  System::GetLastErrorMessage().c_str());
         return false;
      }
   }
   else
   {
      System::Sleep(SELECT_TIMEOUT);
   }

   return true;
}

}
}
}

