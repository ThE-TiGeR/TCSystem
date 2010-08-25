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
//  $Id: TCInterlocked.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#ifndef _TC_INTERLOCKED_H_
#define _TC_INTERLOCKED_H_

#include "TCTypes.h"

namespace TC
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * Defines the namespace TC::Interlocked
    *
    * @author Thomas Goessler
    */

   /**
    * Methods for interlocked modifying of integer values
    * @author Thomas Goessler
    */
   namespace Interlocked
   {
      /** Data type for interlockee operations */
      typedef long Type;

      /**
       * @brief The function increments (increases by one) the value of the specified variable
       *
       * The function prevents more than one thread from using the same variable simultaneously.
       * @param target [in, out] reference of the variable to be incremented.
       * @return The return value is the resulting incremented value.
       */
      TCBASE_API Type Increment(Type& target);

      /**
       * @brief The function decrements (decreases by one) the value of the specified variable.
       *
       * The function prevents more than one thread from using the same variable simultaneously.
       * @param target [in, out] reference of the variable to be decremented.
       * @return The return value is the resulting decremented value.
       */
      TCBASE_API Type Decrement(Type& target);

      /**
       * @brief The function atomically exchanges a pair of values.
       *
       * The function prevents more than one thread from using the same variable simultaneously.
       * @param target [in, out] reference to the value to be exchanged.
       *               The function sets this variable to new_value, and returns its prior value.
       * @param new_value [in] Value to be exchanged with the value pointed to by Target.
       * @return The return value is the prior target value value.
       */
      TCBASE_API Type Exchange(Type& target, sint32 new_value);

      /**
       * @brief The function performs an atomic addition of an increment value to an addend variable.
       *
       * The function prevents more than one thread from using the same variable simultaneously.
       * @param target [in, out] Pointer to the value that is to have Value added to it.
       * @param value_to_add [in] Value to be added to the variable pointed to by the Addend parameter.
       * @return The return value is the initial value of the variable pointed to by the Addend parameter.
       */
      TCBASE_API Type Add(Type& target, sint32 value_to_add);

   } // namespace Interlocked

   /**
    *  @}
    */

} // namespace TC

#endif // _TC_INTERLOCKED_H_
