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

#ifndef _TC_NET_BASE_H_
#define _TC_NET_BASE_H_

#include "TCNonCopyable.h"
#include "TCNetTypes.h"

#include <string>

namespace tc
{
namespace net
{
namespace imp
{

   /**
    * @addtogroup TC_NET_IMP
    * @{
    */

   /**
   * TCNet
   * basic class for network communication
   */
   class TC_DLL_LOCAL Base: protected NonCopyAble
   {
   public:
      /**
      * Constructs an TCNet object and initialices sockets
      * @see tc::net::imp::Util::InitSocket()
      */
      Base();
      /**
      * when last net object is deleted cleanup sockets under windows
      */
      virtual ~Base();

    private:
      /**
      * counts the number of currently created net objects
      */
      static int32_t m_num_created_net_objects;
   };

   /**
    * @}
    */

} // namespace imp
} // namespace net
} // namespace tc

#endif // _TC_NET_BASE_H_
