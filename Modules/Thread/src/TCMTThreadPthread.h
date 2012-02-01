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

#ifndef _TC_MT_THREAD_PTHREAD_H_
#define _TC_MT_THREAD_PTHREAD_H_

#include "TCMTThreadBase.h"

namespace tc
{
namespace multi_threading
{
namespace imp
{

   class MessageQueue;

   /**
    * @addtogroup TC_MT_IMP
    * @{
    */

   /**
    * @brief Class for creating a thread which then can start an ThreadObject
    */
   class TC_DLL_LOCAL ThreadPthread: public ThreadBase
   {
   public:
      /**
       * @brief Create a thread object
       *
       * When returning the thread is already in the running state
       * @param thread_name Name of the thread
       * @param stack_size Stack size of the thread 0 = system default stack size
       * @param priority Priority of the thread @see ThreadPriority
       */
      static ThreadPtr Create(const std::string& thread_name,
                              uint32 stack_size,
                              ThreadPriority priority);
      /**
       * @return  the calling thread's Thread object.
       *          If the calling thread is not the main thread and
       *          is not created using this library, returns 0.
       */
      static ThreadPtr Self();

      /** allows another thread to run. */
      static void SwitchContext();

   public:
      ThreadPthread(const std::string& thread_name,
                  uint32 stack_size, ThreadPriority priority);
      ThreadPthread(const std::string& thread_name, pthread_t handle);
      virtual ~ThreadPthread();

   protected:
      virtual bool CreateOSThread(InitStruct* init_data);
      virtual bool JoinOS();
      virtual bool SetPriorityOS(ThreadPriority priority);

   private:
      /** @brief Get OS dependent thread priority because of ThreadPriority */
      static sint32 GetPriority(ThreadPriority priority);
      static void* Wrapper(void* ptr);

   private:
      /** pointer to the thread handle */
      pthread_t m_handle;
   };

   /**
    * @}
    */

} // namespace imp
} // namespace multi_threading
} // namespace tc

#endif // _TC_MT_THREAD_PTHREAD_H_
