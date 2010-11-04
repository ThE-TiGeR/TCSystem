//************************************************************
//
// *******   ***   ***               *
//    *     *     *                  *
//    *    *      *                *****
//    *    *       ***  *   *   **   *    **    ***
//    *    *          *  * *   *     *   ****  * * *
//    *     *         *   *      *   * * *     * * *
//    *      ***   ***    *     **   **   **   *   *
//                        *
// see http://sourceforge.net/projects/tcsystem/ for details.
// Copyright (C) 2003 - 2007 Thomas Goessler (the____tiger@hotmail.com)
//************************************************************
//
// TCSystem is the legal property of its developers.
// Please refer to the COPYRIGHT file distributed with this source distribution.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------
//  $Id$
//----------------------------------------------------------------------
#ifdef HAVE_UNIT_TESTS
#include "TCStreamTest.h"

#include "TCFactory.h"
#include "TCSystem.h"
#include "TCFile.h"

#include <jf/unittest/test_case.h>

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class StreamTest: public jf::unittest::TestCase
      {
      public:
         StreamTest(const std::string& test_name)
            :jf::unittest::TestCase("TC::Tests::" + test_name + "Test")
         {
         }

         virtual void run()
         {
            DoWrite();
            DoRead();
         }

      protected:
         virtual StreamPtr CreateWriteStream() = 0;
         virtual StreamPtr CreateReadStream() = 0;

      private:
         template <class TYPE>
         void WriteValue(StreamPtr stream, TYPE val)
         {
            stream << val;
            JFUNIT_ASSERT(stream->IsOk());
         }

         template <class TYPE>
         void ReadValue(StreamPtr stream, TYPE val_in)
         {
            TYPE val;
            stream >> val;
            JFUNIT_ASSERT(val == val_in);
            JFUNIT_ASSERT(stream->IsOk());
         }

         void DoWrite()
         {
            StreamPtr stream = CreateWriteStream();
            WriteValue<char>(stream, 't');
            WriteValue<uint8>(stream, '8');
            WriteValue<uint16>(stream, 1); stream << space;
            WriteValue<uint32>(stream, 1); stream << endl;
            WriteValue<uint64>(stream, 1); stream << space;

            WriteValue<sint8>(stream, '8');
            WriteValue<sint16>(stream, 1); stream << space;
            WriteValue<sint32>(stream, 1); stream << endl;
            WriteValue<sint64>(stream, 1); stream << space;

            WriteValue<std::string>(stream, "Hallo"); stream << endl;
            WriteValue<std::wstring>(stream, L"Hallo"); stream << endl;
         }

         void DoRead()
         {
            StreamPtr stream = CreateReadStream();
            ReadValue<char>(stream, 't');
            ReadValue<uint8>(stream, '8');
            ReadValue<uint16>(stream, 1);
            ReadValue<uint32>(stream, 1);
            ReadValue<uint64>(stream, 1);

            ReadValue<sint8>(stream, '8');
            ReadValue<sint16>(stream, 1);
            ReadValue<sint32>(stream, 1);
            ReadValue<sint64>(stream, 1);

            ReadValue<std::string>(stream, "Hallo");
            ReadValue<std::wstring>(stream, L"Hallo");
         }
      };

      class AsciiCodec
      {
      public:
         CodecPtr CreateCodec() {return Factory::CreateAsciiCodec();}
      };

      class BinaryCodec
      {
      public:
         CodecPtr CreateCodec() {return Factory::CreateBinaryCodec();}
      };

      template <class CodecType>
      class MemoryTest: public StreamTest, public CodecType
      {
      public:
         MemoryTest(const std::string& name)
            :StreamTest(name + "Memory")
         {
         }

      protected:
         virtual StreamPtr CreateWriteStream()
         {
            return Factory::CreateMemoryStream(CreateCodec(), m_data);
         }

         virtual StreamPtr CreateReadStream()
         {
            return Factory::CreateMemoryStream(CreateCodec(), m_data);
         }

      private:
         ByteVector m_data;
      };

      class StringTest: public StreamTest
      {
      public:
         StringTest()
            :StreamTest("String")
         {
         }

      protected:
         virtual StreamPtr CreateWriteStream()
         {
            return Factory::CreateStringStream(m_data);
         }

         virtual StreamPtr CreateReadStream()
         {
            return Factory::CreateStringStream(m_data);
         }

      private:
         std::string m_data;
      };

      template <class CodecType>
      class FileTest: public StreamTest, public CodecType
      {
      public:
         FileTest(const std::string& name)
            :StreamTest(name + "File")
            ,m_file_name(System::GetTmpFileName())
         {
         }

      protected:
         virtual StreamPtr CreateWriteStream()
         {
            return Factory::CreateFileStream(m_file_name, Stream::stream_write, CreateCodec());
         }

         virtual StreamPtr CreateReadStream()
         {
            return Factory::CreateFileStream(m_file_name, Stream::stream_read, CreateCodec());
         }

         virtual void teardown()
         {
            File::Remove(m_file_name);
         }
      private:
         std::string m_file_name;
      };

      template <class CodecType>
      class GzFileTest: public StreamTest, public CodecType
      {
      public:
         GzFileTest(const std::string& name)
            :StreamTest(name + "GzFile")
            ,m_file_name(System::GetTmpFileName())
         {
         }

      protected:
         virtual StreamPtr CreateWriteStream()
         {
            return Factory::CreateGzFileStream(m_file_name, Stream::stream_write, CreateCodec());
         }

         virtual StreamPtr CreateReadStream()
         {
            return Factory::CreateGzFileStream(m_file_name, Stream::stream_read, CreateCodec());
         }

         virtual void teardown()
         {
            File::Remove(m_file_name);
         }
      private:
         std::string m_file_name;
      };

      template <class CodecType>
      class Bz2FileTest: public StreamTest, public CodecType
      {
      public:
         Bz2FileTest(const std::string& name)
            :StreamTest(name + "Bz2File")
            ,m_file_name(System::GetTmpFileName())
         {
         }

      protected:
         virtual StreamPtr CreateWriteStream()
         {
            return Factory::CreateBz2FileStream(m_file_name, Stream::stream_write, CreateCodec());
         }

         virtual StreamPtr CreateReadStream()
         {
            return Factory::CreateBz2FileStream(m_file_name, Stream::stream_read, CreateCodec());
         }

         virtual void teardown()
         {
            File::Remove(m_file_name);
         }
      private:
         std::string m_file_name;
      };

   }

   namespace Tests
   {
      StreamTestSuite::StreamTestSuite()
         :Unit::TestSuite("TC::Tests::StreamTestSuite")
      {
         add_test(new MemoryTest<BinaryCodec>("Binary"));
         add_test(new MemoryTest<AsciiCodec>("Ascii"));
         add_test(new StringTest);
         add_test(new FileTest<BinaryCodec>("Binary"));
         add_test(new FileTest<AsciiCodec>("Ascii"));
         add_test(new GzFileTest<BinaryCodec>("Binary"));
         add_test(new GzFileTest<AsciiCodec>("Ascii"));
         add_test(new Bz2FileTest<BinaryCodec>("Binary"));
         add_test(new Bz2FileTest<AsciiCodec>("Ascii"));
      }
   }
}

#endif
