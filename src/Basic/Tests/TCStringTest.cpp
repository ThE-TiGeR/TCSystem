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
#include "TCStringTest.h"

#include "TCString.h"
#include "TCUtil.h"

#include <jf/unittest/test_case.h>
#include <cstring>
#include <cmath>

#include "TCNewEnable.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#define snprintf _snprintf
#endif

namespace TC
{
   namespace
   {
      class ConvertToUpperLower: public jf::unittest::TestCase
      {
      public:
         ConvertToUpperLower() : jf::unittest::TestCase("TC::Tests::ConvertToUpperLower") {}
         virtual void run();
      };

      void ConvertToUpperLower::run()
      {
         std::string text;

         // test conversion to upper case
         text = "ConvertToUpper";
         text = String::ToUpper(text);
         JFUNIT_ASSERT(text == "CONVERTTOUPPER");

         // test conversion to lower case
         text = String::ToLower(text);
         JFUNIT_ASSERT(text == "converttoupper");

         text = "ConvertToLower";
         text = String::ToLower(text);
         JFUNIT_ASSERT(text == "converttolower");
      }

      // -------------------------------------------------------------------
      class ConvertToNumber: public jf::unittest::TestCase
      {
      public:
         ConvertToNumber() : jf::unittest::TestCase("TC::Tests::ConvertToNumber") {}
         virtual void run();
      };

      void ConvertToNumber::run()
      {
         // test conversion to uint32
         JFUNIT_ASSERT(String::ToUint32("123456.789") == 123456);
         JFUNIT_ASSERT(String::ToUint32("0")          == 0);               // min value
         JFUNIT_ASSERT(String::ToUint32("4294967295") == 4294967295ul);      // max value
         JFUNIT_ASSERT(String::ToUint32("-1")         == 4294967295ul);

         // test conversion to sint32
         JFUNIT_ASSERT(String::ToSint32("123456.789") == 123456);
         JFUNIT_ASSERT(String::ToSint32("0") == 0);
         JFUNIT_ASSERT(String::ToSint32("-123") == -123);
         JFUNIT_ASSERT(String::ToSint32("-2147483647") == -2147483647);    // min value
         JFUNIT_ASSERT(String::ToSint32("2147483647") == 2147483647);      // max value

         JFUNIT_ASSERT(fabs(String::ToFlt32("123456.789") - 123456.789f) < 0.1f);
         JFUNIT_ASSERT(String::ToFlt64("123456.789") == 123456.789);
      }

      // -------------------------------------------------------------------
      class ConvertToString: public jf::unittest::TestCase
      {
      public:
         ConvertToString() : jf::unittest::TestCase("TC::Tests::ConvertToString") {}
         virtual void run();
      };

