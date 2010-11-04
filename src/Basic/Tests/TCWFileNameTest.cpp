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

#include "TCWFileNameTest.h"

#include "TCWFileName.h"

#include <jf/unittest/test_case.h>

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class AddFileNameAndExtension: public jf::unittest::TestCase
      {
      public:
         AddFileNameAndExtension()
            :jf::unittest::TestCase("TC::Tests::AddFileNameAndExtension")
         {
         }
         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::AddFileNameAndExtension(L"hallo", L"txt") == L"hallo.txt");
            JFUNIT_ASSERT(WFileName::AddFileNameAndExtension(L"hallo", L"") == L"hallo");
            JFUNIT_ASSERT(WFileName::AddFileNameAndExtension(L"", L"") == L"");
            JFUNIT_ASSERT(WFileName::AddFileNameAndExtension(L"", L"txt") == L".txt");
         }
      };

      class AddFileNameAndPath: public jf::unittest::TestCase
      {
      public:
         AddFileNameAndPath()
            :jf::unittest::TestCase("TC::Tests::AddFileNameAndPath")
         {
         }

         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::AddFileNameAndPath(L"hallo.txt", L"test") == L"test/hallo.txt");
            JFUNIT_ASSERT(WFileName::AddFileNameAndPath(L"hallo.txt", L"") == L"hallo.txt");
            JFUNIT_ASSERT(WFileName::AddFileNameAndPath(L"hallo.txt", L".") == L"./hallo.txt");
            JFUNIT_ASSERT(WFileName::AddFileNameAndPath(L"", L"test") == L"test/");
         }
      };

      class AddPaths: public jf::unittest::TestCase
      {
      public:
         AddPaths()
            :jf::unittest::TestCase("TC::Tests::AddPaths")
         {
         }

         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::AddPaths(L"test1", L"test2") == L"test1/test2");
         }
      };

      class GetDirectories: public jf::unittest::TestCase
      {
      public:
         GetDirectories()
            :jf::unittest::TestCase("TC::Tests::GetDirectories")
         {
         }

         virtual void run()
         {
            {
               std::vector<std::wstring> dirs;
               WFileName::GetDirectoriesOfFileName(L"test1/test2/test3/text.txt", dirs);
               JFUNIT_ASSERT(dirs.size() == 3);
               JFUNIT_ASSERT(dirs[0] == L"test1");
               JFUNIT_ASSERT(dirs[1] == L"test2");
               JFUNIT_ASSERT(dirs[2] == L"test3");
            }

            {
               std::vector<std::wstring> dirs;
               WFileName::GetDirectoriesOfPath(L"test1/test2/test3/text.txt", dirs);
               JFUNIT_ASSERT(dirs.size() == 4);
               JFUNIT_ASSERT(dirs[0] == L"test1");
               JFUNIT_ASSERT(dirs[1] == L"test2");
               JFUNIT_ASSERT(dirs[2] == L"test3");
               JFUNIT_ASSERT(dirs[3] == L"text.txt");
            }
         }
      };

      class GetExtension: public jf::unittest::TestCase
      {
      public:
         GetExtension()
            :jf::unittest::TestCase("TC::Tests::GetExtension")
         {
         }

         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::GetExtension(L"test.txt") == L"txt");
            JFUNIT_ASSERT(WFileName::GetExtension(L"hallo/test.txt") == L"txt");
            JFUNIT_ASSERT(WFileName::GetExtension(L"hallo.exe/test.txt") == L"txt");
            JFUNIT_ASSERT(WFileName::GetExtension(L"hallo.exe/test") == L"");
            JFUNIT_ASSERT(WFileName::GetExtension(L"hallo/test") == L"");
         }
      };

      class GetExtensionIfExtension: public jf::unittest::TestCase
      {
      public:
         GetExtensionIfExtension()
            :jf::unittest::TestCase("TC::Tests::GetExtensionIfExtension")
         {
         }

         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"test.txt", L"gz") == L"");
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo/test.txt", L"gz") == L"");
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo.exe/test.txt", L"gz") == L"");
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo.exe/test", L"gz") == L"");
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo/test", L"gz") == L"");

            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"test.txt.gz", L"gz") == L"txt");
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo/test.txt.gz", L"gz") == L"txt");
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo.exe/test.txt.gz", L"gz") == L"txt");
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo.exe/test.gz", L"gz") == L"");
            JFUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo/test.gz", L"gz") == L"");
         }
      };

      class GetName: public jf::unittest::TestCase
      {
      public:
         GetName()
            :jf::unittest::TestCase("TC::Tests::GetName")
         {
         }

         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::GetName(L"test.txt") == L"test.txt");
            JFUNIT_ASSERT(WFileName::GetName(L"hallo/test.txt") == L"test.txt");
            JFUNIT_ASSERT(WFileName::GetName(L"/1/2/3/test.txt") == L"test.txt");
            JFUNIT_ASSERT(WFileName::GetName(L"/1/2/3/test.txt/") == L"");
            JFUNIT_ASSERT(WFileName::GetName(L"1/2/3/") == L"");
         }
      };
      class GetPath: public jf::unittest::TestCase
      {
      public:
         GetPath()
            :jf::unittest::TestCase("TC::Tests::GetPath")
         {
         }

         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::GetPath(L"test.txt") == L"");
            JFUNIT_ASSERT(WFileName::GetPath(L"hallo/test.txt") == L"hallo/");
            JFUNIT_ASSERT(WFileName::GetPath(L"/1/2/3/test.txt") == L"/1/2/3/");
            JFUNIT_ASSERT(WFileName::GetPath(L"/1/2/3/test.txt/") == L"/1/2/3/test.txt/");
            JFUNIT_ASSERT(WFileName::GetPath(L"1/2/3/") == L"1/2/3/");
         }
      };

      class RemoveExtension: public jf::unittest::TestCase
      {
      public:
         RemoveExtension()
            :jf::unittest::TestCase("TC::Tests::RemoveExtension")
         {
         }

         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::RemoveExtension(L"test.txt") == L"test");
            JFUNIT_ASSERT(WFileName::RemoveExtension(L"hallo/test.txt") == L"hallo/test");
            JFUNIT_ASSERT(WFileName::RemoveExtension(L"hallo/test.txt/") == L"hallo/test.txt/");
         }
      };

      class Simplify: public jf::unittest::TestCase
      {
      public:
         Simplify()
            :jf::unittest::TestCase("TC::Tests::Simplify")
         {
         }

         virtual void run()
         {
            JFUNIT_ASSERT(WFileName::Simplify(L"/aa/bb/../cc") == L"/aa/cc");
            JFUNIT_ASSERT(WFileName::Simplify(L"/aa/bb/../cc/") == L"/aa/cc/");
            JFUNIT_ASSERT(WFileName::Simplify(L"/aa/bb/../..") == L"/");
            JFUNIT_ASSERT(WFileName::Simplify(L"../../bb") == L"../../bb");
            JFUNIT_ASSERT(WFileName::Simplify(L"../../bb/") == L"../../bb/");
            JFUNIT_ASSERT(WFileName::Simplify(L"/../") == L"/");
            JFUNIT_ASSERT(WFileName::Simplify(L"./aa/bb/../../") == L"./");
            JFUNIT_ASSERT(WFileName::Simplify(L"a/..") == L".");
            JFUNIT_ASSERT(WFileName::Simplify(L"a/../") == L"./");
            JFUNIT_ASSERT(WFileName::Simplify(L"./a") == L"./a");
#ifdef TCOS_WINDOWS
            JFUNIT_ASSERT(WFileName::Simplify(L"/////./././") == L"//");
            JFUNIT_ASSERT(WFileName::Simplify(L"c:/../") == L"c:/");
            JFUNIT_ASSERT(WFileName::Simplify(L"c:a/..") == L"c:");
#else
            JFUNIT_ASSERT(WFileName::Simplify(L"/////./././") == L"/");
#endif
            JFUNIT_ASSERT(WFileName::Simplify(L"/.") == L"/");
         }
      };
   }

   namespace Tests
   {
      WFileNameTestSuite::WFileNameTestSuite()
         :Unit::TestSuite("TC::Tests::WFileNameTestSuite")
      {
         add_test(new AddFileNameAndExtension);
         add_test(new AddFileNameAndPath);
         add_test(new AddPaths);
         add_test(new GetDirectories);
         add_test(new GetExtension);
         add_test(new GetExtensionIfExtension);
         add_test(new GetName);
         add_test(new GetPath);
         add_test(new RemoveExtension);
         add_test(new Simplify);
      }
   }
}

#endif
