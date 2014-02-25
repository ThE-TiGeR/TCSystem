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
#ifndef _TCBASE_TYPES_H_
#define _TCBASE_TYPES_H_

#include "TCDefines.h"

#include <cstdint>
#include <cstddef>

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of global data types
    *
    * @author Thomas Goessler
    */

   /** @brief typedef for 8bit signed int */
   using std::int8_t;
   /** @brief typedef for 16bit signed int */
   using std::int16_t;
   /** @brief typedef for singned int 32bit */
   using std::int32_t;
   /** typedef for 64bi signed int */
   using std::int64_t;

   /** @brief typedef for 8bit unsigned int */
   using std::uint8_t;
   /** @brief typedef for 16bit unsigned int */
   using std::uint16_t;
   /** @brief typedef for 32bit unsigned int*/
   using std::uint32_t;
   /** @brief typedef for 64bit unsigned int */
   using std::uint64_t;

   // typedef for 32bit float does not need to be defined -> is float
   // typedef for 64bit float does not need to be defined -> is double
   // signed char 8bit does not need to be defined -> is char

   using std::size_t;
#ifdef TCOS_64BIT
   typedef int64_t ssize_t;
#else
   typedef int32_t ssize_t;
#endif

} // namespac tc

/**
 * @}
 */

#endif // _TCBASE_TYPES_H_
