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
#include "TCNew.h"

#include "TCNewEnable.h"
#include "TCNewDisable.h"

#include <cstdlib>

#if TC_USE_MEM_CHECK

#ifndef _Ret_notnull_
#define _Ret_notnull_
#endif

#ifndef _Post_writable_byte_size_
#define _Post_writable_byte_size_(SIZE)
#endif

_Ret_notnull_ _Post_writable_byte_size_(size) void* TC_CRT_DECL operator new(size_t size) TC_NEW_THROW
{
   return tc::system::GetInstance()->Alloc(static_cast<tc::uint32_t>(size), "unknown new", 0);
}
void* TC_CRT_DECL operator new[](size_t size) TC_NEW_THROW
{
   return tc::system::GetInstance()->Alloc(static_cast<tc::uint32_t>(size), "unknown new[]", 1);
}
void* TC_CRT_DECL operator new(size_t size, const char *fileName, int line) TC_NEW_THROW
{
   return tc::system::GetInstance()->Alloc(static_cast<tc::uint32_t>(size), fileName, line);
}
void* TC_CRT_DECL operator new[](size_t size, const char *fileName, int line) TC_NEW_THROW
{
   return tc::system::GetInstance()->Alloc(static_cast<tc::uint32_t>(size), fileName, line);
}

void operator delete(void *p) TC_DEL_THROW
{
   if (tc::system::GetInstance())
      tc::system::GetInstance()->Free(p);
   else
       std::free(p);
}

void operator delete[](void *p) TC_DEL_THROW
{
   if (tc::system::GetInstance())
      tc::system::GetInstance()->Free(p);
   else
      std::free(p);
}
void operator delete(void *p, const char *, int) TC_DEL_THROW
{
   if (tc::system::GetInstance())
      tc::system::GetInstance()->Free(p);
   else
      std::free(p);
}
void operator delete[](void *p, const char *, int) TC_DEL_THROW
{
   if (tc::system::GetInstance())
      tc::system::GetInstance()->Free(p);
   else
      std::free(p);
}

#endif
