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
// Copyright (C) 2003 - 2014 Thomas Goessler. All Rights Reserved. 
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

      int32_t GetStatus() const
      {
         return m_status;
      }
      void ResetStatus();
      bool Error() const
      {
         return GetStatus() != ERROR_NONE;
      }
      bool IsOk() const 
      {
         return GetStatus() == ERROR_NONE;
      }
      
      bool GetDisplayErrorMessages() const
      {
         return m_display_error_messages;
      }
      void EnableDisplayErrorMessages(bool displ);

      virtual uint64_t Read(int8_t &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(uint8_t &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(int16_t &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(uint16_t &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(int32_t &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(uint32_t &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(int64_t &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(uint64_t &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(float &val)  { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(double &val) { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(std::string& val){ return m_codec->Decode(*this, val); }
      virtual uint64_t Read(std::wstring& val){ return m_codec->Decode(*this, val); }
      virtual uint64_t Read(char &val)   { return m_codec->Decode(*this, val); }
      virtual uint64_t Read(bool &val)  { return m_codec->Decode(*this, val); }

      virtual uint64_t Write(int8_t val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(uint8_t val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(int16_t val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(uint16_t val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(int32_t val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(uint32_t val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(int64_t val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(uint64_t val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(float val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(double val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(const char *val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(const std::string& val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(const std::wstring& val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(char val) { return m_codec->Encode(val, *this); }
      virtual uint64_t Write(bool val) { return m_codec->Encode(val, *this); }

      virtual uint64_t WriteEndOfLine() { return m_codec->EncodeEndOfLine(*this); }
      virtual uint64_t WriteSpace() { return m_codec->EncodeSpace(*this); }

      virtual void Flush() {}
      virtual void CloseStream();

   protected:
      /**
       * set the stream error flag
       * @param err error number to set the status
       */
      void SetStatus(int32_t err) const;
      /** displays the error message because of the status */
      virtual void DisplayErrorMessage() const;

      /** @return the stream direction */
      void SetStreamDirection(StreamDirection direction) { m_stream_direction = direction; }
      /** @return the stream direction */
      StreamDirection GetStreamDirection() const
      {
         return m_stream_direction;
      }
      /** @return true if the stream direction is reading */
      bool IsReading() const
      {
         return STREAM_READ == m_stream_direction || IsReadingAndWriting();
      }
      /** @return true if the stream direction is reading */
      bool IsWriting() const
      {
         return STREAM_WRITE == m_stream_direction || IsReadingAndWriting();
      }
      /** @return true if the stream direction is reading */
      bool IsReadingAndWriting() const
      {
        return STREAM_READ_WRITE == m_stream_direction;
      }
      /** @return the pointer to the codec */
      CodecPtr GetCodec()
      {
         return m_codec;
      }
   private:
      /** Pointer to the codec which is used to de/encode the data */
      CodecPtr m_codec;
      /** defines the direction of the stream */
      StreamDirection m_stream_direction;
      /** stores the error of the last operation */
      mutable int32_t m_status;
      /** true if we should write out error messages else false */
      bool m_display_error_messages;
   };

   /**
    * @}
    */

} // namespace imp
} // namespace tc

#endif // _TC_STREAM_BASE_H_
