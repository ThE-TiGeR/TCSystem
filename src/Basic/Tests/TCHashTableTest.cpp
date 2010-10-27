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
#include "TCHashTableTest.h"

#include "TCHashTable.h"

#include <jf/unittest/test_case.h>

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class ConstructTest: public jf::unittest::TestCase
      {
      public:
         ConstructTest()
            :jf::unittest::TestCase("TC::Tests::ConstructTest")
         {
         }
         virtual void run()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            JFUNIT_ASSERT(hash_table.GetHashSize() == 100);
            JFUNIT_ASSERT(hash_table.GetNotFoundValue() == -1);
            std::vector<std::string> keys;
            hash_table.GetAllKeys(keys);
            JFUNIT_ASSERT(keys.size() == 0);
         }
      };

      class HashSizeTest: public jf::unittest::TestCase
      {
      public:
         HashSizeTest()
            :jf::unittest::TestCase("TC::Tests::HashSizeTest")
         {
         }
         virtual void run()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.SetHashSize(200);
            JFUNIT_ASSERT(hash_table.GetHashSize() == 200);
         }
      };

      class NotFoundValueTest: public jf::unittest::TestCase
      {
      public:
         NotFoundValueTest()
            :jf::unittest::TestCase("TC::Tests::NotFoundValueTest")
         {
         }
         virtual void run()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.SetNotFoundValue(0);
            JFUNIT_ASSERT(hash_table.GetNotFoundValue() == 0);
         }
      };

      class AddKeyTest: public jf::unittest::TestCase
      {
      public:
         AddKeyTest()
            :jf::unittest::TestCase("TC::Tests::AddKeyTest")
         {
         }
         virtual void run()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            JFUNIT_ASSERT(hash_table.GetMappedValue("test1") == 1);
            JFUNIT_ASSERT(hash_table.GetMappedValue("test2") == 2);

            hash_table.AddKey("test2", 3);
            JFUNIT_ASSERT(hash_table.GetMappedValue("test2") == 3);
         }
      };

      class RemoveKeyTest: public jf::unittest::TestCase
      {
      public:
         RemoveKeyTest()
            :jf::unittest::TestCase("TC::Tests::RemoveKeyTest")
         {
         }
         virtual void run()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            hash_table.RemoveKey("test2");
            JFUNIT_ASSERT(hash_table.GetMappedValue("test1") == 1);
            JFUNIT_ASSERT(hash_table.GetMappedValue("test2") == hash_table.GetNotFoundValue());
         }
      };

      class ClearTest: public jf::unittest::TestCase
      {
      public:
         ClearTest()
            :jf::unittest::TestCase("TC::Tests::ClearTest")
         {
         }
         virtual void run()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            hash_table.Clear();
            JFUNIT_ASSERT(hash_table.GetMappedValue("test1") == hash_table.GetNotFoundValue());
            JFUNIT_ASSERT(hash_table.GetMappedValue("test2") == hash_table.GetNotFoundValue());
         }
      };

      class GetAllKeysTest: public jf::unittest::TestCase
      {
      public:
         GetAllKeysTest()
            :jf::unittest::TestCase("TC::Tests::GetAllKeysTest")
         {
         }
         virtual void run()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            std::vector<std::string> keys;
            hash_table.GetAllKeys(keys);
            JFUNIT_ASSERT(keys.size() == 2);
            JFUNIT_ASSERT(keys[0] == "test2");
            JFUNIT_ASSERT(keys[1] == "test1");
         }
      };

      class GetAllKeysAndValuesTest: public jf::unittest::TestCase
      {
      public:
         GetAllKeysAndValuesTest()
            :jf::unittest::TestCase("TC::Tests::GetAllKeysAndValuesTest")
         {
         }
         virtual void run()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            std::vector<std::string> keys;
            std::vector<sint32> values;
            hash_table.GetAllKeysAndValues(keys, values);
            JFUNIT_ASSERT(keys.size() == 2);
            JFUNIT_ASSERT(keys[0] == "test2");
            JFUNIT_ASSERT(keys[1] == "test1");
            JFUNIT_ASSERT(values.size() == 2);
            JFUNIT_ASSERT(values[0] == 2);
            JFUNIT_ASSERT(values[1] == 1);
         }
      };
   }

   namespace Tests
   {
      HashTableTestSuite::HashTableTestSuite()
         :jf::unittest::TestSuite("TC::Tests::HashTableTestSuite")
      {
         add_test(new ConstructTest);
         add_test(new HashSizeTest);
         add_test(new NotFoundValueTest);
         add_test(new AddKeyTest);
         add_test(new RemoveKeyTest);
         add_test(new ClearTest);
         add_test(new GetAllKeysTest);
         add_test(new GetAllKeysAndValuesTest);
      }
   }
}

#endif
