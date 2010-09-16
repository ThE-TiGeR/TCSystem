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
#ifndef _TCSTREAM_H_
#define _TCSTREAM_H_

#include "TCNonCopyable.h"
#include "TCSharedPtr.h"

#include <string>
#include <vector>

namespace TC
{
   /**
    * @addtogroup TC_BASE_IO
    * @{
    */

   /** 
    * @file 
    * This header file provides the definition of the class TC::Stream. 
    * 
    * @author Thomas Goessler 
    */

   /**
    * @brief Base class for all streams
    * implements all operators and prepares methods for reading and writing
    */
   class Stream: protected NonCopyAble
   {
   public:
      /**
       * Enumeration which defines the TCStream error flags
       */
      enum StreamErrorFlags
      {
         /** no error happened */
         error_none,
         /** Error openening the stream */
         error_streamopen,
         /** Error closing the stream */
         error_streamclose,
         /** Using wrong stream direction */
         error_streamdirection,
         /** Last error flag */
         error_last
      };

      /**
       * enumeration which defines different stream directions
       */
      enum StreamDirection
      {
         /** stream is not ready for read or write (not open or allready closed */
         stream_dead,
         /** stream is used for reading */
         stream_read,
         /** stream is used for writing */
         stream_write,
         /** stream is used for reading and writing*/
         stream_readwrite
      };

   public:
      virtual ~Stream() {}

      /**
       * @return the error flag
       * defined in TCStream::StreamErrorFlags
       */
      virtual sint32 GetStatus() const = 0;
      /** Reset the error flag to error_none */
      virtual void ResetStatus() = 0;
      /** @return False if no error happend else true */
      virtual bool Error() const = 0;
      /** @return True if no error happend else false */
      virtual bool IsOk() const = 0;

      /**
       * Tell the stream if he should write out error messages or not
       * @param displ true or false fro displaying error messages
       */
      virtual void EnableDisplayErrorMessages(bool displ) = 0;

      enum StreamPosition
      {
         POSITION_SET,
         POSITION_CURRENT,
         POSITION_END,
      };
      virtual bool SetPosition(sint64, StreamPosition pos) = 0;
      virtual uint64 GetPosition() const = 0;

      /**
       * Methode for reading the bytes from the stream
       * has to be implemented in each derived class
       *
       * @param nBytes number of bytes to read
       * @param bytes where to store the data which should be read
       */
      virtual uint64 ReadBytes (uint64 nBytes, void *bytes) = 0;
      /**
       * Methode for writing the bytes to the stream
       * has to be implemented in each derived class
       *
       * @param nBytes number of bytes to written
       * @param bytes where the bytes are stored which should be written
       */
      virtual uint64 WriteBytes(uint64 nBytes, const void *bytes) = 0;

      /**
       * Methode for reading one signed 16bit integer from the stream
       * @param val the short which should store the short
       */
      virtual uint64 Read(sint16 &val) = 0;
      /**
       * Methode for reading one unsigned 16bit integer from the stream
       * @param val the short which should store the short
       */
      virtual uint64 Read(uint16 &val) = 0;
      /**
       * Methode for reading one signed int 32bit integer the stream
       * @param val the int which should store the int
       */
      virtual uint64 Read(sint32 &val) = 0;
      /**
       * Methode for reading one unsigned 32bit integer from the stream
       * @param val the int which should store the int
       */
      virtual uint64 Read(uint32 &val) = 0;
      /**
       * Methode for reading one signed 64bit integer from the stream
       * @param val the long which should store the long
       */
      virtual uint64 Read(sint64 &val) = 0;
      /**
       * Methode for reading one unsigned 64bit integer from the stream
       * @param val the long which should store the long
       */
      virtual uint64 Read(uint64 &val) = 0;
      /**
       * Methode for reading one float from the stream
       * @param val the float which should store the float
       */
      virtual uint64 Read(float &val) = 0;
      /**
       * Methode for reading one double from the stream
       * @param val the double which should store the double
       */
      virtual uint64 Read(double &val) = 0;
      /**
       * Methode for reading one string from the stream
       * @param val the string which should store the string
       */
      virtual uint64 Read(char *val) = 0;
      /**
      * Methode for reading one std::string from the stream
      * @param val the string which should store the string
      */
      virtual uint64 Read(std::string& val) = 0;
      /**
      * Methode for reading one std::wstring from the stream
      * @param val the wstring which should store the string
      */
      virtual uint64 Read(std::wstring& val) = 0;
      /**
      * Methode for reading one char from the stream
      * @param val the char which should store the char
      */
      virtual uint64 Read(char &val) = 0;
      /**
      * Methode for reading one unsigned char from the stream
      * @param val the char which should store the char
      */
      virtual uint64 Read(uchar &val) = 0;


