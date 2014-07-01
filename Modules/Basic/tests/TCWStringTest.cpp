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
// see https://bitbucket.org/the____tiger/tcsystem for details.
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

#include "TCWStringTest.h"

#include "TCWString.h"
#include "TCUtil.h"

#include "TCUnitTestCase.h"
#include <string>
#include <cmath>

#include "TCNewEnable.h"

#ifdef _MSC_VER
#define snprintf ::_snprintf
#define snwprintf ::_snwprintf
#elif defined TCOS_ANDROID
#   define snwprintf ::swprintf
#   define vsnwprintf  ::vswprintf
#elif defined TCOS_LINUX
#   define snwprintf std::swprintf
#   define vsnwprintf  std::vswprintf
#endif


namespace tc
{
   namespace
   {
      class ConvertToUpperLower: public unit::TestCase
      {
      public:
         ConvertToUpperLower() : unit::TestCase("tc::tests::ConvertToUpperLower") {}
         virtual void Execute();
      };

      void ConvertToUpperLower::Execute()
      {
         std::wstring text;

         // test conversion to upper case
         text = L"ConvertToUpper";
         text = wstring::ToUpper(text);
         TCUNIT_ASSERT(text == L"CONVERTTOUPPER");

         // test conversion to lower case
         text = wstring::ToLower(text);
         TCUNIT_ASSERT(text == L"converttoupper");

         text = L"ConvertToLower";
         text = wstring::ToLower(text);
         TCUNIT_ASSERT(text == L"converttolower");
      }

      class ConvertToNumber: public unit::TestCase
      {
      public:
         ConvertToNumber() : unit::TestCase("tc::tests::ConvertToNumber") {}
         virtual void Execute();
      };

      void ConvertToNumber::Execute()
      {
         // test conversion to uint32_t
         TCUNIT_ASSERT(wstring::ToUint32(L"123456.789") == 123456);
         TCUNIT_ASSERT(wstring::ToUint32(L"0")          == 0);               // min value
         TCUNIT_ASSERT(wstring::ToUint32(L"4294967295") == 4294967295ul);      // max value
         TCUNIT_ASSERT(wstring::ToUint32(L"-1")         == 4294967295ul);

         // test conversion to int32_t
         TCUNIT_ASSERT(wstring::ToSint32(L"123456.789") == 123456);
         TCUNIT_ASSERT(wstring::ToSint32(L"0") == 0);
         TCUNIT_ASSERT(wstring::ToSint32(L"-123") == -123);
         TCUNIT_ASSERT(wstring::ToSint32(L"-2147483647") == -2147483647);    // min value
         TCUNIT_ASSERT(wstring::ToSint32(L"2147483647") == 2147483647);      // max value

         TCUNIT_ASSERT(fabs(wstring::ToFlt32(L"123456.789") - 123456.789f) < 0.1f);
         TCUNIT_ASSERT(wstring::ToFlt64(L"123456.789") == 123456.789);
      }

      class ConvertToString: public unit::TestCase
      {
      public:
         ConvertToString() : unit::TestCase("tc::tests::ConvertToString") {}
         virtual void Execute();
      };

