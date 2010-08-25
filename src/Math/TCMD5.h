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
//  $Id: TCMD5.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TC_MD5_H_
#define _TC_MD5_H_

#include "TCNonCopyable.h"
#include "TCMathChecksum.h"

#include <string>

namespace TC
{
namespace Math
{
namespace Impl
{
   /**
    * @addtogroup TC_MATH_IMPL
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TC::Math::Impl::MD5
    * @author Thomas Goessler
    */

   /**
   * @brief Class handles calculation of an md5 checksum
   * With the method Append data can be added to the checksum calculation
   */
   class TC_DLL_LOCAL MD5: public Checksum
   {
   public:
      MD5();
      ~MD5();

      /**
       * @short Append data for to the hash value calculation
       * @param data The data to append to the calculation
       * @param len The length of the data to append
       */
      virtual void Append(const uchar *data, uint32 len);
      
      /** @short Get the calculated hash string */
      virtual std::string GetHashString() const;

      /** @short Get the calculated hash digets */
      virtual std::string GetHashDigets() const;

   private:
      void Process(const uchar data[64]);
      struct MD5Context* md5_context;
   };

   /**
    * @}
    */

} // namespace Impl
} // namespace Math
} // namespace TC

#endif //_TC_MD5_H_
