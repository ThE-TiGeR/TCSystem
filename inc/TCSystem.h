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

#ifndef _TC_SYSTEM_H_
#define _TC_SYSTEM_H_

#include "TCSharedPtr.h"

#include <string>
#include <vector>

namespace TC
{
   /**
   * @addtogroup TC_BASE
   * @{
   */

   /**
   * @file 
   * This header file provides the definition of the namespace TC::System. 
   * 
   * @author Thomas Goessler 
   */

   /**
   * @brief Namespace for getting operating system depending information
   */
   namespace System
   {
      /** @return The operating system name */
      TCBASE_API std::string GetOSName();
      /** @return The operating system version */
      TCBASE_API std::string GetOSVersion();
      /** @return The operating system company */
      TCBASE_API std::string GetOSCompany();

      /** @return The hostname */
      TCBASE_API std::string GetHostName();
      /** @return The Username */
      TCBASE_API std::string GetUserName();

      /** @return Number of CPU´s in the computer */
      TCBASE_API sint32 GetNumCPUs ();
      /** @return The hostname */
      TCBASE_API std::string GetCpuModel();

      /** @return CPU time needed since starttime */
      TCBASE_API uint64 GetCpuTime(uint64 starttime=0);

      /**
      * Suspends the process for specified time if supported by the system
      * otherwise it just waits specified time by checking the wall time
      *
      * @param milli_seconds Number of milli seconds to wait
      */
      TCBASE_API void Sleep(uint64 milli_seconds);

      /** @return The date information stored in a string */
      TCBASE_API std::string  GetDate();
      /** @return The string defined for environment variable */
      TCBASE_API std::string GetEnvironment(const std::string &ename);
      /** @return The process id of the current running process */
      TCBASE_API uint32 GetProcessID();
      /** @return The current thread id of the current running process */
      TCBASE_API uint32 GetCurrentThreadID();

      struct NetworkDeviceInfo
      {
         std::string ip_address;
         std::string mac_address;
      };
      /** Get info of all network devices
      * @return true sucess */
      TCBASE_API bool GetNetworkDeviceInfos(std::vector<NetworkDeviceInfo>& infos);

      /** @return The last happend error code */
      TCBASE_API sint32 GetLastError();
      /** @return The string last happend error code */
      TCBASE_API std::string  GetLastErrorMessage();
      /** @return The string of happend error code */
      TCBASE_API std::string  GetErrorMessage(sint32 error_code);

      /**
      * @brief Interface needed by the console stop handler
      */
      class ConsoleStopHandler
      {
      public:
         /** @brief Methode gets called when ctrl-c is pressed or the console is closed */
         virtual void OnConsoleStop() = 0;

         virtual ~ConsoleStopHandler() {}
      };
      typedef SharedPtr<ConsoleStopHandler> ConsoleStopHandlerPtr;

      /**
      * @brief Set the new stop handler
      * @return the old stop handler
      */
      TCBASE_API ConsoleStopHandlerPtr SetConsoleStopHandler(ConsoleStopHandlerPtr handler);

   }

   /**
   * @}
   */
}

#endif // _TC_SYSTEM_H_