      void ConvertToString::Execute()
      {
         // convert from ascii to wide character
         {
            TCUNIT_ASSERT(wstring::ToString("testing ToString( const char* string )")
               == L"testing ToString( const char* string )");
            TCUNIT_ASSERT(wstring::ToString("") == L"");
            TCUNIT_ASSERT(wstring::ToString(static_cast<char *>(0)).empty());
         }


         // convert from wide character to ascii
         {
            TCUNIT_ASSERT(wstring::ToString(L"testing ToString( const char* string )")
               == "testing ToString( const char* string )");
            TCUNIT_ASSERT(wstring::ToString(L"") == "");
            TCUNIT_ASSERT(wstring::ToString(static_cast<wchar_t *>(0)).empty());
         }

         // convert from uint64_t
         {
            uint64_t uint64_number = 123456789;
            TCUNIT_ASSERT(wstring::ToString(uint64_number)      == L"123456789");
            uint64_number = TC_UINT64_VAL(18446744073709551615);
            TCUNIT_ASSERT(wstring::ToString(uint64_number)      == L"18446744073709551615");
            uint64_number = 0;
            TCUNIT_ASSERT(wstring::ToString(uint64_number)      == L"0");
         }

         // convert from uint32_t
         {
            uint32_t uint32_number = 123456789;
            TCUNIT_ASSERT(wstring::ToString(uint32_number)      == L"123456789");
            uint32_number = 4294967295ul;
            TCUNIT_ASSERT(wstring::ToString(uint32_number)      == L"4294967295");
            uint32_number = 0;
            TCUNIT_ASSERT(wstring::ToString(uint32_number)      == L"0");
         }

         // convert from uint16_t
         {
            uint16_t uint16_number = 12345;
            TCUNIT_ASSERT(wstring::ToString(uint16_number)      == L"12345");
            uint16_number = 65535;
            TCUNIT_ASSERT(wstring::ToString(uint16_number)      == L"65535");
            uint16_number = 0;
            TCUNIT_ASSERT(wstring::ToString(uint16_number)      == L"0");
         }


         // convert from int64_t
         {
            int64_t sint64_number = 123456789;
            TCUNIT_ASSERT(wstring::ToString(sint64_number)      == L"123456789");
            sint64_number = -123456789;
            TCUNIT_ASSERT(wstring::ToString(sint64_number)      == L"-123456789");
            sint64_number = static_cast<int64_t>(TC_SINT64_VAL(9223372036854775807));
            TCUNIT_ASSERT(wstring::ToString(sint64_number)      == L"9223372036854775807");
            sint64_number = TC_SINT64_VAL(-9223372036854775807);
            TCUNIT_ASSERT(wstring::ToString(sint64_number)      == L"-9223372036854775807");
            sint64_number = 0;
            TCUNIT_ASSERT(wstring::ToString(sint64_number)      == L"0");
         }

         // convert from int32_t
         {
            int32_t sint32_number = 123456789;
            TCUNIT_ASSERT(wstring::ToString(sint32_number)      == L"123456789");
            sint32_number = -123456789;
            TCUNIT_ASSERT(wstring::ToString(sint32_number)      == L"-123456789");
            sint32_number = 2147483647;
            TCUNIT_ASSERT(wstring::ToString(sint32_number)      == L"2147483647");
            sint32_number = -2147483647;
            TCUNIT_ASSERT(wstring::ToString(sint32_number)      == L"-2147483647");
            sint32_number = 0;
            TCUNIT_ASSERT(wstring::ToString(sint32_number)      == L"0");
         }

         // convert from int16_t
         {
            int16_t sint16_number = 12345;
            TCUNIT_ASSERT(wstring::ToString(sint16_number)      == L"12345");
            sint16_number = -12345;
            TCUNIT_ASSERT(wstring::ToString(sint16_number)      == L"-12345");
            sint16_number = 32767;
            TCUNIT_ASSERT(wstring::ToString(sint16_number)      == L"32767");
            sint16_number = -32768;
            TCUNIT_ASSERT(wstring::ToString(sint16_number)      == L"-32768");
            sint16_number = 0;
            TCUNIT_ASSERT(wstring::ToString(sint16_number)      == L"0");
         }

         // convert from float
         {
            wchar_t tmp_string[100];
            snwprintf(tmp_string, 100, L"%f", 0.123456f);
            TCUNIT_ASSERT(wstring::ToString(0.123456f)    == tmp_string);
            snwprintf(tmp_string, 100, L"%f", -0.123456f);
            TCUNIT_ASSERT(wstring::ToString(-0.123456f)   == tmp_string);
            snwprintf(tmp_string, 100, L"%f", 123.456f);
            TCUNIT_ASSERT(wstring::ToString(123.456f)     == tmp_string);
            snwprintf(tmp_string, 100, L"%f", -123.456f);
            TCUNIT_ASSERT(wstring::ToString(-123.456f)    == tmp_string);
         }


         // convert from double
         {
            wchar_t tmp_string[100];
            snwprintf(tmp_string, 100, L"%lf", 0.123456);
            TCUNIT_ASSERT(wstring::ToString(0.123456)    == tmp_string);
            snwprintf(tmp_string, 100, L"%lf", -0.123456);
            TCUNIT_ASSERT(wstring::ToString(-0.123456)   == tmp_string);
            snwprintf(tmp_string, 100, L"%lf", 123.456);
            TCUNIT_ASSERT(wstring::ToString(123.456)     == tmp_string);
            snwprintf(tmp_string, 100, L"%lf", -123.456);
            TCUNIT_ASSERT(wstring::ToString(-123.456)    == tmp_string);
         }
      }

      class StringManipulation: public unit::TestCase
      {
      public:
         StringManipulation() : unit::TestCase("tc::tests::StringManipulation") {}
         virtual void Execute()
         {
            // test trim spaces
            {
               std::wstring test_string = L"   test string   ";
               test_string = wstring::TrimmSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");

               test_string = L"test string   ";
               test_string = wstring::TrimmSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");

               test_string = L"    test string";
               test_string = wstring::TrimmSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");
            }

            // test trim leading spaces
            {
               std::wstring test_string = L"   test string   ";
               test_string = wstring::TrimmLeadingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string   ");

               test_string = L"test string   ";
               test_string = wstring::TrimmLeadingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string   ");

               test_string = L"    test string";
               test_string = wstring::TrimmLeadingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");
            }

            // test trim trailing spaces
            {
               std::wstring test_string = L"   test string   ";
               test_string = wstring::TrimmTrailingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"   test string");

               test_string = L"test string   ";
               test_string = wstring::TrimmTrailingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");

               test_string = L"    test string";
               test_string = wstring::TrimmTrailingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"    test string");
            }

