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
#ifndef _TC_69069_RNG_H_
#define _TC_69069_RNG_H_

#include "TCRng.h"

namespace tc
{
   namespace imp
   {

      /**
      * @addtogroup TC_BASE_IMPL
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of tc::Rng69069
      *
      * @author Thomas Goessler
      */

      /**
      * @brief A 69069 random number generator class
      * Range of the rng: 0 - 0xffffffff
      */
      class Rng69069: public Rng
      {
      public:
         /**
         * @brief Constructor with specified seed
         */
         explicit Rng69069(uint32_t seed);

         virtual uint32_t GetRandomNumber();
         virtual uint32_t GetRandomNumber(uint32_t lower_limit, uint32_t upper_limit);
         virtual double GetRandomNumberDouble(double lower_limit, double upper_limit);
         virtual void SetSeed(uint32_t seed);

      private:
         uint32_t CreateRandomNumber();
         uint32_t m_rng;
      };

      /**
      *  @}
      */
   }
}

#endif // _TC_69069_RNG_H_
