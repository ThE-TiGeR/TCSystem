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
//  $Id: TCRng69069.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//----------------------------------------------------------------------

#include "TCRng69069.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace imp
   {
      Rng69069::Rng69069(uint32_t seed)
         :m_rng(seed)
      {
      }

      uint32_t Rng69069::GetRandomNumber()
      {
         return CreateRandomNumber();
      }

      uint32_t Rng69069::GetRandomNumber(uint32_t lower_limit, uint32_t upper_limit)
      {
         const uint64_t diff = 1 + static_cast<uint64_t>(upper_limit - lower_limit);
         uint64_t aux = diff * CreateRandomNumber();
         aux = aux >> 32;

         return lower_limit + static_cast<uint32_t>(aux);
      }

      double Rng69069::GetRandomNumberDouble(double lower_limit, double upper_limit)
      {
         const double val = static_cast<double>(CreateRandomNumber()) / static_cast<double>(0xffffffff);

         return lower_limit + val * (upper_limit - lower_limit);
      }

      void Rng69069::SetSeed(uint32_t seed)
      {
         m_rng = seed;
      }

      uint32_t Rng69069::CreateRandomNumber()
      {
         m_rng = (69069 * m_rng) + 1;
         return m_rng;
      }
   }
}
