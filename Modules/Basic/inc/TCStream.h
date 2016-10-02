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
//  $Id: TCStream.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************
#ifndef _TCSTREAM_H_
#define _TCSTREAM_H_

#include "TCSharedPtr.h"

#include <string>
#include <vector>

namespace tc
{
   /**
    * @addtogroup TC_BASE_IO
    * @{
    */

   /** 
    * @file 
    * This header file provides the definition of the class tc::Stream. 
    * 
    * @author Thomas Goessler 
    */

   class Stream;
   /** @brief Shared pointer of an stream object */
   typedef SharedPtr<Stream> StreamPtr;

   /**
    * @brief Base class for all streams
    * implements all operators and prepares methods for reading and writing
    */
   class Stream
   {
   public:
      /**
       * Enumeration which defines the TCStream error flags
       */
	  enum class Error
      {
         /** no error happened */
         NONE,
         /** Error opening the stream */
         STREAM_OPEN,
         /** Error closing the stream */
         STREAM_CLOSE,
         /** Using wrong stream direction */
         STREAM_DIRECTION,
         /** end of stream reached */
         END_OF_STREAM,
         /** error during reading of stream */
         READ_FROM_STREAM,
         /** error during writing to stream */
         WRITE_TO_STREAM,
         /** error setting stream position */
         SET_STREAM_POSITION
      };

      /**
       * enumeration which defines different stream directions
       */
      enum class Direction
      {
         /** stream is not ready for read or write (not open or already closed */
         DEAD,
         /** stream is used for reading */
         READ,
         /** stream is used for writing */
         WRITE,
         /** stream is used for reading and writing*/
         READ_WRITE
      };

      enum class Position
      {
         SET,
         CURRENT,
         END,
      };

   public:
      /** Clone this stream object */
      virtual StreamPtr Clone() = 0;

      /**
       * @return the error flag
       * defined in TCStream::StreamErrorFlags
       */
	  virtual Error GetStatus() const = 0;
      /** Reset the error flag to ERROR_NONE */
      virtual void ResetStatus() = 0;
      /** @return False if no error happend else true */
      virtual bool HasError() const = 0;
      /** @return True if no error happend else false */
      virtual bool IsOk() const = 0;

      /**
       * Tell the stream if he should write out error messages or not
       * @param displ true or false fro displaying error messages
       */
      virtual void EnableDisplayErrorMessages(bool displ) = 0;

      /** Set the current position of the stream where to read or write */
      virtual bool SetPosition(int64_t, Position pos) = 0;
      /** Get the current position of the stream */
      virtual uint64_t GetPosition() const = 0;

      /**
       * Method for reading the bytes from the stream
       * has to be implemented in each derived class
       *
       * @param nBytes number of bytes to read
       * @param bytes where to store the data which should be read
       */
      virtual uint64_t ReadBytes (uint64_t nBytes, void *bytes) = 0;
      /**
       * Method for writing the bytes to the stream
       * has to be implemented in each derived class
       *
       * @param nBytes number of bytes to written
       * @param bytes where the bytes are stored which should be written
       */
      virtual uint64_t WriteBytes(uint64_t nBytes, const void *bytes) = 0;

      /**
       * Method for reading one signed 16bit integer from the stream
       * @param val the short which should store the short
       */
      virtual uint64_t Read(int8_t &val) = 0;
      /**
       * Method for reading one unsigned 16bit integer from the stream
       * @param val the short which should store the short
       */
      virtual uint64_t Read(uint8_t &val) = 0;
      /**
       * Method for reading one signed 16bit integer from the stream
       * @param val the short which should store the short
       */
      virtual uint64_t Read(int16_t &val) = 0;
      /**
       * Method for reading one unsigned 16bit integer from the stream
       * @param val the short which should store the short
       */
      virtual uint64_t Read(uint16_t &val) = 0;
      /**
       * Method for reading one signed int 32bit integer the stream
       * @param val the int which should store the int
       */
      virtual uint64_t Read(int32_t &val) = 0;
      /**
       * Method for reading one unsigned 32bit integer from the stream
       * @param val the int which should store the int
       */
      virtual uint64_t Read(uint32_t &val) = 0;
      /**
       * Method for reading one signed 64bit integer from the stream
       * @param val the long which should store the long
       */
      virtual uint64_t Read(int64_t &val) = 0;
      /**
       * Method for reading one unsigned 64bit integer from the stream
       * @param val the long which should store the long
       */
      virtual uint64_t Read(uint64_t &val) = 0;
      /**
       * Method for reading one float from the stream
       * @param val the float which should store the float
       */
      virtual uint64_t Read(float &val) = 0;
      /**
       * Method for reading one double from the stream
       * @param val the double which should store the double
       */
      virtual uint64_t Read(double &val) = 0;
      /**
      * Method for reading one std::string from the stream
      * @param val the string which should store the string
      */
      virtual uint64_t Read(std::string& val) = 0;
      /**
      * Method for reading one std::wstring from the stream
      * @param val the wstring which should store the string
      */
      virtual uint64_t Read(std::wstring& val) = 0;
      /**
      * Method for reading one char from the stream
      * @param val the char which should store the char
      */
      virtual uint64_t Read(char &val) = 0;
      /**
      * Method for reading one bool from the stream
      * @param val the char which should store the bool
      */
      virtual uint64_t Read(bool &val) = 0;


