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

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace
   {
      class AddFileNameAndExtension: public Unit::TestCase
      {
      public:
         AddFileNameAndExtension()
            :Unit::TestCase("TC::Tests::AddFileNameAndExtension")
         {
         }
         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::AddFileNameAndExtension(L"hallo", L"txt") == L"hallo.txt");
            TCUNIT_ASSERT(WFileName::AddFileNameAndExtension(L"hallo", L"") == L"hallo");
            TCUNIT_ASSERT(WFileName::AddFileNameAndExtension(L"", L"") == L"");
            TCUNIT_ASSERT(WFileName::AddFileNameAndExtension(L"", L"txt") == L".txt");
         }
      };

      class AddFileNameAndPath: public Unit::TestCase
      {
      public:
         AddFileNameAndPath()
            :Unit::TestCase("TC::Tests::AddFileNameAndPath")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::AddFileNameAndPath(L"hallo.txt", L"test") == L"test/hallo.txt");
            TCUNIT_ASSERT(WFileName::AddFileNameAndPath(L"hallo.txt", L"") == L"hallo.txt");
            TCUNIT_ASSERT(WFileName::AddFileNameAndPath(L"hallo.txt", L".") == L"./hallo.txt");
            TCUNIT_ASSERT(WFileName::AddFileNameAndPath(L"", L"test") == L"test/");
         }
      };

      class AddPaths: public Unit::TestCase
      {
      public:
         AddPaths()
            :Unit::TestCase("TC::Tests::AddPaths")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::AddPaths(L"test1", L"test2") == L"test1/test2");
         }
      };

      class GetDirectories: public Unit::TestCase
      {
      public:
         GetDirectories()
            :Unit::TestCase("TC::Tests::GetDirectories")
         {
         }

         virtual void Execute()
         {
            {
               std::vector<std::wstring> dirs;
               WFileName::GetDirectoriesOfFileName(L"test1/test2/test3/text.txt", dirs);
               TCUNIT_ASSERT(dirs.size() == 3);
               TCUNIT_ASSERT(dirs[0] == L"test1");
               TCUNIT_ASSERT(dirs[1] == L"test2");
               TCUNIT_ASSERT(dirs[2] == L"test3");
            }

            {
               std::vector<std::wstring> dirs;
               WFileName::GetDirectoriesOfPath(L"test1/test2/test3/text.txt", dirs);
               TCUNIT_ASSERT(dirs.size() == 4);
               TCUNIT_ASSERT(dirs[0] == L"test1");
               TCUNIT_ASSERT(dirs[1] == L"test2");
               TCUNIT_ASSERT(dirs[2] == L"test3");
               TCUNIT_ASSERT(dirs[3] == L"text.txt");
            }
         }
      };

      class GetExtension: public Unit::TestCase
      {
      public:
         GetExtension()
            :Unit::TestCase("TC::Tests::GetExtension")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::GetExtension(L"test.txt") == L"txt");
            TCUNIT_ASSERT(WFileName::GetExtension(L"hallo/test.txt") == L"txt");
            TCUNIT_ASSERT(WFileName::GetExtension(L"hallo.exe/test.txt") == L"txt");
            TCUNIT_ASSERT(WFileName::GetExtension(L"hallo.exe/test") == L"");
            TCUNIT_ASSERT(WFileName::GetExtension(L"hallo/test") == L"");
         }
      };

      class GetExtensionIfExtension: public Unit::TestCase
      {
      public:
         GetExtensionIfExtension()
            :Unit::TestCase("TC::Tests::GetExtensionIfExtension")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"test.txt", L"gz") == L"");
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo/test.txt", L"gz") == L"");
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo.exe/test.txt", L"gz") == L"");
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo.exe/test", L"gz") == L"");
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo/test", L"gz") == L"");

            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"test.txt.gz", L"gz") == L"txt");
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo/test.txt.gz", L"gz") == L"txt");
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo.exe/test.txt.gz", L"gz") == L"txt");
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo.exe/test.gz", L"gz") == L"");
            TCUNIT_ASSERT(WFileName::GetExtensionIfExtension(L"hallo/test.gz", L"gz") == L"");
         }
      };

      class GetName: public Unit::TestCase
      {
      public:
         GetName()
            :Unit::TestCase("TC::Tests::GetName")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::GetName(L"test.txt") == L"test.txt");
            TCUNIT_ASSERT(WFileName::GetName(L"hallo/test.txt") == L"test.txt");
            TCUNIT_ASSERT(WFileName::GetName(L"/1/2/3/test.txt") == L"test.txt");
            TCUNIT_ASSERT(WFileName::GetName(L"/1/2/3/test.txt/") == L"");
            TCUNIT_ASSERT(WFileName::GetName(L"1/2/3/") == L"");
         }
      };
      class GetPath: public Unit::TestCase
      {
      public:
         GetPath()
            :Unit::TestCase("TC::Tests::GetPath")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::GetPath(L"test.txt") == L"");
            TCUNIT_ASSERT(WFileName::GetPath(L"hallo/test.txt") == L"hallo/");
            TCUNIT_ASSERT(WFileName::GetPath(L"/1/2/3/test.txt") == L"/1/2/3/");
            TCUNIT_ASSERT(WFileName::GetPath(L"/1/2/3/test.txt/") == L"/1/2/3/test.txt/");
            TCUNIT_ASSERT(WFileName::GetPath(L"1/2/3/") == L"1/2/3/");
         }
      };

      class RemoveExtension: public Unit::TestCase
      {
      public:
         RemoveExtension()
            :Unit::TestCase("TC::Tests::RemoveExtension")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::RemoveExtension(L"test.txt") == L"test");
            TCUNIT_ASSERT(WFileName::RemoveExtension(L"hallo/test.txt") == L"hallo/test");
            TCUNIT_ASSERT(WFileName::RemoveExtension(L"hallo/test.txt/") == L"hallo/test.txt/");
         }
      };

      class Simplify: public Unit::TestCase
      {
      public:
         Simplify()
            :Unit::TestCase("TC::Tests::Simplify")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(WFileName::Simplify(L"/aa/bb/../cc") == L"/aa/cc");
            TCUNIT_ASSERT(WFileName::Simplify(L"/aa/bb/../cc/") == L"/aa/cc/");
            TCUNIT_ASSERT(WFileName::Simplify(L"/aa/bb/../..") == L"/");
            TCUNIT_ASSERT(WFileName::Simplify(L"../../bb") == L"../../bb");
            TCUNIT_ASSERT(WFileName::Simplify(L"../../bb/") == L"../../bb/");
            TCUNIT_ASSERT(WFileName::Simplify(L"/../") == L"/");
            TCUNIT_ASSERT(WFileName::Simplify(L"./aa/bb/../../") == L"./");
            TCUNIT_ASSERT(WFileName::Simplify(L"a/..") == L".");
            TCUNIT_ASSERT(WFileName::Simplify(L"a/../") == L"./");
            TCUNIT_ASSERT(WFileName::Simplify(L"./a") == L"./a");
#ifdef TCOS_WINDOWS
            TCUNIT_ASSERT(WFileName::Simplify(L"/////./././") == L"//");
            TCUNIT_ASSERT(WFileName::Simplify(L"c:/../") == L"c:/");
            TCUNIT_ASSERT(WFileName::Simplify(L"c:a/..") == L"c:");
#else
            TCUNIT_ASSERT(WFileName::Simplify(L"/////./././") == L"/");
#endif
            TCUNIT_ASSERT(WFileName::Simplify(L"/.") == L"/");
         }
      };
   }

   namespace Tests
   {
      WFileNameTestSuite::WFileNameTestSuite()
         :Unit::TestSuite("TC::Tests::WFileNameTestSuite")
      {
         AddTest(new AddFileNameAndExtension);
         AddTest(new AddFileNameAndPath);
         AddTest(new AddPaths);
         AddTest(new GetDirectories);
         AddTest(new GetExtension);
         AddTest(new GetExtensionIfExtension);
         AddTest(new GetName);
         AddTest(new GetPath);
         AddTest(new RemoveExtension);
         AddTest(new Simplify);
      }
   }
}

#endif
