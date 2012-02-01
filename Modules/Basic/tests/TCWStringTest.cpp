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
#endif

#ifdef TCOS_LINUX
#   define snwprintf std::swprintf
#   define vsnwprintf  std::vswprintf
#endif


namespace TC
{
   namespace
   {
      class ConvertToUpperLower: public Unit::TestCase
      {
      public:
         ConvertToUpperLower() : Unit::TestCase("TC::Tests::ConvertToUpperLower") {}
         virtual void Execute();
      };

      void ConvertToUpperLower::Execute()
      {
         std::wstring text;

         // test conversion to upper case
         text = L"ConvertToUpper";
         text = WString::ToUpper(text);
         TCUNIT_ASSERT(text == L"CONVERTTOUPPER");

         // test conversion to lower case
         text = WString::ToLower(text);
         TCUNIT_ASSERT(text == L"converttoupper");

         text = L"ConvertToLower";
         text = WString::ToLower(text);
         TCUNIT_ASSERT(text == L"converttolower");
      }

      class ConvertToNumber: public Unit::TestCase
      {
      public:
         ConvertToNumber() : Unit::TestCase("TC::Tests::ConvertToNumber") {}
         virtual void Execute();
      };

      void ConvertToNumber::Execute()
      {
         // test conversion to uint32
         TCUNIT_ASSERT(WString::ToUint32(L"123456.789") == 123456);
         TCUNIT_ASSERT(WString::ToUint32(L"0")          == 0);               // min value
         TCUNIT_ASSERT(WString::ToUint32(L"4294967295") == 4294967295ul);      // max value
         TCUNIT_ASSERT(WString::ToUint32(L"-1")         == 4294967295ul);

         // test conversion to sint32
         TCUNIT_ASSERT(WString::ToSint32(L"123456.789") == 123456);
         TCUNIT_ASSERT(WString::ToSint32(L"0") == 0);
         TCUNIT_ASSERT(WString::ToSint32(L"-123") == -123);
         TCUNIT_ASSERT(WString::ToSint32(L"-2147483647") == -2147483647);    // min value
         TCUNIT_ASSERT(WString::ToSint32(L"2147483647") == 2147483647);      // max value

         TCUNIT_ASSERT(fabs(WString::ToFlt32(L"123456.789") - 123456.789f) < 0.1f);
         TCUNIT_ASSERT(WString::ToFlt64(L"123456.789") == 123456.789);
      }

      class ConvertToString: public Unit::TestCase
      {
      public:
         ConvertToString() : Unit::TestCase("TC::Tests::ConvertToString") {}
         virtual void Execute();
      };

