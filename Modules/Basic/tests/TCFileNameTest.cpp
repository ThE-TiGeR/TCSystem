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
#include "TCFileNameTest.h"

#include "TCFileName.h"

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
            TCUNIT_ASSERT(file_name::AddFileNameAndExtension("hallo", "txt") == "hallo.txt");
            TCUNIT_ASSERT(file_name::AddFileNameAndExtension("hallo", "") == "hallo");
            TCUNIT_ASSERT(file_name::AddFileNameAndExtension("", "") == "");
            TCUNIT_ASSERT(file_name::AddFileNameAndExtension("", "txt") == ".txt");
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
            TCUNIT_ASSERT(file_name::AddFileNameAndPath("hallo.txt", "test") == "test/hallo.txt");
            TCUNIT_ASSERT(file_name::AddFileNameAndPath("hallo.txt", "") == "hallo.txt");
            TCUNIT_ASSERT(file_name::AddFileNameAndPath("hallo.txt", ".") == "./hallo.txt");
            TCUNIT_ASSERT(file_name::AddFileNameAndPath("", "test") == "test/");
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
            TCUNIT_ASSERT(file_name::AddPaths("test1", "test2") == "test1/test2");
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
               std::vector<std::string> dirs;
               file_name::GetDirectoriesOfFileName("test1/test2/test3/text.txt", dirs);
               TCUNIT_ASSERT(dirs.size() == 3);
               TCUNIT_ASSERT(dirs[0] == "test1");
               TCUNIT_ASSERT(dirs[1] == "test2");
               TCUNIT_ASSERT(dirs[2] == "test3");
            }

            {
               std::vector<std::string> dirs;
               file_name::GetDirectoriesOfPath("test1/test2/test3/text.txt", dirs);
               TCUNIT_ASSERT(dirs.size() == 4);
               TCUNIT_ASSERT(dirs[0] == "test1");
               TCUNIT_ASSERT(dirs[1] == "test2");
               TCUNIT_ASSERT(dirs[2] == "test3");
               TCUNIT_ASSERT(dirs[3] == "text.txt");
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
            TCUNIT_ASSERT(file_name::GetExtension("test.txt") == "txt");
            TCUNIT_ASSERT(file_name::GetExtension("hallo/test.txt") == "txt");
            TCUNIT_ASSERT(file_name::GetExtension("hallo.exe/test.txt") == "txt");
            TCUNIT_ASSERT(file_name::GetExtension("hallo.exe/test") == "");
            TCUNIT_ASSERT(file_name::GetExtension("hallo/test") == "");
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
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("test.txt", "gz") == "");
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("hallo/test.txt", "gz") == "");
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("hallo.exe/test.txt", "gz") == "");
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("hallo.exe/test", "gz") == "");
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("hallo/test", "gz") == "");

            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("test.txt.gz", "gz") == "txt");
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("hallo/test.txt.gz", "gz") == "txt");
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("hallo.exe/test.txt.gz", "gz") == "txt");
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("hallo.exe/test.gz", "gz") == "");
            TCUNIT_ASSERT(file_name::GetExtensionIfExtension("hallo/test.gz", "gz") == "");
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
            TCUNIT_ASSERT(file_name::GetName("test.txt") == "test.txt");
            TCUNIT_ASSERT(file_name::GetName("hallo/test.txt") == "test.txt");
            TCUNIT_ASSERT(file_name::GetName("/1/2/3/test.txt") == "test.txt");
            TCUNIT_ASSERT(file_name::GetName("/1/2/3/test.txt/") == "");
            TCUNIT_ASSERT(file_name::GetName("1/2/3/") == "");
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
            TCUNIT_ASSERT(file_name::GetPath("test.txt") == "");
            TCUNIT_ASSERT(file_name::GetPath("hallo/test.txt") == "hallo/");
            TCUNIT_ASSERT(file_name::GetPath("/1/2/3/test.txt") == "/1/2/3/");
            TCUNIT_ASSERT(file_name::GetPath("/1/2/3/test.txt/") == "/1/2/3/test.txt/");
            TCUNIT_ASSERT(file_name::GetPath("1/2/3/") == "1/2/3/");
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
            TCUNIT_ASSERT(file_name::RemoveExtension("test.txt") == "test");
            TCUNIT_ASSERT(file_name::RemoveExtension("hallo/test.txt") == "hallo/test");
            TCUNIT_ASSERT(file_name::RemoveExtension("hallo/test.txt/") == "hallo/test.txt/");
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
            TCUNIT_ASSERT(file_name::Simplify("/aa/bb/../cc") == "/aa/cc");
            TCUNIT_ASSERT(file_name::Simplify("/aa/bb/../cc/") == "/aa/cc/");
            TCUNIT_ASSERT(file_name::Simplify("/aa/bb/../..") == "/");
            TCUNIT_ASSERT(file_name::Simplify("../../bb") == "../../bb");
            TCUNIT_ASSERT(file_name::Simplify("../../bb/") == "../../bb/");
            TCUNIT_ASSERT(file_name::Simplify("/../") == "/");
            TCUNIT_ASSERT(file_name::Simplify("./aa/bb/../../") == "./");
            TCUNIT_ASSERT(file_name::Simplify("a/..") == ".");
            TCUNIT_ASSERT(file_name::Simplify("a/../") == "./");
            TCUNIT_ASSERT(file_name::Simplify("./a") == "./a");
#ifdef TCOS_WINDOWS
            TCUNIT_ASSERT(file_name::Simplify("/////./././") == "//");
            TCUNIT_ASSERT(file_name::Simplify("c:/../") == "c:/");
            TCUNIT_ASSERT(file_name::Simplify("c:a/..") == "c:");
#else
            TCUNIT_ASSERT(file_name::Simplify("/////./././") == "/");
#endif
            TCUNIT_ASSERT(file_name::Simplify("/.") == "/");
         }
      };
   }

   namespace tests
   {

      FileNameTestSuite::FileNameTestSuite()
         :unit::TestSuite("tc::tests::FileNameTestSuite")
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
