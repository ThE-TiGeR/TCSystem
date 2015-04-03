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
//  $Id: TCHashTableTest.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//----------------------------------------------------------------------
#ifdef HAVE_UNIT_TESTS
#include "TCHashTableTest.h"

#include "TCHashTable.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace
   {
      class ConstructTest: public unit::TestCase
      {
      public:
         ConstructTest()
            :unit::TestCase("tc::tests::ConstructTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, int32_t> hash_table(100, -1);
            TCUNIT_ASSERT(hash_table.GetHashSize() == 100);
            TCUNIT_ASSERT(hash_table.GetNotFoundValue() == -1);
            std::vector<std::string> keys;
            hash_table.GetAllKeys(keys);
            TCUNIT_ASSERT(keys.size() == 0);
         }
      };

      class HashSizeTest: public unit::TestCase
      {
      public:
         HashSizeTest()
            :unit::TestCase("tc::tests::HashSizeTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, int32_t> hash_table(100, -1);
            hash_table.SetHashSize(200);
            TCUNIT_ASSERT(hash_table.GetHashSize() == 200);
         }
      };

      class NotFoundValueTest: public unit::TestCase
      {
      public:
         NotFoundValueTest()
            :unit::TestCase("tc::tests::NotFoundValueTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, int32_t> hash_table(100, -1);
            hash_table.SetNotFoundValue(0);
            TCUNIT_ASSERT(hash_table.GetNotFoundValue() == 0);
         }
      };

      class AddKeyTest: public unit::TestCase
      {
      public:
         AddKeyTest()
            :unit::TestCase("tc::tests::AddKeyTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, int32_t> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            TCUNIT_ASSERT(hash_table.GetMappedValue("test1") == 1);
            TCUNIT_ASSERT(hash_table.GetMappedValue("test2") == 2);

            hash_table.AddKey("test2", 3);
            TCUNIT_ASSERT(hash_table.GetMappedValue("test2") == 3);
         }
      };

      class RemoveKeyTest: public unit::TestCase
      {
      public:
         RemoveKeyTest()
            :unit::TestCase("tc::tests::RemoveKeyTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, int32_t> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            hash_table.RemoveKey("test2");
            TCUNIT_ASSERT(hash_table.GetMappedValue("test1") == 1);
            TCUNIT_ASSERT(hash_table.GetMappedValue("test2") == hash_table.GetNotFoundValue());
         }
      };

      class ClearTest: public unit::TestCase
      {
      public:
         ClearTest()
            :unit::TestCase("tc::tests::ClearTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, int32_t> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            hash_table.Clear();
            TCUNIT_ASSERT(hash_table.GetMappedValue("test1") == hash_table.GetNotFoundValue());
            TCUNIT_ASSERT(hash_table.GetMappedValue("test2") == hash_table.GetNotFoundValue());
         }
      };

      class GetAllKeysTest: public unit::TestCase
      {
      public:
         GetAllKeysTest()
            :unit::TestCase("tc::tests::GetAllKeysTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, int32_t> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            std::vector<std::string> keys;
            hash_table.GetAllKeys(keys);
            TCUNIT_ASSERT(keys.size() == 2);
            TCUNIT_ASSERT(keys[0] == "test2");
            TCUNIT_ASSERT(keys[1] == "test1");
         }
      };

      class GetAllKeysAndValuesTest: public unit::TestCase
      {
      public:
         GetAllKeysAndValuesTest()
            :unit::TestCase("tc::tests::GetAllKeysAndValuesTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, int32_t> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            std::vector<std::string> keys;
            std::vector<int32_t> values;
            hash_table.GetAllKeysAndValues(keys, values);
            TCUNIT_ASSERT(keys.size() == 2);
            TCUNIT_ASSERT(keys[0] == "test2");
            TCUNIT_ASSERT(keys[1] == "test1");
            TCUNIT_ASSERT(values.size() == 2);
            TCUNIT_ASSERT(values[0] == 2);
            TCUNIT_ASSERT(values[1] == 1);
         }
      };
   }

   namespace tests
   {
      HashTableTestSuite::HashTableTestSuite()
         :unit::TestSuite("tc::tests::HashTableTestSuite")
      {
         AddTest(unit::Test::Ptr(new ConstructTest));
         AddTest(unit::Test::Ptr(new HashSizeTest));
         AddTest(unit::Test::Ptr(new NotFoundValueTest));
         AddTest(unit::Test::Ptr(new AddKeyTest));
         AddTest(unit::Test::Ptr(new RemoveKeyTest));
         AddTest(unit::Test::Ptr(new ClearTest));
         AddTest(unit::Test::Ptr(new GetAllKeysTest));
         AddTest(unit::Test::Ptr(new GetAllKeysAndValuesTest));
      }
   }
}

#endif
