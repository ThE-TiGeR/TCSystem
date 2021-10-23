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
//  $Id: TCMTThreadWin32.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TC_MT_THREAD_WIN32_IMP_H_
#define _TC_MT_THREAD_WIN32_IMP_H_

#include "TCMTThreadBase.h"

namespace tc
{
namespace multi_threading
{
namespace imp
{
   /**
    * @addtogroup TC_MT_IMP
    * @{
    */

   /**
    * @brief Class for creating a thread which then can start an ThreadObject
    */
   class TC_DLL_LOCAL ThreadWin32: public ThreadBase
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
                              uint32_t stack_size,
                              Thread::Priority priority);
      /**
       * @return  the calling thread's Thread object.
       *          If the calling thread is not the main thread and
       *          is not created using this library, returns 0.
       */
      static ThreadPtr Self();

      /** allows another thread to run. */
      static void SwitchContext();

   public:
      ThreadWin32(const std::string& thread_name,
                  uint32_t stack_size, Thread::Priority priority);
      ThreadWin32(const std::string& thread_name, void* handle, unsigned long id);
      virtual ~ThreadWin32();

   protected:
      virtual bool CreateOSThread(InitStruct* init_data);
      virtual bool JoinOS();
      virtual bool SetPriorityOS(Priority priority);

   private:
      /** @brief Get OS dependent thread priority because of ThreadPriority */
      static int32_t GetPriority(Priority priority);
      static unsigned long __stdcall Wrapper(void *ptr);

   private:
      /** pointer to the thread handle */
      void* m_handle;
      unsigned long m_thread_id;
   };

   /**
    * @}
    */

} // namespace imp
} // namespace multi_threading
} // namespace tc

#endif // _TC_MT_THREAD_WIN32_IMP_H_
