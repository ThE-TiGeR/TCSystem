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
//  $Id$
//*******************************************************************************

#include "TCMathUtil.h"

#include <float.h>
#include <math.h>

#if defined TCOS_SUN || defined TCOS_CYGWIN
#include <ieeefp.h>
#endif

#include "TCNewEnable.h"

using namespace std;

namespace tc
{
   namespace math
   {
      bool IsNaN(double val)
      {
#ifdef TCOS_WINDOWS
         return (_isnan(val) == 0 ? false : true);
#elif TCOS_QNX
         return (isnanf(val) == 0 ? false : true);
#else
         return (isnan(val) == 0 ? false : true);
#endif
      }

      bool IsFinite(double val)
      {
#ifdef TCOS_WINDOWS
         return (_finite(val) == 0 ? false : true);
#elif TCOS_HPUX
         return (isfinite(val) == 0 ? false : true);
#else
         return (finite(val) == 0 ? false : true);
#endif
      }

      bool IsInf(double val)
      {
         return (IsNaN(val) == false && IsFinite(val) == false);
      }
   }
}
