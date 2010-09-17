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
//  $Id$
//*******************************************************************************
#ifndef _TC_STL_TYPES_H_
#define _TC_STL_TYPES_H_

#include "TCTypes.h"

#include <vector>
#include <set>
#include <string>

namespace TC
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of global data types like for stl containers
    *
    * @author Thomas Goessler
    */

   /** @brief typedef for a byte vector */
   typedef std::vector<uint8> ByteVector;
   /** @brief typedef for a uint16 vector */
   typedef std::vector<uint16> Uint16Vector;
   /** @brief typedef for a uint32 vector */
   typedef std::vector<uint32> Uint32Vector;
   /** @brief typedef for a uint64 vector */
   typedef std::vector<uint64> Uint64Vector;

   /** @brief typedef for a string vector */
   typedef std::vector<std::string> StringVector;
   /** @brief typedef for a string set (strings are sorted */
   typedef std::set<std::string> StringSet;

   /**
   * @}
   */

} // namespac TC

/**
 * @}
 */

#endif // _TCBASE_TYPES_H_