      void ConvertToString::run()
      {
         // convert from ascii to std::string
         {
            JFUNIT_ASSERT(String::ToString("testing ToString( const char* string )")
               == "testing ToString( const char* string )");
            JFUNIT_ASSERT(String::ToString("") == "");
            JFUNIT_ASSERT(String::ToString(static_cast<char *>(NULL)).empty());
         }

         // convert from uint64
         {
            uint64 uint64_number = 123456789;
            JFUNIT_ASSERT(String::ToString(uint64_number)      == "123456789");
            uint64_number = TC_UINT64_VAL(18446744073709551615);
            JFUNIT_ASSERT(String::ToString(uint64_number)      == "18446744073709551615");
            uint64_number = 0;
            JFUNIT_ASSERT(String::ToString(uint64_number)      == "0");
         }

         // convert from uint32
         {
            uint32 uint32_number = 123456789;
            JFUNIT_ASSERT(String::ToString(uint32_number)      == "123456789");
            uint32_number = 4294967295ul;
            JFUNIT_ASSERT(String::ToString(uint32_number)      == "4294967295");
            uint32_number = 0;
            JFUNIT_ASSERT(String::ToString(uint32_number)      == "0");
         }

         // convert from uint16
         {
            uint16 uint16_number = 12345;
            JFUNIT_ASSERT(String::ToString(uint16_number)      == "12345");
            uint16_number = 65535;
            JFUNIT_ASSERT(String::ToString(uint16_number)      == "65535");
            uint16_number = 0;
            JFUNIT_ASSERT(String::ToString(uint16_number)      == "0");
         }


         // convert from sint64
         {
            sint64 sint64_number = 123456789;
            JFUNIT_ASSERT(String::ToString(sint64_number)      == "123456789");
            sint64_number = -123456789;
            JFUNIT_ASSERT(String::ToString(sint64_number)      == "-123456789");
            sint64_number = static_cast<sint64>(TC_SINT64_VAL(9223372036854775807));
            JFUNIT_ASSERT(String::ToString(sint64_number)      == "9223372036854775807");
            sint64_number = TC_SINT64_VAL(-9223372036854775807);
            JFUNIT_ASSERT(String::ToString(sint64_number)      == "-9223372036854775807");
            sint64_number = 0;
            JFUNIT_ASSERT(String::ToString(sint64_number)      == "0");
         }

         // convert from sint32
         {
            sint32 sint32_number = 123456789;
            JFUNIT_ASSERT(String::ToString(sint32_number)      == "123456789");
            sint32_number = -123456789;
            JFUNIT_ASSERT(String::ToString(sint32_number)      == "-123456789");
            sint32_number = 2147483647;
            JFUNIT_ASSERT(String::ToString(sint32_number)      == "2147483647");
            sint32_number = -2147483647;
            JFUNIT_ASSERT(String::ToString(sint32_number)      == "-2147483647");
            sint32_number = 0;
            JFUNIT_ASSERT(String::ToString(sint32_number)      == "0");
         }

         // convert from sint16
         {
            sint16 sint16_number = 12345;
            JFUNIT_ASSERT(String::ToString(sint16_number)      == "12345");
            sint16_number = -12345;
            JFUNIT_ASSERT(String::ToString(sint16_number)      == "-12345");
            sint16_number = 32767;
            JFUNIT_ASSERT(String::ToString(sint16_number)      == "32767");
            sint16_number = -32768;
            JFUNIT_ASSERT(String::ToString(sint16_number)      == "-32768");
            sint16_number = 0;
            JFUNIT_ASSERT(String::ToString(sint16_number)      == "0");
         }

         // convert from float
         {
            char tmp_string[100];
            ::snprintf(tmp_string, 100, "%f", 0.123456f);
            JFUNIT_ASSERT(String::ToString(0.123456f)    == tmp_string);
            ::snprintf(tmp_string, 100, "%f", -0.123456f);
            JFUNIT_ASSERT(String::ToString(-0.123456f)   == tmp_string);
            ::snprintf(tmp_string, 100, "%f", 123.456f);
            JFUNIT_ASSERT(String::ToString(123.456f)     == tmp_string);
            ::snprintf(tmp_string, 100, "%f", -123.456f);
            JFUNIT_ASSERT(String::ToString(-123.456f)    == tmp_string);
         }


         // convert from double
         {
            char tmp_string[100];
            ::snprintf(tmp_string, 100, "%lf", 0.123456);
            JFUNIT_ASSERT(String::ToString(0.123456)    == tmp_string);
            ::snprintf(tmp_string, 100, "%lf", -0.123456);
            JFUNIT_ASSERT(String::ToString(-0.123456)   == tmp_string);
            ::snprintf(tmp_string, 100, "%lf", 123.456);
            JFUNIT_ASSERT(String::ToString(123.456)     == tmp_string);
            ::snprintf(tmp_string, 100, "%lf", -123.456);
            JFUNIT_ASSERT(String::ToString(-123.456)    == tmp_string);
         }
      }

      // -------------------------------------------------------------------
      class StringManipulation: public jf::unittest::TestCase
      {
      public:
         StringManipulation() : jf::unittest::TestCase("TC::Tests::StringManipulation") {}
         virtual void run()
         {
            // test trim spaces
            {
               std::string test_string = "   test string   ";
               test_string = String::TrimmSpaces(test_string);
               JFUNIT_ASSERT(test_string == "test string");

               test_string = "test string   ";
               test_string = String::TrimmSpaces(test_string);
               JFUNIT_ASSERT(test_string == "test string");

               test_string = "    test string";
               test_string = String::TrimmSpaces(test_string);
               JFUNIT_ASSERT(test_string == "test string");
            }

            // test trim leading spaces
            {
               std::string test_string = "   test string   ";
               test_string = String::TrimmLeadingSpaces(test_string);
               JFUNIT_ASSERT(test_string == "test string   ");

               test_string = "test string   ";
               test_string = String::TrimmLeadingSpaces(test_string);
               JFUNIT_ASSERT(test_string == "test string   ");

               test_string = "    test string";
               test_string = String::TrimmLeadingSpaces(test_string);
               JFUNIT_ASSERT(test_string == "test string");
            }

            // test trim trailing spaces
            {
               std::string test_string = "   test string   ";
               test_string = String::TrimmTrailingSpaces(test_string);
               JFUNIT_ASSERT(test_string == "   test string");

               test_string = "test string   ";
               test_string = String::TrimmTrailingSpaces(test_string);
               JFUNIT_ASSERT(test_string == "test string");

               test_string = "    test string";
               test_string = String::TrimmTrailingSpaces(test_string);
               JFUNIT_ASSERT(test_string == "    test string");
            }

            // test replace string
            {
               std::string test_string = "test string";
               test_string = String::Replace(test_string, "test", "123");
               JFUNIT_ASSERT(test_string == "123 string");

               test_string = "test string test testtest";
               test_string = String::Replace(test_string, "test", "123");
               JFUNIT_ASSERT(test_string == "123 string 123 123123");

               //             test_string = "test string test testtest";
               //             String::Replace(test_string, "test", 2, "123", 3);
               //             JFUNIT_ASSERT(test_string == "123st string 123st 123st123st");
               // 
               //             test_string = "test string test testtest";
               //             String::Replace(test_string, "test", 3, "123", 2);
               //             JFUNIT_ASSERT(test_string == "12t string 12t 12t12t");
            }
         }
      };

