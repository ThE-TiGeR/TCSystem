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
//  $Id: TCNetApi.h,v 655ae364031f 2014/09/26 18:19:58 thomas $
//*******************************************************************************

#ifndef _TC_NET_API_H_
#define _TC_NET_API_H_

#include "TCDefines.h"

/**
 * @addtogroup TC_NET
 * @{
 */

/**
 * @file
 * @brief This file provides the definition of #TCNET_API
 *
 * If TCMT_EXPORTS is defined the define #TCNET_API is defined for exporting classes
 * out of an dll. otherwise for importing.
 * @author Thomas Goessler
 */
#ifdef TCNET_EXPORTS
#  define TCNET_API TC_EXPORT_DLL
#else
   /**
    * @brief define for exporting or importing classes and functions from the TC::Net dll
    */
#  ifdef TC_COMPILE_STATIC_LIBS
#     define TCNET_API
#  else
#     define TCNET_API TC_IMPORT_DLL
#endif
#endif

/**
 * @}
 */

#endif // _TC_NET_API_H_
