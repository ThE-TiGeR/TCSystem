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
//  $Id: TCApplication.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCApplication.h"

#include "TCAssert.h"
#include "TCFactory.h"
#include "TCFile.h"
#include "TCFileName.h"
#include "TCMLSHandler.h"
#include "TCOutput.h"
#include "TCString.h"
#include "TCSystem.h"

#include <cstdlib>

#include "TCNewEnable.h"

namespace tc
{

   TC_CT_ASSERT(sizeof(int64_t) == 8);
   TC_CT_ASSERT(sizeof(int32_t) == 4);
   TC_CT_ASSERT(sizeof(int16_t) == 2);
   TC_CT_ASSERT(sizeof(int8_t)  == 1);

   TC_CT_ASSERT(sizeof(uint64_t) == 8);
   TC_CT_ASSERT(sizeof(uint32_t) == 4);
   TC_CT_ASSERT(sizeof(uint16_t) == 2);
   TC_CT_ASSERT(sizeof(uint8_t)  == 1);

   TC_CT_ASSERT(sizeof(double) == 8);
   TC_CT_ASSERT(sizeof(float) == 4);

   Application *Application::m_application = 0;

   Application::Application()
   {
      if (m_application == 0)
      {
         m_application = this;
      }
      else
      {
         TCERRORS("TCBASE", "It is not allowed to create two applications");
         std::exit(7);
      }
   }

   Application::~Application()
   {
   }

   Application* Application::GetInstance()
   {
      TC_ASSERT(m_application != 0); 
      return m_application;
   }

   bool Application::Init(uint32_t narg, char **argv, const std::string& name, 
      const std::string& version, const std::string& company)
   {
      m_programm_name    = name;
      m_programm_version = version;
      m_programm_company = company;

      std::vector<std::string> a;
      uint32_t i;
      for (i=1; i<narg; i++)
      {
         a.push_back(argv[i]);
      }

      for (std::vector<std::string>::iterator it=a.begin(); it!=a.end();)
      {
         std::string currentArg = string::Replace(*it, '/', '-');

         if (currentArg == "-h" ||
             currentArg == "--help")
         {
            DisplayUsage();
            return false;
         }
         else if (currentArg == "-v" ||
            currentArg == "--version")
         {
            factory::CreateStdOutStream() << m_programm_name << " " <<  version << endl;
            return false;
         }
         else if (currentArg == "-t" ||
            currentArg == "--trace")
         {
            it = a.erase(it);
            output::SetTraceLevel(string::ToSint32(*it));
            it = a.erase(it);
         }
         else
         {
            it++;
         }
      }

      return ProcessArguments(a);
   }

   void Application::DisplayUsage()
   {
      StreamPtr stream(factory::CreateStdOutStream());
      stream << "Usage: " << GetUsage() << endl;
      stream << "\t\t" << "[-trace number (set the trace number)]" << endl;
   }

   bool Application::ProcessArguments(const std::vector< std::string >& args)
   {
      return args.size() == 0;
   }

   std::string Application::GetUsage()
   {
      return "";
   }
}
