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
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCMTSemaphorePthread.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#include "TCDefines.h"

#ifdef TCOS_POSIX

#include "TCMTSemaphorePthread.h"

#include "TCMTOS.h"
#include "TCTime.h"

#include <fcntl.h>
#include <sys/stat.h>


#include "TCNewEnable.h"

namespace tc
{
   namespace multi_threading
   {
      namespace imp
      {

         SemaphorePthread::SemaphorePthread() 
            :m_semaphore(SEM_FAILED)
            ,m_name()
            ,m_shared(false)
         {
         }

         SemaphorePthread::~SemaphorePthread()
         {
            if (m_semaphore != SEM_FAILED)
            {
               if (m_shared)
               {
                  ::sem_close(m_semaphore);
                  if (m_name.length() > 0)
                  {
                     ::sem_unlink(m_name.c_str());
                  }
               }
               else
               {
                  ::sem_destroy(m_semaphore);
                  delete m_semaphore;
               }

               m_semaphore = SEM_FAILED;
            }
         }

         bool SemaphorePthread::Init(uint32_t initial)
         {
            m_semaphore = new sem_t;
            if (::sem_init(m_semaphore, 0, initial) != 0)
            {
               delete m_semaphore;
               m_semaphore = SEM_FAILED;
               ::perror("::sem_init failed");
               return false;
            }

            return true;
         }

         bool SemaphorePthread::Init(const std::string& shared_name, uint32_t initial, factory::CreationMode mode)
         {
            switch(mode)
            {
            case factory::CreationMode::ALWAYS:
               m_semaphore = ::sem_open(shared_name.c_str(), O_CREAT, S_IRWXU, initial);
               m_name = shared_name;
               break;
            case factory::CreationMode::WHEN_EXISTS:
               m_semaphore = ::sem_open(shared_name.c_str(), O_CREAT|O_EXCL, S_IRWXU, initial);
               // if unable to open it allready exists so it is ok
               if (m_semaphore == SEM_FAILED)
               {
                  m_semaphore = ::sem_open(shared_name.c_str(), O_CREAT, 0, initial);
               }
               else
               {
                  ::sem_close(m_semaphore);
                  m_semaphore = SEM_FAILED;
               }
               break;
            case factory::CreationMode::WHEN_NOT_EXISTS:
               m_semaphore = ::sem_open(shared_name.c_str(), O_CREAT|O_EXCL, S_IRWXU, initial);
               m_name = shared_name;
               break;
            }
            m_shared = true;
            return m_semaphore != SEM_FAILED;
         }

         bool SemaphorePthread::Wait()
         {
            while (::sem_wait(m_semaphore) != 0)
            {
               if (errno == EINTR)
               {
                  continue;
               }

               ::perror("::sem_wait failed");
               return false;
            }

            return true;
         }

         bool SemaphorePthread::Try()
         {
            return ::sem_trywait(m_semaphore) == 0;
         }

         bool SemaphorePthread::TryWait(const Time& timeout)
         {
            Time time = Time::Now() + timeout;
            timespec t = {time_t(time.Seconds()), time_t(time.NanoSeconds())};

            while (::sem_timedwait(m_semaphore, &t) != 0)
            {
               if (errno == EINTR)
               {
                  continue;
               }

               if (errno == ETIMEDOUT)
               {
                  return false;
               }

               ::perror("::sem_timedwait failed");
               return false;
            }

            return true;
         }

         bool SemaphorePthread::Post()
         {
            if (::sem_post(m_semaphore) != 0)
            {
               ::perror("::sem_post failed");
               return false;
            }

            return true;
         }

      } // namespace imp
   } // namespace multi_threading
} // namespace tc

#endif // TCOS_POSIX
