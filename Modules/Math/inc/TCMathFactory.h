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
//  $Id: TCMathFactory.h,v a33a555a5a40 2012/02/01 22:48:18 Thomas $
//*******************************************************************************

#ifndef _TC_MATH_FACTORY_H_
#define _TC_MATH_FACTORY_H_

#include "TCMathApi.h"
#include "TCMathChecksum.h"

#include <string>

namespace tc
{
   namespace math
   {
      /**
      * @addtogroup TC_MATH
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of tc::math::factory
      * @author Thomas Goessler
      */

      /**
      * @brief factory for creating tc::math objects
      */
      namespace factory
      {
         /**
         * @brief Creates an MD5 Checksum calculation object
         */
         TCMATH_API ChecksumPtr CreateMD5Checksum();
      }

      /**
      * @}
      */

   } // namespace math
} // namespace tc

#endif //_TC_MATH_FACTORY_H_
