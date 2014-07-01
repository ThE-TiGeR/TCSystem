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
#include "TCNetAddressTest.h"

#include "TCNetAddress.h"

#include "TCUnitTestCase.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace net
   {
      namespace
      {
         class AddressConstsTest: public unit::TestCase
         {
         public:
            AddressConstsTest()
               :unit::TestCase("tc::net::tests::AddressConstsTest")
            {
            }

            virtual void Execute()
            {
               TCUNIT_ASSERT(Address::GetAnyAddress().GetDotNotation() == "0.0.0.0");
               TCUNIT_ASSERT(Address::GetBroadcastAddress().GetDotNotation() == "255.255.255.255");
               TCUNIT_ASSERT(Address::GetLoopBackAddress().GetDotNotation() == "127.0.0.1");
            }
         };
      }

      namespace tests
      {
         AddressTestSuite::AddressTestSuite()
            :unit::TestSuite("tc::net::tests::AddressTestSuite")
         {
            AddTest(unit::Test::Ptr(new AddressConstsTest));
         }
      }
   }

}

#endif
