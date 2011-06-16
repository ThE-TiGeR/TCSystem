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

namespace TC
{
   namespace Math
   {
      namespace Tests
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

         class CoordNConstructTest: public Unit::TestCase
         {
         public:
            CoordNConstructTest()
               :Unit::TestCase("TC::Math::Tests::CoordNCunstructTest")
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

         class CoordNAssignTest: public Unit::TestCase
         {
         public:
            CoordNAssignTest()
               :Unit::TestCase("TC::Math::Tests::CoordNAssignTest")
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

         class CoordNAddAssignTest: public Unit::TestCase
         {
         public:
            CoordNAddAssignTest()
               :Unit::TestCase("TC::Math::Tests::CoordNAddAssignTest")
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

         class CoordNSubtractAssignTest: public Unit::TestCase
         {
         public:
            CoordNSubtractAssignTest()
               :Unit::TestCase("TC::Math::Tests::CoordNSubtractAssignTest")
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


         class CoordNMultiplyAssignTest: public Unit::TestCase
         {
         public:
            CoordNMultiplyAssignTest()
               :Unit::TestCase("TC::Math::Tests::CoordNMultiplyAssignTest")
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

         class CoordNDivideAssignTest: public Unit::TestCase
         {
         public:
            CoordNDivideAssignTest()
               :Unit::TestCase("TC::Math::Tests::CoordNDivideAssignTest")
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

         class CoordNEqualTest: public Unit::TestCase
         {
         public:
            CoordNEqualTest()
               :Unit::TestCase("TC::Math::Tests::CoordNEqualTest")
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

         class CoordNLessTest: public Unit::TestCase
         {
         public:
            CoordNLessTest()
               :Unit::TestCase("TC::Math::Tests::CoordNLessTest")
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
            :Unit::TestSuite("TC::Math::Tests::CoordNSuite")
         {
            AddTest(Unit::Test::Ptr(new CoordNConstructTest));
            AddTest(Unit::Test::Ptr(new CoordNAssignTest));
            AddTest(Unit::Test::Ptr(new CoordNAddAssignTest));
            AddTest(Unit::Test::Ptr(new CoordNSubtractAssignTest));
            AddTest(Unit::Test::Ptr(new CoordNMultiplyAssignTest));
            AddTest(Unit::Test::Ptr(new CoordNDivideAssignTest));
            AddTest(Unit::Test::Ptr(new CoordNEqualTest));
            AddTest(Unit::Test::Ptr(new CoordNLessTest));
         }
      }
   }
}

#endif
