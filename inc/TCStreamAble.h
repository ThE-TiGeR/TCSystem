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

#ifndef _TC_STREAM_ABLE_H_
#define _TC_STREAM_ABLE_H_

#include "TCStream.h"

namespace TC
{
   /**
    * @addtogroup TC_BASE_IO
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TC::StreamAble
    *
    * @author Thomas Goessler
    */

   /**
    * @brief Interface class define functions for streaming a object to a Stream
    */
   class StreamAble
   {
   public:
      /**
       * @brief Read the object data from a Stream
       * @param stream Stream where to read the data from
       * @return success or not
       */
      virtual bool ReadFromStream(StreamPtr stream) = 0;
      /**
       * @brief Write the object data to a Stream
       * @param stream Stream where to write the data to
       * @return success or not
       */
      virtual bool WriteOnStream(StreamPtr stream) const = 0;

      virtual ~StreamAble() {}
   };

   /**
    * @brief Read operator for reading a StreamAble object reference from a Stream
    *
    * @param stream Stream where to read the data from
    * @param object The object on which to read the data
    * @return the stream
    */
   TCBASE_API StreamPtr operator>>(StreamPtr stream, StreamAble &object);

   /**
    * @brief Write operator for writing a StreamAble object reference to a Stream
    *
    * @param stream Stream where to write the data to
    * @param object The object of which to write the data
    * @return the stream
    */
   TCBASE_API StreamPtr operator<<(StreamPtr stream, const StreamAble &object);

   /**
    * @}
    */

} // namspace TC

#endif // _TC_STREAM_ABLE_H_
