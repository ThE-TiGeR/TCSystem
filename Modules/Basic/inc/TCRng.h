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

#ifndef _TC_RNG_H_
#define _TC_RNG_H_

#include "TCSharedPtr.h"

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of tc::Rng
    *
    * @author Thomas Goessler
    */

   /**
    * @brief Provides functions to get/create/set file information
    */
   class Rng
   {
   public:
      /**
      * @brief Get Random true or false value
      */
      virtual uint32 GetRandomNumber() = 0;
      /**
      * @brief Get random uint32 value between lower_limit and upper_limit
      * The lower_limit and upper_limit is included in the set of random numbers
      */
      virtual uint32 GetRandomNumber(uint32 lower_limit, uint32 upper_limit) = 0;

      /**
      * @brief Get random double value between lower_limit and upper_limit
      * The lower_limit and upper_limit is included in the set of random numbers
      */
      virtual double GetRandomNumberDouble(double lower_limit, double upper_limit) = 0;

      /** @brief Set a new seed to the random number generator */
      virtual void SetSeed(uint32 seed) = 0;

      virtual ~Rng() {}
   };

   typedef SharedPtr<Rng> RngPtr;

   /**
    * @}
    */

} // namespace tc

#endif // _TC_RNG_H_
