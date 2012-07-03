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

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace
   {
      class StreamTest: public unit::TestCase
      {
      public:
         StreamTest(const std::string& test_name)
            :unit::TestCase("tc::tests::" + test_name + "Test")
         {
         }

         virtual void Execute()
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
            TCUNIT_ASSERT(stream->IsOk());
         }

         template <class TYPE>
         void ReadValue(StreamPtr stream, TYPE val_in)
         {
            TYPE val;
            stream >> val;
            TCUNIT_ASSERT(val == val_in);
            TCUNIT_ASSERT(stream->IsOk());
         }

         void DoWrite()
         {
            StreamPtr stream = CreateWriteStream();
            WriteValue<char>(stream, 't'); stream << space;
            WriteValue<uint8_t>(stream, uint8_t(8)); stream << space;
            WriteValue<uint16_t>(stream, 1); stream << space;
            WriteValue<uint32_t>(stream, 1); stream << endl;
            WriteValue<uint64_t>(stream, 1); stream << space;

            WriteValue<int8_t>(stream, int8_t(8)); stream << space;
            WriteValue<int16_t>(stream, 1); stream << space;
            WriteValue<int32_t>(stream, 1); stream << endl;
            WriteValue<int64_t>(stream, 1); stream << space;

            WriteValue<bool>(stream, true); stream << space;
            WriteValue<bool>(stream, false); stream << space;

            WriteValue<std::string>(stream, "Hallo"); stream << endl;
            WriteValue<std::wstring>(stream, L"Hallo"); stream << endl;
         }

         void DoRead()
         {
            StreamPtr stream = CreateReadStream();
            ReadValue<char>(stream, 't');
            ReadValue<uint8_t>(stream, uint8_t(8));
            ReadValue<uint16_t>(stream, 1);
            ReadValue<uint32_t>(stream, 1);
            ReadValue<uint64_t>(stream, 1);

            ReadValue<int8_t>(stream, int8_t(8));
            ReadValue<int16_t>(stream, 1);
            ReadValue<int32_t>(stream, 1);
            ReadValue<int64_t>(stream, 1);
            ReadValue<bool>(stream, true);
            ReadValue<bool>(stream, false);

            ReadValue<std::string>(stream, "Hallo");
            ReadValue<std::wstring>(stream, L"Hallo");
         }
      };

      class AsciiCodec
      {
      public:
         CodecPtr CreateCodec() {return factory::CreateAsciiCodec();}
      };

      class BinaryCodec
      {
      public:
         CodecPtr CreateCodec() {return factory::CreateBinaryCodec();}
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
            return factory::CreateMemoryStream(this->CreateCodec(), m_data);
         }

         virtual StreamPtr CreateReadStream()
         {
            return factory::CreateMemoryStream(this->CreateCodec(), m_data);
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
            return factory::CreateStringStream(m_data);
         }

         virtual StreamPtr CreateReadStream()
         {
            return factory::CreateStringStream(m_data);
         }

      private:
         std::string m_data;
      };

      template <class CodecType>
      class FileTest: public StreamTest, public CodecType
      {
      public:
         FileTest(const std::string& name)
            :StreamTest(name + "file")
            ,m_file_name(system::GetTmpFileName())
         {
         }

      protected:
         virtual StreamPtr CreateWriteStream()
         {
            return factory::CreateFileStream(m_file_name, Stream::stream_write, this->CreateCodec());
         }

         virtual StreamPtr CreateReadStream()
         {
            return factory::CreateFileStream(m_file_name, Stream::stream_read, this->CreateCodec());
         }

         virtual void teardown()
         {
            file::Remove(m_file_name);
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
            ,m_file_name(system::GetTmpFileName())
         {
         }

      protected:
         virtual StreamPtr CreateWriteStream()
         {
            return factory::CreateGzFileStream(m_file_name, Stream::stream_write, this->CreateCodec());
         }

         virtual StreamPtr CreateReadStream()
         {
            return factory::CreateGzFileStream(m_file_name, Stream::stream_read, this->CreateCodec());
         }

         virtual void teardown()
         {
            file::Remove(m_file_name);
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
            ,m_file_name(system::GetTmpFileName())
         {
         }

      protected:
         virtual StreamPtr CreateWriteStream()
         {
            return factory::CreateBz2FileStream(m_file_name, Stream::stream_write, this->CreateCodec());
         }

         virtual StreamPtr CreateReadStream()
         {
            return factory::CreateBz2FileStream(m_file_name, Stream::stream_read, this->CreateCodec());
         }

         virtual void teardown()
         {
            file::Remove(m_file_name);
         }
      private:
         std::string m_file_name;
      };

   }

   namespace tests
   {
      StreamTestSuite::StreamTestSuite()
         :unit::TestSuite("tc::tests::StreamTestSuite")
      {
         AddTest(unit::Test::Ptr(new MemoryTest<BinaryCodec>("Binary")));
         AddTest(unit::Test::Ptr(new MemoryTest<AsciiCodec>("Ascii")));
         AddTest(unit::Test::Ptr(new StringTest));
         AddTest(unit::Test::Ptr(new FileTest<BinaryCodec>("Binary")));
         AddTest(unit::Test::Ptr(new FileTest<AsciiCodec>("Ascii")));
         AddTest(unit::Test::Ptr(new GzFileTest<BinaryCodec>("Binary")));
         AddTest(unit::Test::Ptr(new GzFileTest<AsciiCodec>("Ascii")));
         AddTest(unit::Test::Ptr(new Bz2FileTest<BinaryCodec>("Binary")));
         AddTest(unit::Test::Ptr(new Bz2FileTest<AsciiCodec>("Ascii")));
      }
   }
}

#endif
