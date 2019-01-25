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
//  $Id: TCMathBox.h,v d8463fc094c7 2013/04/03 22:28:58 thomas $
//*******************************************************************************

#ifndef _TC_MATH_BOX_H_
#define _TC_MATH_BOX_H_

#include "TCMathCoordUtil.h"
#include "TCMathUtil.h"
#include "TCStream.h"

#include <limits>
#include <vector>

namespace tc
{
   namespace math
   {
      /**
       * @brief Class defining a box with two coordinates
       *        The minimum and maximum pos in all components and the 
       */
      template <class COORD_TYPE>
      class Box
      {
      public:
         typedef COORD_TYPE CoordType;
         typedef typename COORD_TYPE::DataType DataType;

      public:
         /** @brief Construct an empty box */
         Box()
         {
         }

         /** @brief Copy an existing box */
         Box(const Box& box)
            :m_min(box.m_min),
            m_max(box.m_max)
         {
         }

         /** @brief Copy an existing box of different coord type */
         template <class T1>
         Box(const Box<T1>& box)
            :m_min(box.m_min),
            m_max(box.m_max)
         {
         }

         /**
          * @brief Construct a box out of minimum and maximum coordinate
          *
          * @param min Min coordinate
          * @param max Max coordinate
          */
         Box(const COORD_TYPE& min, const COORD_TYPE& max)
            :m_min(min),
            m_max(max)
         {
         }

         /** @brief assign one box tho this box */
         Box& operator=(const Box& box) 
         {
            m_min    = box.m_min;
            m_max = box.m_max;
            return *this;
         }

         /** @brief Get min coordinate of the box */
         const COORD_TYPE& GetMin() const {return m_min;}
         /** @brief Get max coordinate of the box */
         const COORD_TYPE& GetMax() const {return m_max;}

         /** @brief Set min coordinate of the box */
         void SetMin(const COORD_TYPE& top) {m_min=top;}
         /** @brief Set max coordinate of the box */
         void SetMax(const COORD_TYPE& bottom) {m_max=bottom;}

         /** Get by index 0 = min 1 = max */
         COORD_TYPE& operator[](uint32_t i){ return (&m_min)[i]; }
         /** Get by index 0 = min 1 = max */
         const COORD_TYPE& operator[](uint32_t i) const { return (&m_min)[i]; }

         /** @brief compare two boxes */
         bool operator==(const Box& box) const { return m_min==box.m_min && m_max==box.m_max; }
         /** @brief compare two boxes */
         bool operator!=(const Box& box) const { return m_min!=box.m_min || m_max!=box.m_max; }


         /// Width of box
         DataType GetWidth() const { return m_max[0] - m_min[0]; }
         /// Height of box
         DataType GetHeight() const { return m_max[1] - m_min[1]; }
         /// Depth of box
         DataType GetDepth() const { return m_max[2] - m_min[2]; }

         /// Longest side
         DataType GetLongest() const
         {
            COORD_TYPE diag = GetDiagonal();
            return util::Max(diag[0], diag[1], diag[2]);
         }

         /// shortest side
         DataType GetShortest() const
         {
            COORD_TYPE diag = GetDiagonal();
            return util::Min(diag[0], diag[1], diag[2]);
         }

         double GetDiameter() const 
         {
            return Distance(m_max, m_min); 
         }
         double GetRadius() const 
         {
            return GetDiameter()/static_cast<DataType>(2); 
         }

         /// Compute diagonal
         COORD_TYPE GetDiagonal() const {return m_max - m_min;}

         /// Get center of box
         COORD_TYPE GetCenter() const {return (m_max + m_min)/static_cast<DataType>(2);}

         /// Test if empty
         bool IsEmpty() const
         {
            return !((m_min.Length2() + m_max.Length2()) > 0);
         }

         /**
         * Check if the specified point is inside the box area
         */
         bool Contains(const COORD_TYPE& p) const
         {
            for (uint32_t i=0; i<COORD_TYPE::NUM_COMPONENTS; i++)
            {
               bool inside = m_min[i] <= p[i] && p[i] < m_max[i];
               if (!inside)
               {
                  return false;
               }
            }

            return true;
         }

         /**
         * Check if this box contains the specified box
         */
         bool Contains(const Box& r) const
         {
            return Contains(r.m_min) && Contains(r.m_max);
         }

         /**
          * @brief Get the combined box out of two boxes
          *
          * @param box1 First box to combine
          * @param box2 Second box to combine
          * @return The combined box out of box1 and box2
          */
         static Box Unite(const Box& box1, const Box& box2)
         {
            if (box1.IsEmpty())
            {
               return box2;
            }
            else if (box2.IsEmpty())
            {
               return box1;
            }

            COORD_TYPE min = Min(box1.GetMin(), box2.GetMin());
            COORD_TYPE max = Max(box1.GetMax(), box2.GetMax());
            return Box(min, max);
         }

         /**
          * @brief Create bounding box out of an vector of coordinates
          *
          * @param coords Vector of coordinates for setting up the bounding box
          * @return The created bounding box
          */
         static Box BoundingBox(const std::vector<COORD_TYPE>& coords)
         {
            COORD_TYPE c_min(std::numeric_limits<DataType>::max());
            COORD_TYPE c_max(std::numeric_limits<DataType>::min());

            typename std::vector<COORD_TYPE>::const_iterator c_it;
            for (c_it= coords.begin(); c_it!=coords.end(); c_it++)
            {
               c_min = Min(c_min, *c_it);
               c_max = Max(c_max, *c_it);
            }

            return Box(c_min, c_max);
         }
      private:
         COORD_TYPE m_min;
         COORD_TYPE m_max;

         template <class T1> friend class Box;
      };

      template <class COORD_TYPE>
      inline StreamPtr operator<<(StreamPtr stream, const Box<COORD_TYPE>& box)
      {
          return stream << box.GetMin() << space << box.GetMax();
      }

      template <class COORD_TYPE>
      inline StreamPtr operator>>(StreamPtr stream, Box<COORD_TYPE>& box)
      {
          COORD_TYPE min;
          COORD_TYPE max;
          stream >> min >> max;

          if (stream->IsOk())
          {
              box.SetMin(min);
              box.SetMax(max);
          }
          return stream;
      }
   }
}
#endif