      void ConvertToString::Execute()
      {
         // convert from ascii to wide character
         {
            TCUNIT_ASSERT(WString::ToString("testing ToString( const char* string )")
               == L"testing ToString( const char* string )");
            TCUNIT_ASSERT(WString::ToString("") == L"");
            TCUNIT_ASSERT(WString::ToString(static_cast<char *>(0)).empty());
         }


         // convert from wide character to ascii
         {
            TCUNIT_ASSERT(WString::ToString(L"testing ToString( const char* string )")
               == "testing ToString( const char* string )");
            TCUNIT_ASSERT(WString::ToString(L"") == "");
            TCUNIT_ASSERT(WString::ToString(static_cast<wchar_t *>(0)).empty());
         }

         // convert from uint64
         {
            uint64 uint64_number = 123456789;
            TCUNIT_ASSERT(WString::ToString(uint64_number)      == L"123456789");
            uint64_number = TC_UINT64_VAL(18446744073709551615);
            TCUNIT_ASSERT(WString::ToString(uint64_number)      == L"18446744073709551615");
            uint64_number = 0;
            TCUNIT_ASSERT(WString::ToString(uint64_number)      == L"0");
         }

         // convert from uint32
         {
            uint32 uint32_number = 123456789;
            TCUNIT_ASSERT(WString::ToString(uint32_number)      == L"123456789");
            uint32_number = 4294967295ul;
            TCUNIT_ASSERT(WString::ToString(uint32_number)      == L"4294967295");
            uint32_number = 0;
            TCUNIT_ASSERT(WString::ToString(uint32_number)      == L"0");
         }

         // convert from uint16
         {
            uint16 uint16_number = 12345;
            TCUNIT_ASSERT(WString::ToString(uint16_number)      == L"12345");
            uint16_number = 65535;
            TCUNIT_ASSERT(WString::ToString(uint16_number)      == L"65535");
            uint16_number = 0;
            TCUNIT_ASSERT(WString::ToString(uint16_number)      == L"0");
         }


         // convert from sint64
         {
            sint64 sint64_number = 123456789;
            TCUNIT_ASSERT(WString::ToString(sint64_number)      == L"123456789");
            sint64_number = -123456789;
            TCUNIT_ASSERT(WString::ToString(sint64_number)      == L"-123456789");
            sint64_number = static_cast<sint64>(TC_SINT64_VAL(9223372036854775807));
            TCUNIT_ASSERT(WString::ToString(sint64_number)      == L"9223372036854775807");
            sint64_number = TC_SINT64_VAL(-9223372036854775807);
            TCUNIT_ASSERT(WString::ToString(sint64_number)      == L"-9223372036854775807");
            sint64_number = 0;
            TCUNIT_ASSERT(WString::ToString(sint64_number)      == L"0");
         }

         // convert from sint32
         {
            sint32 sint32_number = 123456789;
            TCUNIT_ASSERT(WString::ToString(sint32_number)      == L"123456789");
            sint32_number = -123456789;
            TCUNIT_ASSERT(WString::ToString(sint32_number)      == L"-123456789");
            sint32_number = 2147483647;
            TCUNIT_ASSERT(WString::ToString(sint32_number)      == L"2147483647");
            sint32_number = -2147483647;
            TCUNIT_ASSERT(WString::ToString(sint32_number)      == L"-2147483647");
            sint32_number = 0;
            TCUNIT_ASSERT(WString::ToString(sint32_number)      == L"0");
         }

         // convert from sint16
         {
            sint16 sint16_number = 12345;
            TCUNIT_ASSERT(WString::ToString(sint16_number)      == L"12345");
            sint16_number = -12345;
            TCUNIT_ASSERT(WString::ToString(sint16_number)      == L"-12345");
            sint16_number = 32767;
            TCUNIT_ASSERT(WString::ToString(sint16_number)      == L"32767");
            sint16_number = -32768;
            TCUNIT_ASSERT(WString::ToString(sint16_number)      == L"-32768");
            sint16_number = 0;
            TCUNIT_ASSERT(WString::ToString(sint16_number)      == L"0");
         }

         // convert from float
         {
            wchar_t tmp_string[100];
            snwprintf(tmp_string, 100, L"%f", 0.123456f);
            TCUNIT_ASSERT(WString::ToString(0.123456f)    == tmp_string);
            snwprintf(tmp_string, 100, L"%f", -0.123456f);
            TCUNIT_ASSERT(WString::ToString(-0.123456f)   == tmp_string);
            snwprintf(tmp_string, 100, L"%f", 123.456f);
            TCUNIT_ASSERT(WString::ToString(123.456f)     == tmp_string);
            snwprintf(tmp_string, 100, L"%f", -123.456f);
            TCUNIT_ASSERT(WString::ToString(-123.456f)    == tmp_string);
         }


         // convert from double
         {
            wchar_t tmp_string[100];
            snwprintf(tmp_string, 100, L"%lf", 0.123456);
            TCUNIT_ASSERT(WString::ToString(0.123456)    == tmp_string);
            snwprintf(tmp_string, 100, L"%lf", -0.123456);
            TCUNIT_ASSERT(WString::ToString(-0.123456)   == tmp_string);
            snwprintf(tmp_string, 100, L"%lf", 123.456);
            TCUNIT_ASSERT(WString::ToString(123.456)     == tmp_string);
            snwprintf(tmp_string, 100, L"%lf", -123.456);
            TCUNIT_ASSERT(WString::ToString(-123.456)    == tmp_string);
         }
      }

      class StringManipulation: public Unit::TestCase
      {
      public:
         StringManipulation() : Unit::TestCase("TC::Tests::StringManipulation") {}
         virtual void Execute()
         {
            // test trim spaces
            {
               std::wstring test_string = L"   test string   ";
               test_string = WString::TrimmSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");

               test_string = L"test string   ";
               test_string = WString::TrimmSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");

               test_string = L"    test string";
               test_string = WString::TrimmSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");
            }

            // test trim leading spaces
            {
               std::wstring test_string = L"   test string   ";
               test_string = WString::TrimmLeadingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string   ");

               test_string = L"test string   ";
               test_string = WString::TrimmLeadingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string   ");

               test_string = L"    test string";
               test_string = WString::TrimmLeadingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");
            }

            // test trim trailing spaces
            {
               std::wstring test_string = L"   test string   ";
               test_string = WString::TrimmTrailingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"   test string");

               test_string = L"test string   ";
               test_string = WString::TrimmTrailingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"test string");

               test_string = L"    test string";
               test_string = WString::TrimmTrailingSpaces(test_string);
               TCUNIT_ASSERT(test_string == L"    test string");
            }

