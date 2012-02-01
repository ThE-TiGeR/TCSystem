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
#include "TCMathMatrixNTestSuite.h"

#include "TCUnitTestCase.h"
#include "TCMathMatrixN.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace math
   {
      namespace tests
      {
         typedef MatrixN<int, 5> MatrixType;
         static MatrixType GetDefaultMatrix()
         {
            MatrixType val;
            for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
            {
               for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
               {
                  val(i, j) = i * MatrixType::NUM_COMPONENTS + j;
               }
            }

            return val;
         }

         class MatrixNConstructTest: public unit::TestCase
         {
         public:
            MatrixNConstructTest()
               :unit::TestCase("tc::math::tests::MatrixNCunstructTest")
            {
            }

            virtual void Execute()
            {
               {
                  MatrixType val(0);
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == MatrixType::DataType(0));
                     }
                  }
               }

               {
                  MatrixType val(27);
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<i; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == MatrixType::DataType(0));
                     }

                     TCUNIT_ASSERT(val(i, i) == MatrixType::DataType(27));

                     for (int j=i+1; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == MatrixType::DataType(0));
                     }
                  }
               }

               {
                  MatrixType val(GetDefaultMatrix());

                  MatrixType val1(val);
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val(i, j));
                     }
                  }
               }

               {
                  MatrixN<double, MatrixType::NUM_COMPONENTS> val;
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        val(i, j) = MatrixN<double, MatrixType::NUM_COMPONENTS>::DataType(i);
                     }
                  }

                  MatrixType val1(val);
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val1(i, j) == MatrixType::DataType(i));
                     }
                  }
               }
            }
         };

         class MatrixNAssignTest: public unit::TestCase
         {
         public:
            MatrixNAssignTest()
               :unit::TestCase("tc::math::tests::MatrixNAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  MatrixType val(GetDefaultMatrix());

                  MatrixType val1;
                  val1 = val;

                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val1(i, j) == val(i, j));
                     }
                  }
               }

               {
                  MatrixType val;
                  val = MatrixType::DataType(27);

                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<i; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == MatrixType::DataType(0));
                     }

                     TCUNIT_ASSERT(val(i, i) == MatrixType::DataType(27));

                     for (int j=i+1; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == MatrixType::DataType(0));
                     }
                  }
               }
            }
         };

         class MatrixNAddAssignTest: public unit::TestCase
         {
         public:
            MatrixNAddAssignTest()
               :unit::TestCase("tc::math::tests::MatrixNAddAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  MatrixType val1(GetDefaultMatrix());

                  MatrixType val(0);
                  val += val1;
                  val += val1;

                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val1(i, j)*2);
                     }
                  }

                  val = val1 + val1 + val1;
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val1(i, j)*3);
                     }
                  }
               }
            }
         };

         class MatrixNSubtractAssignTest: public unit::TestCase
         {
         public:
            MatrixNSubtractAssignTest()
               :unit::TestCase("tc::math::tests::MatrixNSubtractAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  MatrixType val1(GetDefaultMatrix());

                  MatrixType val(0);
                  val -= val1;
                  val -= val1;

                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val1(i, j)*-2);
                     }
                  }

                  val = val1 - val1 - val1;
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == -val1(i, j));
                     }
                  }
               }
            }
         };

         class MatrixNMultiplyAssignTest: public unit::TestCase
         {
         public:
            MatrixNMultiplyAssignTest()
               :unit::TestCase("tc::math::tests::MatrixNMultiplyAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  MatrixType val1(GetDefaultMatrix());

                  MatrixType val(val1);
                  val *= 7;

                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val1(i, j)*7);
                     }
                  }

                  val = val1 * 5;
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val1(i, j)*5);
                     }
                  }

                  val = 3 * val1;
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val1(i, j)*3);
                     }
                  }
               }
            }
         };
         
         class MatrixNDivideAssignTest: public unit::TestCase
         {
         public:
            MatrixNDivideAssignTest()
               :unit::TestCase("tc::math::tests::MatrixNDivideAssignTest")
            {
            }

            virtual void Execute()
            {
               {
                  MatrixType val1(GetDefaultMatrix()*7);
                  MatrixType val(val1);
                  val /= 7;

                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val1(i, j)/7);
                     }
                  }

                  val = val1 / 7;
                  for (int i=0; i<MatrixType::NUM_COMPONENTS; i++)
                  {
                     for (int j=0; j<MatrixType::NUM_COMPONENTS; j++)
                     {
                        TCUNIT_ASSERT(val(i, j) == val1(i, j)/7);
                     }
                  }
               }
            }
         };
         
         class MatrixNEqualTest: public unit::TestCase
         {
         public:
            MatrixNEqualTest()
               :unit::TestCase("tc::math::tests::MatrixNEqualTest")
            {
            }

            virtual void Execute()
            {
               {
                  MatrixType val1(0);
                  MatrixType val2(0);
                  TCUNIT_ASSERT(val1 == val2);
                  TCUNIT_ASSERT(!(val1 != val2));
               }

               {
                  MatrixType val1(GetDefaultMatrix());
                  MatrixType val2(GetDefaultMatrix());
                  TCUNIT_ASSERT(val1 == val2);
                  TCUNIT_ASSERT(!(val1 != val2));

                  val1(0, 0) = 12345;
                  TCUNIT_ASSERT(val1 != val2);
                  TCUNIT_ASSERT(!(val1 == val2));
               }
            }
         };
         
         class MatrixNLessTest: public unit::TestCase
         {
         public:
            MatrixNLessTest()
               :unit::TestCase("tc::math::tests::MatrixNLessTest")
            {
            }

            virtual void Execute()
            {
               {
                  MatrixType val1(0);
                  MatrixType val2(0);

                  TCUNIT_ASSERT(!(val1 <  val2));
                  TCUNIT_ASSERT( (val1 <= val2));
                  TCUNIT_ASSERT(!(val1 >  val2));
                  TCUNIT_ASSERT( (val1 >= val2));
               }

               {
                  MatrixType val1(GetDefaultMatrix());
                  MatrixType val2(GetDefaultMatrix());
                  val2(0, 0) = -1;

                  TCUNIT_ASSERT(!(val1 <  val2));
                  TCUNIT_ASSERT(!(val1 <= val2));
                  TCUNIT_ASSERT( (val1 >  val2));
                  TCUNIT_ASSERT( (val1 >= val2));
               }

               {
                  MatrixType val1(GetDefaultMatrix());
                  MatrixType val2(GetDefaultMatrix());
                  val1(0, 0) = -1;

                  TCUNIT_ASSERT( (val1 <  val2));
                  TCUNIT_ASSERT( (val1 <= val2));
                  TCUNIT_ASSERT(!(val1 >  val2));
                  TCUNIT_ASSERT(!(val1 >= val2));
               }

               {
                  MatrixType val1(GetDefaultMatrix());
                  MatrixType val2(GetDefaultMatrix());
                  val2(MatrixType::NUM_COMPONENTS-1, MatrixType::NUM_COMPONENTS-1) = -1;

                  TCUNIT_ASSERT(!(val1 <  val2));
                  TCUNIT_ASSERT(!(val1 <= val2));
                  TCUNIT_ASSERT( (val1 >  val2));
                  TCUNIT_ASSERT( (val1 >= val2));
               }

               {
                  MatrixType val1(GetDefaultMatrix());
                  MatrixType val2(GetDefaultMatrix());
                  val1(MatrixType::NUM_COMPONENTS-1, MatrixType::NUM_COMPONENTS-1) = -1;

                  TCUNIT_ASSERT( (val1 <  val2));
                  TCUNIT_ASSERT( (val1 <= val2));
                  TCUNIT_ASSERT(!(val1 >  val2));
                  TCUNIT_ASSERT(!(val1 >= val2));
               }
            }
         };

         MatrixNSuite::MatrixNSuite()
            :unit::TestSuite("tc::math::tests::MatrixNSuite")
         {
            AddTest(unit::Test::Ptr(new MatrixNConstructTest));
            AddTest(unit::Test::Ptr(new MatrixNAssignTest));
            AddTest(unit::Test::Ptr(new MatrixNAddAssignTest));
            AddTest(unit::Test::Ptr(new MatrixNSubtractAssignTest));
            AddTest(unit::Test::Ptr(new MatrixNMultiplyAssignTest));
            AddTest(unit::Test::Ptr(new MatrixNDivideAssignTest));
            AddTest(unit::Test::Ptr(new MatrixNEqualTest));
            AddTest(unit::Test::Ptr(new MatrixNLessTest));
         }
      }
   }
}

#endif
