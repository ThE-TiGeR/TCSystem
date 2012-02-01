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
#include "TCMathCoordNTestSuite.h"

#include "TCUnitTestCase.h"
#include "TCMathCoordN.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace math
   {
      namespace tests
      {
         typedef CoordN<int, 5> CoordType;
         static CoordType GetDefaultCoord()
         {
            CoordType val;
            for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
            {
               val[i] = i;
            }

            return val;
         }

         class CoordNConstructTest: public unit::TestCase
         {
         public:
            CoordNConstructTest()
               :unit::TestCase("tc::math::tests::CoordNCunstructTest")
            {
            }

            virtual void Execute()
            {
               {
                  CoordType val;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == 0);
                  }
               }

               {
                  CoordType val(1);
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == 1);
                  }
               }

               {
                  CoordType val(GetDefaultCoord());

                  CoordType val1(val);
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i));
                  }
               }
               {
                  CoordN<double, CoordType::NUM_COMPONENTS> val;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     val[i] = CoordN<double, CoordType::NUM_COMPONENTS>::DataType(i);
                  }

                  CoordType val1(val);
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val1[i] == CoordType::DataType(i));
                  }
               }
            }
         };

         class CoordNAssignTest: public unit::TestCase
         {
         public:
            CoordNAssignTest()
               :unit::TestCase("tc::math::tests::CoordNAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  CoordType val(GetDefaultCoord());

                  CoordType val1;
                  val1 = val;

                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val1[i] == CoordType::DataType(i));
                  }
               }

               {
                  CoordType val;
                  val = CoordType::DataType(7);

                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(7));
                  }
               }
            }
         };

         class CoordNAddAssignTest: public unit::TestCase
         {
         public:
            CoordNAddAssignTest()
               :unit::TestCase("tc::math::tests::CoordNAddAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  CoordType val1(GetDefaultCoord());

                  CoordType val;
                  val += val1;
                  val += val1;

                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(2*i));
                  }

                  val = val1 + val1 + val1;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(3*i));
                  }
               }

               {
                  CoordType val1(GetDefaultCoord());

                  CoordType val;
                  val += val1;
                  val += 7;

                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i+7));
                  }

                  val = val1 + 5;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i+5));
                  }

               }
            }
         };

         class CoordNSubtractAssignTest: public unit::TestCase
         {
         public:
            CoordNSubtractAssignTest()
               :unit::TestCase("tc::math::tests::CoordNSubtractAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  CoordType val1(GetDefaultCoord());

                  CoordType val;
                  val -= val1;
                  val -= val1;

                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(-2*i));
                  }

                  val = val1 - val1 - val1;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(-i));
                  }
               }

               {
                  CoordType val1(GetDefaultCoord());

                  CoordType val;
                  val -= val1;
                  val -= 7;

                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(-i-7));
                  }

                  val = val1 - 5;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i-5));
                  }
               }
            }
         };


         class CoordNMultiplyAssignTest: public unit::TestCase
         {
         public:
            CoordNMultiplyAssignTest()
               :unit::TestCase("tc::math::tests::CoordNMultiplyAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  CoordType val1(GetDefaultCoord());

                  CoordType val(val1);
                  val *= 7;

                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i*7));
                  }

                  val = val1 * 5;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i*5));
                  }

                  val = 3 * val1;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i*3));
                  }
               }
            }
         };

         class CoordNDivideAssignTest: public unit::TestCase
         {
         public:
            CoordNDivideAssignTest()
               :unit::TestCase("tc::math::tests::CoordNDivideAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  CoordType val1;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     val1[i] = i * 7;
                  }

                  CoordType val(val1);
                  val /= 7;

                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i));
                  }

                  val = val1 / 7;
                  for (int i=0; i<CoordType::NUM_COMPONENTS; i++)
                  {
                     TCUNIT_ASSERT(val[i] == CoordType::DataType(i));
                  }
               }
            }
         };

         class CoordNEqualTest: public unit::TestCase
         {
         public:
            CoordNEqualTest()
               :unit::TestCase("tc::math::tests::CoordNEqualTest")
            {
            }

            virtual void Execute()
            {
               {
                  CoordType val1;
                  CoordType val2;
                  TCUNIT_ASSERT(val1 == val2);
                  TCUNIT_ASSERT(!(val1 != val2));
               }

               {
                  CoordType val1(GetDefaultCoord());
                  CoordType val2(GetDefaultCoord());
                  TCUNIT_ASSERT(val1 == val2);
                  TCUNIT_ASSERT(!(val1 != val2));

                  val1[0] = 12345;
                  TCUNIT_ASSERT(val1 != val2);
                  TCUNIT_ASSERT(!(val1 == val2));
               }
            }
         };

         class CoordNLessTest: public unit::TestCase
         {
         public:
            CoordNLessTest()
               :unit::TestCase("tc::math::tests::CoordNLessTest")
            {
            }

            virtual void Execute()
            {
               {
                  CoordType val1;
                  CoordType val2;

                  TCUNIT_ASSERT(!(val1 <  val2));
                  TCUNIT_ASSERT( (val1 <= val2));
                  TCUNIT_ASSERT(!(val1 >  val2));
                  TCUNIT_ASSERT( (val1 >= val2));
               }

               {
                  CoordType val1(GetDefaultCoord());
                  CoordType val2(GetDefaultCoord());
                  val2[0] = -1;

                  TCUNIT_ASSERT(!(val1 <  val2));
                  TCUNIT_ASSERT(!(val1 <= val2));
                  TCUNIT_ASSERT( (val1 >  val2));
                  TCUNIT_ASSERT( (val1 >= val2));
               }

               {
                  CoordType val1(GetDefaultCoord());
                  CoordType val2(GetDefaultCoord());
                  val1[0] = -1;

                  TCUNIT_ASSERT( (val1 <  val2));
                  TCUNIT_ASSERT( (val1 <= val2));
                  TCUNIT_ASSERT(!(val1 >  val2));
                  TCUNIT_ASSERT(!(val1 >= val2));
               }

               {
                  CoordType val1(GetDefaultCoord());
                  CoordType val2(GetDefaultCoord());
                  val2[CoordType::NUM_COMPONENTS-1] = -1;

                  TCUNIT_ASSERT(!(val1 <  val2));
                  TCUNIT_ASSERT(!(val1 <= val2));
                  TCUNIT_ASSERT( (val1 >  val2));
                  TCUNIT_ASSERT( (val1 >= val2));
               }

               {
                  CoordType val1(GetDefaultCoord());
                  CoordType val2(GetDefaultCoord());
                  val1[CoordType::NUM_COMPONENTS-1] = -1;

                  TCUNIT_ASSERT( (val1 <  val2));
                  TCUNIT_ASSERT( (val1 <= val2));
                  TCUNIT_ASSERT(!(val1 >  val2));
                  TCUNIT_ASSERT(!(val1 >= val2));
               }
            }
         };

         CoordNSuite::CoordNSuite()
            :unit::TestSuite("tc::math::tests::CoordNSuite")
         {
            AddTest(unit::Test::Ptr(new CoordNConstructTest));
            AddTest(unit::Test::Ptr(new CoordNAssignTest));
            AddTest(unit::Test::Ptr(new CoordNAddAssignTest));
            AddTest(unit::Test::Ptr(new CoordNSubtractAssignTest));
            AddTest(unit::Test::Ptr(new CoordNMultiplyAssignTest));
            AddTest(unit::Test::Ptr(new CoordNDivideAssignTest));
            AddTest(unit::Test::Ptr(new CoordNEqualTest));
            AddTest(unit::Test::Ptr(new CoordNLessTest));
         }
      }
   }
}

#endif
