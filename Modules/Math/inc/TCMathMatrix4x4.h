//*******************************************************************************
//
// *******   ***   ***               *
//    *     *     *                  *
//    *    *      *                *****
//    *    *       ***  *   *   **   *    **    ***
//    *    *          *  * *   *     *   ****  * * *
//    *     *         *   *      *   * * *     * * *
//    *      ***   ***    *     **   **   **   *   *
//                        *
//*******************************************************************************
// see http://sourceforge.net/projects/tcsystem/ for details.
// Copyright (C) 2003 - 2012 Thomas Goessler. All Rights Reserved. 
//*******************************************************************************
//
// TCSystem is the legal property of its developers.
// Please refer to the COPYRIGHT file distributed with this source distribution.
// 
// This library is free software; you can redistribute it and/or             
// modify it under the terms of the GNU Lesser General Public                
// License as published by the Free Software Foundation; either              
// version 2.1 of the License, or (at your option) any later version.        
//                                                                           
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
//                                                                           
// You should have received a copy of the GNU Lesser General Public          
// License along with this library; if not, write to the Free Software       
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
//*******************************************************************************
//  $Id:$
//*******************************************************************************

#ifndef _TC_MATH_MATRIX_4x4_H_
#define _TC_MATH_MATRIX_4x4_H_

#include "TCMathMatrixN.h"
#include "TCMathCoordN.h"
#include "TCAssert.h"

namespace tc
{
   namespace math
   {
      /**
      * @addtogroup TC_MATH
      * @{
      */

      /**
      * @file
      * @brief This file provides the definition of tc::math::Matrix4x4f, tc::math::Matrix4x4d
      * @author Thomas Goessler
      */

      typedef MatrixN<float, 4, 4> Matrix4x4f;
      typedef MatrixN<double, 4, 4> Matrix4x4d;

      template <typename T>
      void TranslateMatrix(MatrixN<T, 4, 4>& m, const CoordN<T, 3>& trans)
      {
         m[3][0] += trans[0]*m[0][0] + trans[1]*m[1][0] + trans[2]*m[2][0];
         m[3][1] += trans[0]*m[0][1] + trans[1]*m[1][1] + trans[2]*m[2][1];
         m[3][2] += trans[0]*m[0][2] + trans[1]*m[1][2] + trans[2]*m[2][2];
         m[3][3] += trans[0]*m[0][3] + trans[1]*m[1][3] + trans[2]*m[2][3];
      }

      template <typename T>
      void ScaleMatrix(MatrixN<T, 4, 4>& m, const CoordN<T, 3>& scale)
      {
         m[0][0]*=scale[0]; m[0][1]*=scale[0]; m[0][2]*=scale[0]; m[0][3]*=scale[0];
         m[1][0]*=scale[1]; m[1][1]*=scale[1]; m[1][2]*=scale[1]; m[1][3]*=scale[1];
         m[2][0]*=scale[2]; m[2][1]*=scale[2]; m[2][2]*=scale[2]; m[2][3]*=scale[2];
      }

      // Obtain local x axis of quaternion
      template <typename T>
      CoordN<T, 3> GetXAxis(const CoordN<T, 4>& r) 
      {
         T ty = 2*r[1];
         T tz = 2*r[2];
         return CoordN<T, 3>(T(1.0-ty*r[1]-tz*r[2]), T(ty*r[0]+tz*r[3]), T(tz*r[0]-ty*r[3]));
      }

      // Obtain local y axis quaternion
      template <typename T>
      CoordN<T, 3> GetYAxis(const CoordN<T, 4>& r)
      {
         T tx = 2*r[0];
         T tz = 2*r[2];
         return CoordN<T, 3>(T(tx*r[1]-tz*r[3]), T(1.0-tx*r[0]-tz*r[2]), T(tz*r[1]+tx*r[3]));
      }


      // Obtain local z axis quaternion
      template <typename T>
      CoordN<T, 3> GetZAxis(const CoordN<T, 4>& r)
      {
         T tx = 2*r[0];
         T ty = 2*r[1];
         return CoordN<T, 3>(T(tx*r[2]+ty*r[3]), T(ty*r[2]-tx*r[3]), T(1.0-tx*r[0]-ty*r[1]));
      }

