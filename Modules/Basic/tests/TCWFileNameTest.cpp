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

#include "TCWFileNameTest.h"

#include "TCWFileName.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace
   {
      class AddFileNameAndExtension: public unit::TestCase
      {
      public:
         AddFileNameAndExtension()
            :unit::TestCase("tc::tests::AddFileNameAndExtension")
         {
         }
         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::AddFileNameAndExtension(L"hallo", L"txt") == L"hallo.txt");
            TCUNIT_ASSERT(wfile_name::AddFileNameAndExtension(L"hallo", L"") == L"hallo");
            TCUNIT_ASSERT(wfile_name::AddFileNameAndExtension(L"", L"") == L"");
            TCUNIT_ASSERT(wfile_name::AddFileNameAndExtension(L"", L"txt") == L".txt");
         }
      };

      class AddFileNameAndPath: public unit::TestCase
      {
      public:
         AddFileNameAndPath()
            :unit::TestCase("tc::tests::AddFileNameAndPath")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::AddFileNameAndPath(L"hallo.txt", L"test") == L"test/hallo.txt");
            TCUNIT_ASSERT(wfile_name::AddFileNameAndPath(L"hallo.txt", L"") == L"hallo.txt");
            TCUNIT_ASSERT(wfile_name::AddFileNameAndPath(L"hallo.txt", L".") == L"./hallo.txt");
            TCUNIT_ASSERT(wfile_name::AddFileNameAndPath(L"", L"test") == L"test/");
         }
      };

      class AddPaths: public unit::TestCase
      {
      public:
         AddPaths()
            :unit::TestCase("tc::tests::AddPaths")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::AddPaths(L"test1", L"test2") == L"test1/test2");
         }
      };

      class GetDirectories: public unit::TestCase
      {
      public:
         GetDirectories()
            :unit::TestCase("tc::tests::GetDirectories")
         {
         }

         virtual void Execute()
         {
            {
               std::vector<std::wstring> dirs;
               wfile_name::GetDirectoriesOfFileName(L"test1/test2/test3/text.txt", dirs);
               TCUNIT_ASSERT(dirs.size() == 3);
               TCUNIT_ASSERT(dirs[0] == L"test1");
               TCUNIT_ASSERT(dirs[1] == L"test2");
               TCUNIT_ASSERT(dirs[2] == L"test3");
            }

            {
               std::vector<std::wstring> dirs;
               wfile_name::GetDirectoriesOfPath(L"test1/test2/test3/text.txt", dirs);
               TCUNIT_ASSERT(dirs.size() == 4);
               TCUNIT_ASSERT(dirs[0] == L"test1");
               TCUNIT_ASSERT(dirs[1] == L"test2");
               TCUNIT_ASSERT(dirs[2] == L"test3");
               TCUNIT_ASSERT(dirs[3] == L"text.txt");
            }
         }
      };

      class GetExtension: public unit::TestCase
      {
      public:
         GetExtension()
            :unit::TestCase("tc::tests::GetExtension")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::GetExtension(L"test.txt") == L"txt");
            TCUNIT_ASSERT(wfile_name::GetExtension(L"hallo/test.txt") == L"txt");
            TCUNIT_ASSERT(wfile_name::GetExtension(L"hallo.exe/test.txt") == L"txt");
            TCUNIT_ASSERT(wfile_name::GetExtension(L"hallo.exe/test") == L"");
            TCUNIT_ASSERT(wfile_name::GetExtension(L"hallo/test") == L"");
         }
      };

      class GetExtensionIfExtension: public unit::TestCase
      {
      public:
         GetExtensionIfExtension()
            :unit::TestCase("tc::tests::GetExtensionIfExtension")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"test.txt", L"gz") == L"");
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"hallo/test.txt", L"gz") == L"");
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"hallo.exe/test.txt", L"gz") == L"");
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"hallo.exe/test", L"gz") == L"");
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"hallo/test", L"gz") == L"");

            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"test.txt.gz", L"gz") == L"txt");
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"hallo/test.txt.gz", L"gz") == L"txt");
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"hallo.exe/test.txt.gz", L"gz") == L"txt");
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"hallo.exe/test.gz", L"gz") == L"");
            TCUNIT_ASSERT(wfile_name::GetExtensionIfExtension(L"hallo/test.gz", L"gz") == L"");
         }
      };

      class GetName: public unit::TestCase
      {
      public:
         GetName()
            :unit::TestCase("tc::tests::GetName")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::GetName(L"test.txt") == L"test.txt");
            TCUNIT_ASSERT(wfile_name::GetName(L"hallo/test.txt") == L"test.txt");
            TCUNIT_ASSERT(wfile_name::GetName(L"/1/2/3/test.txt") == L"test.txt");
            TCUNIT_ASSERT(wfile_name::GetName(L"/1/2/3/test.txt/") == L"");
            TCUNIT_ASSERT(wfile_name::GetName(L"1/2/3/") == L"");
         }
      };
      class GetPath: public unit::TestCase
      {
      public:
         GetPath()
            :unit::TestCase("tc::tests::GetPath")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::GetPath(L"test.txt") == L"");
            TCUNIT_ASSERT(wfile_name::GetPath(L"hallo/test.txt") == L"hallo/");
            TCUNIT_ASSERT(wfile_name::GetPath(L"/1/2/3/test.txt") == L"/1/2/3/");
            TCUNIT_ASSERT(wfile_name::GetPath(L"/1/2/3/test.txt/") == L"/1/2/3/test.txt/");
            TCUNIT_ASSERT(wfile_name::GetPath(L"1/2/3/") == L"1/2/3/");
         }
      };

      class RemoveExtension: public unit::TestCase
      {
      public:
         RemoveExtension()
            :unit::TestCase("tc::tests::RemoveExtension")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::RemoveExtension(L"test.txt") == L"test");
            TCUNIT_ASSERT(wfile_name::RemoveExtension(L"hallo/test.txt") == L"hallo/test");
            TCUNIT_ASSERT(wfile_name::RemoveExtension(L"hallo/test.txt/") == L"hallo/test.txt/");
         }
      };

      class Simplify: public unit::TestCase
      {
      public:
         Simplify()
            :unit::TestCase("tc::tests::Simplify")
         {
         }

         virtual void Execute()
         {
            TCUNIT_ASSERT(wfile_name::Simplify(L"/aa/bb/../cc") == L"/aa/cc");
            TCUNIT_ASSERT(wfile_name::Simplify(L"/aa/bb/../cc/") == L"/aa/cc/");
            TCUNIT_ASSERT(wfile_name::Simplify(L"/aa/bb/../..") == L"/");
            TCUNIT_ASSERT(wfile_name::Simplify(L"../../bb") == L"../../bb");
            TCUNIT_ASSERT(wfile_name::Simplify(L"../../bb/") == L"../../bb/");
            TCUNIT_ASSERT(wfile_name::Simplify(L"/../") == L"/");
            TCUNIT_ASSERT(wfile_name::Simplify(L"./aa/bb/../../") == L"./");
            TCUNIT_ASSERT(wfile_name::Simplify(L"a/..") == L".");
            TCUNIT_ASSERT(wfile_name::Simplify(L"a/../") == L"./");
            TCUNIT_ASSERT(wfile_name::Simplify(L"./a") == L"./a");
#ifdef TCOS_WINDOWS
            TCUNIT_ASSERT(wfile_name::Simplify(L"/////./././") == L"//");
            TCUNIT_ASSERT(wfile_name::Simplify(L"c:/../") == L"c:/");
            TCUNIT_ASSERT(wfile_name::Simplify(L"c:a/..") == L"c:");
#else
            TCUNIT_ASSERT(wfile_name::Simplify(L"/////./././") == L"/");
#endif
            TCUNIT_ASSERT(wfile_name::Simplify(L"/.") == L"/");
         }
      };
   }

   namespace tests
   {
      WFileNameTestSuite::WFileNameTestSuite()
         :unit::TestSuite("tc::tests::WFileNameTestSuite")
      {
         AddTest(unit::Test::Ptr(new AddFileNameAndExtension));
         AddTest(unit::Test::Ptr(new AddFileNameAndPath));
         AddTest(unit::Test::Ptr(new AddPaths));
         AddTest(unit::Test::Ptr(new GetDirectories));
         AddTest(unit::Test::Ptr(new GetExtension));
         AddTest(unit::Test::Ptr(new GetExtensionIfExtension));
         AddTest(unit::Test::Ptr(new GetName));
         AddTest(unit::Test::Ptr(new GetPath));
         AddTest(unit::Test::Ptr(new RemoveExtension));
         AddTest(unit::Test::Ptr(new Simplify));
      }
   }
}

#endif
