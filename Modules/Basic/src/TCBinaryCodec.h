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
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCBinaryCodec.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifndef _TCBINARY_CODEC_H_
#define _TCBINARY_CODEC_H_

#include "TCCodec.h"

namespace tc
{
namespace imp
{
   /**
    * @addtogroup TC_BASE_IO_IMPL
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of 
    *        tc::imp::CreateBinaryCodec
    *        tc::imp::CreateBinaryCodec
    *        tc::imp::CreateBinaryCodec
    *
    * @author Thomas Goessler
    */

   TC_DLL_LOCAL CodecPtr CreateBinaryCodec();
   TC_DLL_LOCAL CodecPtr CreateLittleEndianBinaryCodec();
   TC_DLL_LOCAL CodecPtr CreateBigEndianBinaryCodec();

   /**
    * @}
    */
}
}

#endif // _TCBINARY_CODEC_H_
