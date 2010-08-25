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
//  $Id: TCMTOS.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TC_MT_OS_H_
#define _TC_MT_OS_H_

#ifdef TCOS_POSIX
#   include <pthread.h>
#   include <semaphore.h>
#   include <time.h>
#   ifdef OS_LINUX
#      include <sys/time.h>
#   endif
#elif TCOS_WINDOWS
#   include <wtypes.h>
#   undef GetClassName
#   undef CreateEvent
#   undef CreateMutex
#   undef CreateSemaphore
#   undef GetMessage
#   undef DispatchMessage
#else
#   error Thread not implementd for this OS
#endif

#endif // _TC_MT_OS_H_