      // -------------------------------------------------------------------
      class Split: public jf::unittest::TestCase
      {
      public:
         Split() : jf::unittest::TestCase("TC::Tests::Split") {}

         virtual void run()
         {
            {
               std::vector<std::string> values;
               //             String::Split("1 2 3", L' ', values, false);
               //             JFUNIT_ASSERT(values.size()==3 && values[0]=="1" && values[1]=="2" && values[2]=="3");

               String::Split("1 2 3", " ", values/*, false*/);
               JFUNIT_ASSERT(values.size()==3 && values[0]=="1" && values[1]=="2" && values[2]=="3");

               //             String::Split("1 2 3", " ", values, true);
               //             JFUNIT_ASSERT(values.size()==6 && values[0]=="1" && values[1]=="2" && values[2]=="3"
               //                && values[3]=="1" && values[4]=="2" && values[5]=="3");
            }
            // 
            //          {
            //             std::vector<uint32> values;
            //             String::Split("1 2 3", L' ', values, false);
            //             JFUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==2 && values[2]==3);
            // 
            //             String::Split("1 2 3", " ", values, false);
            //             JFUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==2 && values[2]==3);
            // 
            //             String::Split("1 2 3", " ", values, true);
            //             JFUNIT_ASSERT(values.size()==6 && values[0]==1 && values[1]==2 && values[2]==3
            //                && values[3]==1 && values[4]==2 && values[5]==3);
            //          }
            // 
            //          {
            //             std::vector<sint32> values;
            //             String::Split("1 -2 3", L' ', values, false);
            //             JFUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==-2 && values[2]==3);
            // 
            //             String::Split("1 -2 3", " ", values, false);
            //             JFUNIT_ASSERT(values.size()==3 && values[0]==1 && values[1]==-2 && values[2]==3);
            // 
            //             String::Split("1 -2 3", " ", values, true);
            //             JFUNIT_ASSERT(values.size()==6 && values[0]==1 && values[1]==-2 && values[2]==3
            //                && values[3]==1 && values[4]==-2 && values[5]==3);
            //          }
            // 
            //          {
            //             std::vector<double> values;
            //             String::Split("1.1 -2.2 3.3", L' ', values, false);
            //             JFUNIT_ASSERT(values.size()==3 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3);
            // 
            //             String::Split("1.1 -2.2 3.3", " ", values, false);
            //             JFUNIT_ASSERT(values.size()==3 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3);
            // 
            //             String::Split("1.1 -2.2 3.3", " ", values, true);
            //             JFUNIT_ASSERT(values.size()==6 && values[0]==1.1 && values[1]==-2.2 && values[2]==3.3
            //                && values[3]==1.1 && values[4]==-2.2 && values[5]==3.3);
            //          }
         }
      };

      // -------------------------------------------------------------------
      class Print: public jf::unittest::TestCase
      {
      public:
         Print() : jf::unittest::TestCase("TC::Tests::Print") {}
         virtual void run()
         {
            {
               char buffer[100];
               sint32 l = String::Snprintf(buffer, TC::Util::ArraySize(buffer),
                  "%d, %g, %s", 1, 1.0, "test");
               JFUNIT_ASSERT(l != -1);
               JFUNIT_ASSERT(strcmp("1, 1, test", buffer) == 0);

            }

            {
               char buffer[5];
               sint32 l = String::Snprintf(buffer, TC::Util::ArraySize(buffer),
                  "%d, %g, %s", 1, 1.0, "test");
               JFUNIT_ASSERT(l == -1);
               JFUNIT_ASSERT(strcmp("1, 1", buffer) == 0);

            }

            {
               std::string s = String::Print("%d, %g, %s", 1, 1.0, "test");
               JFUNIT_ASSERT(s == "1, 1, test");
            }

            {
               std::string s = String::Print("%d, %g, %s", 1, 1.0,
                  "12345678901234567890123456789012345678901234567890123456789012345678901234567890");
               JFUNIT_ASSERT(s == "1, 1, 12345678901234567890123456789012345678901234567890123456789012345678901234567890");
            }
         }
      };
   }

   namespace Tests
   {
      // -------------------------------------------------------------------
      StringTestSuite::StringTestSuite()
         :jf::unittest::TestSuite("TC::StringTestSuite")
      {
         add_test(new ConvertToUpperLower);
         add_test(new ConvertToNumber);
         add_test(new ConvertToString);
         add_test(new StringManipulation);
         add_test(new Split);
         add_test(new Print);
      }
   }
}

#endif
