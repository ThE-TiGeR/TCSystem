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
            TCUNIT_ASSERT(FileName::AddFileNameAndExtension("hallo", "txt") == "hallo.txt");
            TCUNIT_ASSERT(FileName::AddFileNameAndExtension("hallo", "") == "hallo");
            TCUNIT_ASSERT(FileName::AddFileNameAndExtension("", "") == "");
            TCUNIT_ASSERT(FileName::AddFileNameAndExtension("", "txt") == ".txt");
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
            TCUNIT_ASSERT(FileName::AddFileNameAndPath("hallo.txt", "test") == "test/hallo.txt");
            TCUNIT_ASSERT(FileName::AddFileNameAndPath("hallo.txt", "") == "hallo.txt");
            TCUNIT_ASSERT(FileName::AddFileNameAndPath("hallo.txt", ".") == "./hallo.txt");
            TCUNIT_ASSERT(FileName::AddFileNameAndPath("", "test") == "test/");
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
            TCUNIT_ASSERT(FileName::AddPaths("test1", "test2") == "test1/test2");
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
               std::vector<std::string> dirs;
               FileName::GetDirectoriesOfFileName("test1/test2/test3/text.txt", dirs);
               TCUNIT_ASSERT(dirs.size() == 3);
               TCUNIT_ASSERT(dirs[0] == "test1");
               TCUNIT_ASSERT(dirs[1] == "test2");
               TCUNIT_ASSERT(dirs[2] == "test3");
            }

            {
               std::vector<std::string> dirs;
               FileName::GetDirectoriesOfPath("test1/test2/test3/text.txt", dirs);
               TCUNIT_ASSERT(dirs.size() == 4);
               TCUNIT_ASSERT(dirs[0] == "test1");
               TCUNIT_ASSERT(dirs[1] == "test2");
               TCUNIT_ASSERT(dirs[2] == "test3");
               TCUNIT_ASSERT(dirs[3] == "text.txt");
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
            TCUNIT_ASSERT(FileName::GetExtension("test.txt") == "txt");
            TCUNIT_ASSERT(FileName::GetExtension("hallo/test.txt") == "txt");
            TCUNIT_ASSERT(FileName::GetExtension("hallo.exe/test.txt") == "txt");
            TCUNIT_ASSERT(FileName::GetExtension("hallo.exe/test") == "");
            TCUNIT_ASSERT(FileName::GetExtension("hallo/test") == "");
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
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("test.txt", "gz") == "");
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("hallo/test.txt", "gz") == "");
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("hallo.exe/test.txt", "gz") == "");
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("hallo.exe/test", "gz") == "");
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("hallo/test", "gz") == "");

            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("test.txt.gz", "gz") == "txt");
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("hallo/test.txt.gz", "gz") == "txt");
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("hallo.exe/test.txt.gz", "gz") == "txt");
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("hallo.exe/test.gz", "gz") == "");
            TCUNIT_ASSERT(FileName::GetExtensionIfExtension("hallo/test.gz", "gz") == "");
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
            TCUNIT_ASSERT(FileName::GetName("test.txt") == "test.txt");
            TCUNIT_ASSERT(FileName::GetName("hallo/test.txt") == "test.txt");
            TCUNIT_ASSERT(FileName::GetName("/1/2/3/test.txt") == "test.txt");
            TCUNIT_ASSERT(FileName::GetName("/1/2/3/test.txt/") == "");
            TCUNIT_ASSERT(FileName::GetName("1/2/3/") == "");
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
            TCUNIT_ASSERT(FileName::GetPath("test.txt") == "");
            TCUNIT_ASSERT(FileName::GetPath("hallo/test.txt") == "hallo/");
            TCUNIT_ASSERT(FileName::GetPath("/1/2/3/test.txt") == "/1/2/3/");
            TCUNIT_ASSERT(FileName::GetPath("/1/2/3/test.txt/") == "/1/2/3/test.txt/");
            TCUNIT_ASSERT(FileName::GetPath("1/2/3/") == "1/2/3/");
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
            TCUNIT_ASSERT(FileName::RemoveExtension("test.txt") == "test");
            TCUNIT_ASSERT(FileName::RemoveExtension("hallo/test.txt") == "hallo/test");
            TCUNIT_ASSERT(FileName::RemoveExtension("hallo/test.txt/") == "hallo/test.txt/");
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
            TCUNIT_ASSERT(FileName::Simplify("/aa/bb/../cc") == "/aa/cc");
            TCUNIT_ASSERT(FileName::Simplify("/aa/bb/../cc/") == "/aa/cc/");
            TCUNIT_ASSERT(FileName::Simplify("/aa/bb/../..") == "/");
            TCUNIT_ASSERT(FileName::Simplify("../../bb") == "../../bb");
            TCUNIT_ASSERT(FileName::Simplify("../../bb/") == "../../bb/");
            TCUNIT_ASSERT(FileName::Simplify("/../") == "/");
            TCUNIT_ASSERT(FileName::Simplify("./aa/bb/../../") == "./");
            TCUNIT_ASSERT(FileName::Simplify("a/..") == ".");
            TCUNIT_ASSERT(FileName::Simplify("a/../") == "./");
            TCUNIT_ASSERT(FileName::Simplify("./a") == "./a");
#ifdef TCOS_WINDOWS
            TCUNIT_ASSERT(FileName::Simplify("/////./././") == "//");
            TCUNIT_ASSERT(FileName::Simplify("c:/../") == "c:/");
            TCUNIT_ASSERT(FileName::Simplify("c:a/..") == "c:");
#else
            TCUNIT_ASSERT(FileName::Simplify("/////./././") == "/");
#endif
            TCUNIT_ASSERT(FileName::Simplify("/.") == "/");
         }
      };
   }

   namespace Tests
   {

      FileNameTestSuite::FileNameTestSuite()
         :Unit::TestSuite("TC::Tests::FileNameTestSuite")
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