      /**
      * Methode for writing one signed 16bit integer to the stream
      * @param val the short which to write
      */
      virtual uint64 Write(sint16 val) = 0;
      /**
      * Methode for writing one unsigned 16bit integer to the stream
      * @param val the short which to write
      */
      virtual uint64 Write(uint16 val) = 0;
      /**
      * Methode for writing one signed 32bit integer to the stream
      * @param val the int which to write
      */
      virtual uint64 Write(sint32 val) = 0;
      /**
      * Methode for writing one unsigned 32bit integer to the stream
      * @param val the int which to write
      */
      virtual uint64 Write(uint32 val) = 0;
      /**
      * Methode for writing one signed 64bit integer to the stream
      * @param val the long which to write
      */
      virtual uint64 Write(sint64 val) = 0;
      /**
      * Methode for writing one unsigned 64bit integer to the stream
      * @param val the long which to write
      */
      virtual uint64 Write(uint64 val) = 0;
      /**
      * Methode for writing one float to the stream
      * @param val the float which to write
      */
      virtual uint64 Write(float val) = 0;
      /**
      * Methode for writing one double to the stream
      * @param val the double which to write
      */
      virtual uint64 Write(double val) = 0;
      /**
      * Methode for writing one string to the stream
      * @param val the string which to write
      */
      virtual uint64 Write(const char *val) = 0;
      /**
      * Methode for writing one std::string to the stream
      * @param val the string which to write
      */
      virtual uint64 Write(const std::string& val) = 0;
      /**
      * Methode for writing one std::wstring to the stream
      * @param val the wstring which to write
      */
      virtual uint64 Write(const std::wstring& val) = 0;
      /**
      * Methode for writing one signed char to the stream
      * @param val the char which to write
      */
      virtual uint64 Write(char val) = 0;
      /**
      * Methode for writing one unsigned char to the stream
      * @param val the char which to write
      */
      virtual uint64 Write(uchar val) = 0;

      /** Writes the end of line characters if needed for the stream */
      virtual uint64 WriteEndOfLine() = 0;
      /**
      * Writes space to the stream if needed for the stream
      * needed for separating words or numbers
      */
      virtual uint64 WriteSpace() = 0;

      /** flushes the stream if needed */
      virtual void Flush() = 0;
      /** Closes the stream and sets the streamdirection to stream_dead */
      virtual void CloseStream() = 0;
   };

   /** @brief Shared pointer of an stream object */
   typedef SharedPtr<Stream> StreamPtr;

   // -----------------------------------------------------------------
   // read operators
   // -----------------------------------------------------------------
   inline StreamPtr operator>>(StreamPtr stream, std::string& val) { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, std::wstring& val) { stream->Read(val); return stream; }

   inline StreamPtr operator>>(StreamPtr stream, uchar& val) { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, char& val)  { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, char* val)  { stream->Read(val); return stream; }

   inline StreamPtr operator>>(StreamPtr stream, sint16& val) { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, sint32& val) { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, sint64& val) { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, uint16& val) { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, uint32& val) { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, uint64& val) { stream->Read(val); return stream; }

   inline StreamPtr operator>>(StreamPtr stream, float& val)  { stream->Read(val); return stream; }
   inline StreamPtr operator>>(StreamPtr stream, double& val) { stream->Read(val); return stream; }

   // -----------------------------------------------------------------
   // write operators
   // -----------------------------------------------------------------
   inline StreamPtr operator<<(StreamPtr stream, StreamPtr (*_Pfn)(StreamPtr)) {return ((*_Pfn)(stream));}

   inline StreamPtr operator<<(StreamPtr stream, const std::string& val) { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, const std::wstring& val) { stream->Write(val); return stream; }

   inline StreamPtr operator<<(StreamPtr stream, uchar val)       { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, char val)        { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, const char* val) { stream->Write(val); return stream; }

   inline StreamPtr operator<<(StreamPtr stream, sint16 val)      { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, sint32 val)      { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, sint64 val)      { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, uint16 val)      { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, uint32 val)      { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, uint64 val)      { stream->Write(val); return stream; }

   inline StreamPtr operator<<(StreamPtr stream, float val)       { stream->Write(val); return stream; }
   inline StreamPtr operator<<(StreamPtr stream, double val)      { stream->Write(val); return stream; }

   template <class T>
   StreamPtr operator<<(StreamPtr stream, const std::vector<T>& values)
   {
       stream << static_cast<uint32>(values.size());
       for (std::vector<T>::const_iterator it=values.begin(); it!=values.end(); ++it)
       {
           stream << *it;
       }
       return stream;
   }

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

   /**
    * @}
    */

} // namespace TC

#endif // _TCSTREAM_H_
