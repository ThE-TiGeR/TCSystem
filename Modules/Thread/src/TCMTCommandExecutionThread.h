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
#ifndef _TC_MT_COMMAND_EXECUTION_THREAD_H_
#define _TC_MT_COMMAND_EXECUTION_THREAD_H_

#include "TCMTCommandMessage.h"
#include "TCMTThreadObject.h"

namespace tc
{
    namespace multi_threading
    {
        class CommandExecutionThreadObject: public ThreadObject
        {
        public:
            virtual bool Run()
            {
                multi_threading::ThreadPtr thread = multi_threading::factory::GetCurrentThread();
                bool running(true);
                while (running)
                {
                    multi_threading::MessagePtr message;
                    switch (thread->WaitThreadMessage(message))
                    {
                    case multi_threading::Message::MSG_RECEIVED:
                        {
                            CommandMessage::Ptr c_message(CommandMessage::Ptr::StaticCast(message));
                            c_message->Execute();
                        }
                        break;

                    case multi_threading::Message::MSG_RECEIVE_FAILED:
                    case multi_threading::Message::MSG_RECEIVE_TIMEOUT:
                    case multi_threading::Message::MSG_QUIT_RECEIVED:
                        running = false;
                        break;
                    }

                }
                return true;
            }
        };
    }
}

#endif //_TC_MT_COMMAND_EXECUTION_THREAD_H_