            // test replace string
            {
               std::wstring test_string = L"test string";
               test_string = WString::Replace(test_string, L"test", L"123");
               TCUNIT_ASSERT(test_string == L"123 string");

               test_string = L"test string test testtest";
               test_string = WString::Replace(test_string, L"test", L"123");
               TCUNIT_ASSERT(test_string == L"123 string 123 123123");

               //             test_string = L"test string test testtest";
               //             WString::Replace(test_string, L"test", 2, L"123", 3);
               //             TCUNIT_ASSERT(test_string == L"123st string 123st 123st123st");
               // 
               //             test_string = L"test string test testtest";
               //             WString::Replace(test_string, L"test", 3, L"123", 2);
               //             TCUNIT_ASSERT(test_string == L"12t string 12t 12t12t");
            }
         }
      };

      class Split: public Unit::TestCase
      {
      public:
         Split() : Unit::TestCase("TC::Tests::Split") {}

         virtual void Execute()
         {
            {
               std::vector<std::wstring> values;
               //             WString::Split(L"1 2 3", L' ', values, false);
               //             TCUNIT_ASSERT(values.size()==3 && values[0]==L"1" && values[1]==L"2" && values[2]==L"3");

               WString::Split(L"1 2 3", L" ", values/*, false*/);
               TCUNIT_ASSERT(values.size()==3 && values[0]==L"1" && values[1]==L"2" && values[2]==L"3");

               //             WString::Split(L"1 2 3", L" ", values, true);
               //             TCUNIT_ASSERT(values.size()==6 && values[0]==L"1" && values[1]==L"2" && values[2]==L"3"
               //                && values[3]==L"1" && values[4]==L"2" && values[5]==L"3");
            }
            // 
            //          {
            //             std::vector<uint32> values;
            //             WString::Split(L"1 2 3", L' ', values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==2 && values[2]==3);
            // 
            //             WString::Split(L"1 2 3", L" ", values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==2 && values[2]==3);
            // 
            //             WString::Split(L"1 2 3", L" ", values, true);
            //             TCUNIT_ASSERT(values.size()==6 && values[0]==1 && values[1]==2 && values[2]==3
            //                && values[3]==1 && values[4]==2 && values[5]==3);
            //          }
            // 
            //          {
            //             std::vector<sint32> values;
            //             WString::Split(L"1 -2 3", L' ', values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==-2 && values[2]==3);
            // 
            //             WString::Split(L"1 -2 3", L" ", values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==-2 && values[2]==3);
            // 
            //             WString::Split(L"1 -2 3", L" ", values, true);
            //             TCUNIT_ASSERT(values.size()==6 && values[0]==1 && values[1]==-2 && values[2]==3
            //                && values[3]==1 && values[4]==-2 && values[5]==3);
            //          }
            // 
            //          {
            //             std::vector<double> values;
            //             WString::Split(L"1.1 -2.2 3.3", L' ', values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3);
            // 
            //             WString::Split(L"1.1 -2.2 3.3", L" ", values, false);
            //             TCUNIT_ASSERT(values.size()==3 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3);
            // 
            //             WString::Split(L"1.1 -2.2 3.3", L" ", values, true);
            //             TCUNIT_ASSERT(values.size()==6 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3
            //                && values[3]==1.1 && values[4]==-2.2 && values[5]==3.3);
            //          }
         }
      };

      class Print: public Unit::TestCase
      {
      public:
         Print() : Unit::TestCase("TC::Tests::Print") {}
         virtual void Execute()
         {
            {
               wchar_t buffer[100];
               sint32 l = WString::Snprintf(buffer, TC::Util::ArraySize(buffer),
                  L"%d, %g, %ls", 1, 1.0, L"test");
               TCUNIT_ASSERT(l != -1);
               TCUNIT_ASSERT(wcscmp(L"1, 1, test", buffer) == 0);

            }

            {
               wchar_t buffer[5];
               sint32 l = WString::Snprintf(buffer, TC::Util::ArraySize(buffer),
                  L"%d, %g, %ls", 1, 1.0, L"test");
               TCUNIT_ASSERT(l == -1);
               TCUNIT_ASSERT(wcscmp(L"1, 1", buffer) == 0);

            }

            {
               std::wstring s = WString::Print(L"%d, %g, %ls", 1, 1.0, L"test");
               TCUNIT_ASSERT(s == L"1, 1, test");
            }

            {
               std::wstring s = WString::Print(L"%d, %g, %ls", 1, 1.0,
                  L"12345678901234567890123456789012345678901234567890123456789012345678901234567890");
               TCUNIT_ASSERT(s == L"1, 1, 12345678901234567890123456789012345678901234567890123456789012345678901234567890");
            }
         }
      };
   }

   namespace Tests
   {
      WStringTestSuite::WStringTestSuite()
         :Unit::TestSuite("TC::WStringTestSuite")
      {
         AddTest(Unit::Test::Ptr(new ConvertToUpperLower));
         AddTest(Unit::Test::Ptr(new ConvertToNumber));
         AddTest(Unit::Test::Ptr(new ConvertToString));
         AddTest(Unit::Test::Ptr(new StringManipulation));
         AddTest(Unit::Test::Ptr(new Split));
         AddTest(Unit::Test::Ptr(new Print));
      }
   }
}

#endif