            // test replace string
            {
               std::wstring test_string = L"test string";
               test_string = wstring::Replace(test_string, L"test", L"123");
               TCUNIT_ASSERT(test_string == L"123 string");

               test_string = L"test string test testtest";
               test_string = wstring::Replace(test_string, L"test", L"123");
               TCUNIT_ASSERT(test_string == L"123 string 123 123123");

               //             test_string = L"test string test testtest";
               //             wstring::Replace(test_string, L"test", 2, L"123", 3);
               //             TCUNIT_ASSERT(test_string == L"123st string 123st 123st123st");
               // 
               //             test_string = L"test string test testtest";
               //             wstring::Replace(test_string, L"test", 3, L"123", 2);
               //             TCUNIT_ASSERT(test_string == L"12t string 12t 12t12t");
            }
         }
      };

      class Split: public unit::TestCase
      {
      public:
         Split() : unit::TestCase("tc::tests::Split") {}

         virtual void Execute()
         {
            {
               std::vector<std::wstring> values;
               //             wstring::Split(L"1 2 3", L' ', values, false);
               //             TCUNIT_ASSERT(values.size()==3 && values[0]==L"1" && values[1]==L"2" && values[2]==L"3");

               wstring::Split(L"1 2 3", L" ", values/*, false*/);
               TCUNIT_ASSERT(values.size()==3 && values[0]==L"1" && values[1]==L"2" && values[2]==L"3");

               //             wstring::Split(L"1 2 3", L" ", values, true);
               //             TCUNIT_ASSERT(values.size()==6 && values[0]==L"1" && values[1]==L"2" && values[2]==L"3"
               //                && values[3]==L"1" && values[4]==L"2" && values[5]==L"3");
            }
            // 
            //          {
            //             std::vector<uint32_t> values;
            //             wstring::Split(L"1 2 3", L' ', values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==2 && values[2]==3);
            // 
            //             wstring::Split(L"1 2 3", L" ", values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==2 && values[2]==3);
            // 
            //             wstring::Split(L"1 2 3", L" ", values, true);
            //             TCUNIT_ASSERT(values.size()==6 && values[0]==1 && values[1]==2 && values[2]==3
            //                && values[3]==1 && values[4]==2 && values[5]==3);
            //          }
            // 
            //          {
            //             std::vector<int32_t> values;
            //             wstring::Split(L"1 -2 3", L' ', values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==-2 && values[2]==3);
            // 
            //             wstring::Split(L"1 -2 3", L" ", values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==-2 && values[2]==3);
            // 
            //             wstring::Split(L"1 -2 3", L" ", values, true);
            //             TCUNIT_ASSERT(values.size()==6 && values[0]==1 && values[1]==-2 && values[2]==3
            //                && values[3]==1 && values[4]==-2 && values[5]==3);
            //          }
            // 
            //          {
            //             std::vector<double> values;
            //             wstring::Split(L"1.1 -2.2 3.3", L' ', values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3);
            // 
            //             wstring::Split(L"1.1 -2.2 3.3", L" ", values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3);
            // 
            //             wstring::Split(L"1.1 -2.2 3.3", L" ", values, true);
            //             TCUNIT_ASSERT(values.size()==6 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3
            //                && values[3]==1.1 && values[4]==-2.2 && values[5]==3.3);
            //          }
         }
      };

      class Print: public unit::TestCase
      {
      public:
         Print() : unit::TestCase("tc::tests::Print") {}
         virtual void Execute()
         {
            {
               wchar_t buffer[100];
               int32_t l = wstring::Snprintf(buffer, tc::util::ArraySize(buffer),
                  L"%d, %g, %ls", 1, 1.0, L"test");
               TCUNIT_ASSERT(l != -1);
               TCUNIT_ASSERT(wcscmp(L"1, 1, test", buffer) == 0);

            }

            {
               wchar_t buffer[5];
               int32_t l = wstring::Snprintf(buffer, tc::util::ArraySize(buffer),
                  L"%d, %g, %ls", 1, 1.0, L"test");
               TCUNIT_ASSERT(l == -1);
               TCUNIT_ASSERT(wcscmp(L"1, 1", buffer) == 0);

            }

            {
               std::wstring s = wstring::Print(L"%d, %g, %ls", 1, 1.0, L"test");
               TCUNIT_ASSERT(s == L"1, 1, test");
            }

            {
               std::wstring s = wstring::Print(L"%d, %g, %ls", 1, 1.0,
                  L"12345678901234567890123456789012345678901234567890123456789012345678901234567890");
               TCUNIT_ASSERT(s == L"1, 1, 12345678901234567890123456789012345678901234567890123456789012345678901234567890");
            }
         }
      };
   }

   namespace tests
   {
      WStringTestSuite::WStringTestSuite()
         :unit::TestSuite("tc::WStringTestSuite")
      {
         AddTest(unit::Test::Ptr(new ConvertToUpperLower));
         AddTest(unit::Test::Ptr(new ConvertToNumber));
         AddTest(unit::Test::Ptr(new ConvertToString));
         AddTest(unit::Test::Ptr(new StringManipulation));
         AddTest(unit::Test::Ptr(new Split));
         AddTest(unit::Test::Ptr(new Print));
      }
   }
}

#endif