      /**
      * Method for writing one signed 8bit integer to the stream
      * @param val the short which to write
      */
      virtual uint64_t Write(int8_t val) = 0;
      /**
      * Method for writing one unsigned 8bit integer to the stream
      * @param val the short which to write
      */
      virtual uint64_t Write(uint8_t val) = 0;
      /**
      * Method for writing one signed 16bit integer to the stream
      * @param val the short which to write
      */
      virtual uint64_t Write(int16_t val) = 0;
      /**
      * Method for writing one unsigned 16bit integer to the stream
      * @param val the short which to write
      */
      virtual uint64_t Write(uint16_t val) = 0;
      /**
      * Method for writing one signed 32bit integer to the stream
      * @param val the int which to write
      */
      virtual uint64_t Write(int32_t val) = 0;
      /**
      * Method for writing one unsigned 32bit integer to the stream
      * @param val the int which to write
      */
      virtual uint64_t Write(uint32_t val) = 0;
      /**
      * Method for writing one signed 64bit integer to the stream
      * @param val the long which to write
      */
      virtual uint64_t Write(int64_t val) = 0;
      /**
      * Method for writing one unsigned 64bit integer to the stream
      * @param val the long which to write
      */
      virtual uint64_t Write(uint64_t val) = 0;
      /**
      * Method for writing one float to the stream
      * @param val the float which to write
      */
      virtual uint64_t Write(float val) = 0;
      /**
      * Method for writing one double to the stream
      * @param val the double which to write
      */
      virtual uint64_t Write(double val) = 0;
      /**
      * Method for writing one string to the stream
      * @param val the string which to write
      */
      virtual uint64_t Write(const char *val) = 0;
      /**
      * Method for writing one std::string to the stream
      * @param val the string which to write
      */
      virtual uint64_t Write(const std::string& val) = 0;
      /**
      * Method for writing one std::wstring to the stream
      * @param val the wstring which to write
      */
      virtual uint64_t Write(const std::wstring& val) = 0;
      /**
      * Method for writing one signed char to the stream
      * @param val the char which to write
      */
      virtual uint64_t Write(char val) = 0;
      /**
      * Method for writing one bool to the stream
      * @param val the bool which to write
      */
      virtual uint64_t Write(bool val) = 0;

      /** Writes the end of line characters if needed for the stream */
      virtual uint64_t WriteEndOfLine() = 0;
      /**
      * Writes space to the stream if needed for the stream
      * needed for separating words or numbers
      */
      virtual uint64_t WriteSpace() = 0;

      /** flushes the stream if needed */
      virtual void Flush() = 0;
      /** Closes the stream and sets the streamdirection to StreamDirection::DEAD */
      virtual void CloseStream() = 0;

      virtual ~Stream() {}
   };

   /**
    * Flush the stream
    * @param stream The stream which to flush
    * @return The stream
    */
   inline StreamPtr flush(StreamPtr stream) { stream->Flush(); return stream; }
   /**
    * Write end of line to the stream
    * @param stream Where to write end of line
    * @return The stream
    */
   inline StreamPtr endl(StreamPtr stream) { stream->WriteEndOfLine(); return stream; }
   /**
    * Write a word or number separator to the stream
    * @param stream Where to write the seperator
    * @return The stream
    */
   inline StreamPtr space(StreamPtr stream) { stream->WriteSpace(); return stream; }

   // -----------------------------------------------------------------
   // read operators
   // -----------------------------------------------------------------
   template <class T>
   inline StreamPtr operator>>(StreamPtr stream, T& val) { stream->Read(val); return stream; }

   template <class T>
   inline StreamPtr operator>>(StreamPtr stream, std::vector<T>& values)
   {
       uint64_t size;
       stream >> size;
       values.resize(typename std::vector<T>::size_type(size));
       for (auto& val : values)
       {
           stream >> val;
       }
       return stream;
   }

   // -----------------------------------------------------------------
   // write operators
   // -----------------------------------------------------------------
   inline StreamPtr operator<<(StreamPtr stream, StreamPtr (*_Pfn)(StreamPtr)) {return ((*_Pfn)(stream));}

   template <class T>
   inline StreamPtr operator<<(StreamPtr stream, const T& val) { stream->Write(val); return stream; }

   template <class T>
   inline StreamPtr operator<<(StreamPtr stream, const std::vector<T>& values)
   {
       stream << static_cast<uint64_t>(values.size()) << space;
       for (const auto& val : values)
       {
           stream << val << space;
       }
       return stream;
   }

   /**
    * @}
    */

} // namespace tc

#endif // _TCSTREAM_H_
