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

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class ConstructTest: public Unit::TestCase
      {
      public:
         ConstructTest()
            :Unit::TestCase("TC::Tests::ConstructTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            TCUNIT_ASSERT(hash_table.GetHashSize() == 100);
            TCUNIT_ASSERT(hash_table.GetNotFoundValue() == -1);
            std::vector<std::string> keys;
            hash_table.GetAllKeys(keys);
            TCUNIT_ASSERT(keys.size() == 0);
         }
      };

      class HashSizeTest: public Unit::TestCase
      {
      public:
         HashSizeTest()
            :Unit::TestCase("TC::Tests::HashSizeTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.SetHashSize(200);
            TCUNIT_ASSERT(hash_table.GetHashSize() == 200);
         }
      };

      class NotFoundValueTest: public Unit::TestCase
      {
      public:
         NotFoundValueTest()
            :Unit::TestCase("TC::Tests::NotFoundValueTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.SetNotFoundValue(0);
            TCUNIT_ASSERT(hash_table.GetNotFoundValue() == 0);
         }
      };

      class AddKeyTest: public Unit::TestCase
      {
      public:
         AddKeyTest()
            :Unit::TestCase("TC::Tests::AddKeyTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            TCUNIT_ASSERT(hash_table.GetMappedValue("test1") == 1);
            TCUNIT_ASSERT(hash_table.GetMappedValue("test2") == 2);

            hash_table.AddKey("test2", 3);
            TCUNIT_ASSERT(hash_table.GetMappedValue("test2") == 3);
         }
      };

      class RemoveKeyTest: public Unit::TestCase
      {
      public:
         RemoveKeyTest()
            :Unit::TestCase("TC::Tests::RemoveKeyTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            hash_table.RemoveKey("test2");
            TCUNIT_ASSERT(hash_table.GetMappedValue("test1") == 1);
            TCUNIT_ASSERT(hash_table.GetMappedValue("test2") == hash_table.GetNotFoundValue());
         }
      };

      class ClearTest: public Unit::TestCase
      {
      public:
         ClearTest()
            :Unit::TestCase("TC::Tests::ClearTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            hash_table.Clear();
            TCUNIT_ASSERT(hash_table.GetMappedValue("test1") == hash_table.GetNotFoundValue());
            TCUNIT_ASSERT(hash_table.GetMappedValue("test2") == hash_table.GetNotFoundValue());
         }
      };

      class GetAllKeysTest: public Unit::TestCase
      {
      public:
         GetAllKeysTest()
            :Unit::TestCase("TC::Tests::GetAllKeysTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            std::vector<std::string> keys;
            hash_table.GetAllKeys(keys);
            TCUNIT_ASSERT(keys.size() == 2);
            TCUNIT_ASSERT(keys[0] == "test2");
            TCUNIT_ASSERT(keys[1] == "test1");
         }
      };

      class GetAllKeysAndValuesTest: public Unit::TestCase
      {
      public:
         GetAllKeysAndValuesTest()
            :Unit::TestCase("TC::Tests::GetAllKeysAndValuesTest")
         {
         }
         virtual void Execute()
         {
            HashTable<std::string, sint32> hash_table(100, -1);
            hash_table.AddKey("test1", 1);
            hash_table.AddKey("test2", 2);

            std::vector<std::string> keys;
            std::vector<sint32> values;
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

   namespace Tests
   {
      HashTableTestSuite::HashTableTestSuite()
         :Unit::TestSuite("TC::Tests::HashTableTestSuite")
      {
         AddTest(new ConstructTest);
         AddTest(new HashSizeTest);
         AddTest(new NotFoundValueTest);
         AddTest(new AddKeyTest);
         AddTest(new RemoveKeyTest);
         AddTest(new ClearTest);
         AddTest(new GetAllKeysTest);
         AddTest(new GetAllKeysAndValuesTest);
      }
   }
}

#endif
