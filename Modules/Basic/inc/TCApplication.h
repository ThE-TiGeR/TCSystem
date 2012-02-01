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
#ifndef _TC_APPLICATION_H_
#define _TC_APPLICATION_H_

#include "TCNonCopyable.h"

#include <vector>
#include <string>
#include <cstdarg>

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of tc::BaseApplication
    *
    * It also defines some macros for implementing classes derived from TCBase
    *
    * @author Thomas Goessler
    */

   /**
    * @brief Base class for all applications
    * Class stores and reads settings form application.init file
    * Class stores, handles and reads messages from files
    *
    * @author Thomas Goessler
    */
   class TCBASE_API Application: protected NonCopyAble
   {
   public:
      /** default constructor */
      Application();
      /** destructor */
      virtual ~Application();
      
      /** Init the app */
      virtual bool Init(uint32 narg, char **argv,
                        const std::string &name,
                        const std::string &version,
                        const std::string &company);

      /** @return the one and only existing BaseApplication object */
      static Application* GetInstance();

      /** @return the name of the program e.g. tc*/
      inline const std::string& GetProgrammName() const      { return m_programm_name;}
      /** @return the version of the program e.g. 1.0*/
      inline const std::string& GetProgrammVersion() const   { return m_programm_version;}

   protected:
      void DisplayUsage();
      virtual bool ProcessArguments(const std::vector< std::string > &);;
      virtual std::string GetUsage();

   private:
      std::string m_programm_name;
      std::string m_programm_version;
      std::string m_programm_company;
     
      static Application *m_application;
   };

   /**
    * @}
    */

} // namespace tc

#endif // _TCBASE_APPLICATION_H_