      template <typename T>
      MatrixN<T, 3, 3> ToRotationMatrix(const CoordN<T, 4>& r)
      {
         CoordN<T, 3> x(GetXAxis(r));
         CoordN<T, 3> y(GetYAxis(r));
         CoordN<T, 3> z(GetZAxis(r));

         MatrixN<T, 3, 3> m;
         m[0][0] = x[0];
         m[0][1] = x[1];
         m[0][2] = x[2];
         m[1][0] = y[0];
         m[1][1] = y[1];
         m[1][2] = y[2];
         m[2][0] = z[0];
         m[2][1] = z[1];
         m[2][2] = z[2];

         return m;
      }

      template <typename T>
      void RotateMatrix(MatrixN<T, 4, 4>& m, const CoordN<T, 4>& q)
      {
         T x,y,z;

         // Get rotation matrix
         MatrixN<T, 3, 3> r(ToRotationMatrix(q));

         // Pre-multiply
         x=m[0][0]; y=m[1][0]; z=m[2][0];
         m[0][0]=x*r[0][0]+y*r[0][1]+z*r[0][2];
         m[1][0]=x*r[1][0]+y*r[1][1]+z*r[1][2];
         m[2][0]=x*r[2][0]+y*r[2][1]+z*r[2][2];
         x=m[0][1]; y=m[1][1]; z=m[2][1];
         m[0][1]=x*r[0][0]+y*r[0][1]+z*r[0][2];
         m[1][1]=x*r[1][0]+y*r[1][1]+z*r[1][2];
         m[2][1]=x*r[2][0]+y*r[2][1]+z*r[2][2];
         x=m[0][2]; y=m[1][2]; z=m[2][2];
         m[0][2]=x*r[0][0]+y*r[0][1]+z*r[0][2];
         m[1][2]=x*r[1][0]+y*r[1][1]+z*r[1][2];
         m[2][2]=x*r[2][0]+y*r[2][1]+z*r[2][2];
         x=m[0][3]; y=m[1][3]; z=m[2][3];
         m[0][3]=x*r[0][0]+y*r[0][1]+z*r[0][2];
         m[1][3]=x*r[1][0]+y*r[1][1]+z*r[1][2];
         m[2][3]=x*r[2][0]+y*r[2][1]+z*r[2][2];
      }

      template <typename T>
      MatrixN<T, 4, 4> InvertMatrix(const MatrixN<T, 4, 4>& m)
      {
         MatrixN<T, 4, 4> r(MatrixN<T, 4, 4>::PredefinedType::IDENTITY);
         MatrixN<T, 4, 4> x(m);

         T pvv,t;
         uint32_t i,j,pvi;
         for (i=0; i<4; i++)
         {
            pvv = x[i][i];
            pvi = i;
            for (j=i+1; j<4; j++)
            {   
               // Find pivot (largest in column i)
               if(std::fabs(x[j][i])>std::fabs(pvv))
               {
                  pvi = j;
                  pvv = x[j][i];
               }
            }

            TC_ASSERT(pvv != 0.0f);  // Should not be singular
            if (pvi != i)
            {             
#define MATRIX_SWAP(a,b,t) ((t)=(a),(a)=(b),(b)=(t))
               // Swap rows i and pvi
               MATRIX_SWAP(r[i][0],r[pvi][0],t); MATRIX_SWAP(r[i][1],r[pvi][1],t); MATRIX_SWAP(r[i][2],r[pvi][2],t); MATRIX_SWAP(r[i][3],r[pvi][3],t);
               MATRIX_SWAP(x[i][0],x[pvi][0],t); MATRIX_SWAP(x[i][1],x[pvi][1],t); MATRIX_SWAP(x[i][2],x[pvi][2],t); MATRIX_SWAP(x[i][3],x[pvi][3],t);
#undef MATRIX_SWAP
            }
            x[i][0]/=pvv; x[i][1]/=pvv; x[i][2]/=pvv; x[i][3]/=pvv;
            r[i][0]/=pvv; r[i][1]/=pvv; r[i][2]/=pvv; r[i][3]/=pvv;

            for(j=0; j<4; j++)
            {     
               // Eliminate column i
               if(j!=i)
               {
                  t=x[j][i];
                  x[j][0]-=x[i][0]*t; x[j][1]-=x[i][1]*t; x[j][2]-=x[i][2]*t; x[j][3]-=x[i][3]*t;
                  r[j][0]-=r[i][0]*t; r[j][1]-=r[i][1]*t; r[j][2]-=r[i][2]*t; r[j][3]-=r[i][3]*t;
               }
            }
         }
         return r;
      }
      /**
      * @}
      */

   } // namespace math
} // namespace tc

#endif //_TC_MATH_MATRIX_4x4_H_
