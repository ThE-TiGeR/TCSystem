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
            JFUNIT_ASSERT(FileName::AddFileNameAndExtension("hallo", "txt") == "hallo.txt");
            JFUNIT_ASSERT(FileName::AddFileNameAndExtension("hallo", "") == "hallo");
            JFUNIT_ASSERT(FileName::AddFileNameAndExtension("", "") == "");
            JFUNIT_ASSERT(FileName::AddFileNameAndExtension("", "txt") == ".txt");
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
            JFUNIT_ASSERT(FileName::AddFileNameAndPath("hallo.txt", "test") == "test/hallo.txt");
            JFUNIT_ASSERT(FileName::AddFileNameAndPath("hallo.txt", "") == "hallo.txt");
            JFUNIT_ASSERT(FileName::AddFileNameAndPath("hallo.txt", ".") == "./hallo.txt");
            JFUNIT_ASSERT(FileName::AddFileNameAndPath("", "test") == "test/");
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
            JFUNIT_ASSERT(FileName::AddPaths("test1", "test2") == "test1/test2");
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
               std::vector<std::string> dirs;
               FileName::GetDirectoriesOfFileName("test1/test2/test3/text.txt", dirs);
               JFUNIT_ASSERT(dirs.size() == 3);
               JFUNIT_ASSERT(dirs[0] == "test1");
               JFUNIT_ASSERT(dirs[1] == "test2");
               JFUNIT_ASSERT(dirs[2] == "test3");
            }

            {
               std::vector<std::string> dirs;
               FileName::GetDirectoriesOfPath("test1/test2/test3/text.txt", dirs);
               JFUNIT_ASSERT(dirs.size() == 4);
               JFUNIT_ASSERT(dirs[0] == "test1");
               JFUNIT_ASSERT(dirs[1] == "test2");
               JFUNIT_ASSERT(dirs[2] == "test3");
               JFUNIT_ASSERT(dirs[3] == "text.txt");
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
            JFUNIT_ASSERT(FileName::GetExtension("test.txt") == "txt");
            JFUNIT_ASSERT(FileName::GetExtension("hallo/test.txt") == "txt");
            JFUNIT_ASSERT(FileName::GetExtension("hallo.exe/test.txt") == "txt");
            JFUNIT_ASSERT(FileName::GetExtension("hallo.exe/test") == "");
            JFUNIT_ASSERT(FileName::GetExtension("hallo/test") == "");
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
            JFUNIT_ASSERT(FileName::GetName("test.txt") == "test.txt");
            JFUNIT_ASSERT(FileName::GetName("hallo/test.txt") == "test.txt");
            JFUNIT_ASSERT(FileName::GetName("/1/2/3/test.txt") == "test.txt");
            JFUNIT_ASSERT(FileName::GetName("/1/2/3/test.txt/") == "");
            JFUNIT_ASSERT(FileName::GetName("1/2/3/") == "");
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
            JFUNIT_ASSERT(FileName::GetPath("test.txt") == "");
            JFUNIT_ASSERT(FileName::GetPath("hallo/test.txt") == "hallo/");
            JFUNIT_ASSERT(FileName::GetPath("/1/2/3/test.txt") == "/1/2/3/");
            JFUNIT_ASSERT(FileName::GetPath("/1/2/3/test.txt/") == "/1/2/3/test.txt/");
            JFUNIT_ASSERT(FileName::GetPath("1/2/3/") == "1/2/3/");
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
            JFUNIT_ASSERT(FileName::RemoveExtension("test.txt") == "test");
            JFUNIT_ASSERT(FileName::RemoveExtension("hallo/test.txt") == "hallo/test");
            JFUNIT_ASSERT(FileName::RemoveExtension("hallo/test.txt/") == "hallo/test.txt/");
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
            JFUNIT_ASSERT(FileName::Simplify("/aa/bb/../cc") == "/aa/cc");
            JFUNIT_ASSERT(FileName::Simplify("/aa/bb/../cc/") == "/aa/cc/");
            JFUNIT_ASSERT(FileName::Simplify("/aa/bb/../..") == "/");
            JFUNIT_ASSERT(FileName::Simplify("../../bb") == "../../bb");
            JFUNIT_ASSERT(FileName::Simplify("../../bb/") == "../../bb/");
            JFUNIT_ASSERT(FileName::Simplify("/../") == "/");
            JFUNIT_ASSERT(FileName::Simplify("./aa/bb/../../") == "./");
            JFUNIT_ASSERT(FileName::Simplify("a/..") == ".");
            JFUNIT_ASSERT(FileName::Simplify("a/../") == "./");
            JFUNIT_ASSERT(FileName::Simplify("./a") == "./a");
#ifdef TCOS_WINDOWS
            JFUNIT_ASSERT(FileName::Simplify("/////./././") == "//");
            JFUNIT_ASSERT(FileName::Simplify("c:/../") == "c:/");
            JFUNIT_ASSERT(FileName::Simplify("c:a/..") == "c:");
#else
            JFUNIT_ASSERT(FileName::Simplify("/////./././") == "/");
#endif
            JFUNIT_ASSERT(FileName::Simplify("/.") == "/");
         }
      };
   }

   namespace Tests
   {

      FileNameTestSuite::FileNameTestSuite()
         :jf::unittest::TestSuite("TC::Tests::FileNameTestSuite")
      {
         add_test(new AddFileNameAndExtension);
         add_test(new AddFileNameAndPath);
         add_test(new AddPaths);
         add_test(new GetDirectories);
         add_test(new GetExtension);
         add_test(new GetName);
         add_test(new GetPath);
         add_test(new RemoveExtension);
         add_test(new Simplify);
      }
   }
}

#endif
