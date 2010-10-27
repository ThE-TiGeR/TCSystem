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
#ifndef _TC_STREAM_BASE_H_
#define _TC_STREAM_BASE_H_

#include "TCCodec.h"
#include "TCStream.h"

namespace TC
{
namespace Impl
{

   /**
    * @addtogroup TC_BASE_IO_IMPL
    * @{
    */

   /** 
    * @file 
    * This header file provides the definition of the class TCStreamBase. 
    * 
    * @author Thomas Goessler 
    */

   /**
    * @brief Base class for all streams
    * implements all operators and prepares methods for reading and writing
    */
   class TC_DLL_LOCAL StreamBase: public Stream
   {
   public:
      /** construct an TCStreamBase object */
      StreamBase(CodecPtr codec);
      /** destruct an TCStreamBase object */
      virtual ~StreamBase();

      sint32 GetStatus() const
      {
         return m_status;
      }
      void ResetStatus();
      bool Error() const
      {
         return GetStatus() != error_none;
      }
      bool IsOk() const 
      {
         return GetStatus() == error_none;
      }
      
      bool GetDisplayErrorMessages() const
      {
         return m_display_error_messages;
      }
      void EnableDisplayErrorMessages(bool displ);

      virtual uint64 Read(sint16 &val) { return m_codec->Decode(*this, val); }
      virtual uint64 Read(uint16 &val) { return m_codec->Decode(*this, val); }
      virtual uint64 Read(sint32 &val) { return m_codec->Decode(*this, val); }
      virtual uint64 Read(uint32 &val) { return m_codec->Decode(*this, val); }
      virtual uint64 Read(sint64 &val) { return m_codec->Decode(*this, val); }
      virtual uint64 Read(uint64 &val) { return m_codec->Decode(*this, val); }
      virtual uint64 Read(float &val)  { return m_codec->Decode(*this, val); }
      virtual uint64 Read(double &val) { return m_codec->Decode(*this, val); }
      virtual uint64 Read(std::string& val){ return m_codec->Decode(*this, val); }
      virtual uint64 Read(std::wstring& val){ return m_codec->Decode(*this, val); }
      virtual uint64 Read(char &val)   { return m_codec->Decode(*this, val); }
      virtual uint64 Read(uchar &val)  { return m_codec->Decode(*this, val); }

      virtual uint64 Write(sint16 val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(uint16 val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(sint32 val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(uint32 val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(sint64 val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(uint64 val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(float val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(double val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(const char *val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(const std::string& val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(const std::wstring& val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(char val) { return m_codec->Encode(val, *this); }
      virtual uint64 Write(uchar val) { return m_codec->Encode(val, *this); }

      virtual uint64 WriteEndOfLine() { return m_codec->EncodeEndOfLine(*this); }
      virtual uint64 WriteSpace() { return m_codec->EncodeSpace(*this); }

      virtual void Flush() {}
      virtual void CloseStream();

   protected:
      /**
       * set the stream error flag
       * @param err errornumber to set the status
       */
      void setStatus(sint32 err) const;
      /** displays the error messege because of the status */
      virtual void displayErrorMessage() const;

      /** @return the stream direction */
      void setStreamDirection(StreamDirection direction) { m_stream_direction = direction; }
      /** @return the stream direction */
      StreamDirection getStreamDirection() const
      {
         return m_stream_direction;
      }
      /** @return true if the stream direction is reading */
      bool isReading() const
      {
         return stream_read == m_stream_direction || isReadingAndWriting();
      }
      /** @return true if the stream direction is reading */
      bool isWriting() const
      {
         return stream_write == m_stream_direction || isReadingAndWriting();
      }
      /** @return true if the stream direction is reading */
      bool isReadingAndWriting() const
      {
        return stream_readwrite == m_stream_direction;
      }

   private:
      /** Pointer to the codec which is used to de/encode the data */
      CodecPtr m_codec;
      /** defines the direction of the stream */
      StreamDirection m_stream_direction;
      /** stores the error of the last operation */
      mutable sint32 m_status;
      /** true if we should write out error messages else false */
      bool m_display_error_messages;
   };

   /**
    * @}
    */

} // namespace Impl
} // namespace TC

#endif // _TC_STREAM_BASE_H_
