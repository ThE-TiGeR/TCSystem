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
//  $Id: TCGuiBase.h,v 177d39511c9f 2012/03/05 21:59:43 Thomas $
//*******************************************************************************

#ifndef _TCGUI_BASE_H_
#define _TCGUI_BASE_H_

#include "TCApplication.h"
#include "TCGuiApi.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4244)
#endif
#define FX_NO_GLOBAL_NAMESPACE
#include "fx.h"
#include "fxkeys.h"
#ifdef _MSC_VER
#pragma warning (pop)
#endif
#undef SEVERITY_ERROR
#undef CreateMutex

/**
 * @addtogroup TC_GUI
 * @{
 */

/** define for declaring a class based on TCGuiBase and an is also an fox class */
#define TCGUIBASE_DECLARE(a) FXDECLARE(a)

/** define for implementing a class based on TCGuiBase and an is also an fox class */
#define TCGUIBASE_IMPLEMENT(a, b, c, d) FXIMPLEMENT(a, b, c, d)

/**
 * @}
 */

#endif
