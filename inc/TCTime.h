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
// Copyright (C) 2003 - 2010 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCTime.h 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#ifndef _TC_TIME_H_
#define _TC_TIME_H_

#include "TCStream.h"

namespace TC
{
   /**
   * @addtogroup TC_BASE
   * @{
   */

   /**
   * @file 
   * This header file provides the definition of TC::Time. 
   * 
   * @author Thomas Goessler 
   */

   /**
   * @brief Class holding information about absolute time in seconds +  nano seconds
   */
   class TCBASE_API Time
   {
   public:
      enum Consts
      {
         ONE_SECOND_AS_MILLI_SECONDS      = 1000,
         ONE_SECOND_AS_MICRO_SECONDS      = ONE_SECOND_AS_MILLI_SECONDS * 1000,
         ONE_SECOND_AS_NANO_SECONDS       = ONE_SECOND_AS_MICRO_SECONDS * 1000,

         ONE_MILLI_SECOND_AS_MICRO_SECONDS = 1000,
         ONE_MILLI_SECOND_AS_NANO_SECONDS  = ONE_MILLI_SECOND_AS_MICRO_SECONDS * 1000,

         ONE_MICRO_SECOND_AS_NANO_SECONDS = 1000
      };

   public:
      /** Create a Time object with specified seconds */
      static Time FromSeconds(uint64 seconds);
      /** Create a Time object with specified milli seconds */
      static Time FromMilliSeconds(uint64 milli_seconds);
      /** Create a Time object with specified micro seconds */
      static Time FromMicroSeconds(uint64 micro_seconds);
      /** Create a Time object with specified nano seconds */
      static Time FromNanoSeconds(uint64 nanos_seconds);
      /** Create a Time object with current time values */
      static Time Now();
      /** Create a Time object with current monotonic time values 
      (time since a unspecified reference point) */
      static Time NowMonotonic();
      /** Create a Time object holding the time elapsed since specified time */
      static Time Since(const Time& start_time)
      {
         return Now() -= start_time;
      }
      /** Create a Time object holding the monotonic time elapsed since specified time */
      static Time SinceMonotonic(const Time& start_time)
      {
         return NowMonotonic() -= start_time;
      }
      /** Create a Time object holding infinite time */
      static Time Infinite();
      /** Create a Time object holding zero time */
      static Time Zero();

      /** @brief Constructor with time 0 */
      Time()
         :m_secs(0),
         m_nsecs(0)
      {
      }

      /** @brief Constructor with specified seconds and nano seconds */
      Time(uint64 secs, uint64 nsecs);

      /** Get the seconds of the time object */
      uint64 Seconds() const {return m_secs;}
      /** Get the nano seconds of the time object */
      uint64 NanoSeconds() const {return m_nsecs;}

      /** Get Total time in milli seconds */
      uint64 ToMilliSeconds() const
      {
         return m_secs * ONE_SECOND_AS_MILLI_SECONDS + m_nsecs / ONE_MILLI_SECOND_AS_NANO_SECONDS;
      }

      /** Get Total time in micro seconds */
      uint64 ToMicroSeconds() const
      {
         return m_secs * ONE_SECOND_AS_MICRO_SECONDS + m_nsecs / ONE_MICRO_SECOND_AS_NANO_SECONDS;
      }

      /** Get Total time in nano seconds */
      uint64 ToNanoSeconds() const
      {
         return m_secs * ONE_SECOND_AS_NANO_SECONDS + m_nsecs;
      }

      Time& operator+=(const Time& time_to_add);
      Time& operator-=(const Time& time_to_add);

      bool operator==(const Time& a) const
      {
         return (m_secs == a.m_secs) && (m_nsecs == a.m_nsecs);
      }

      bool operator!=(const Time& a) const
      {
         return !this->operator==(a);
      }

      bool operator<(const Time& a) const;

      bool operator<=(const Time& a) const
      {
         return this->operator==(a) || this->operator<(a);
      }

      bool operator>(const Time& a) const
      {
         return !this->operator<=(a);
      }

      bool operator>=(const Time& a) const
      {
         return !this->operator<(a);
      }

      friend TCBASE_API StreamPtr operator>>(StreamPtr stream, Time& time);
      friend TCBASE_API StreamPtr operator<<(StreamPtr stream, const Time& time);

   private:
      uint64 m_secs;
      uint64 m_nsecs;
   };

   inline const Time operator+(const Time& a, const Time& b) 
   {
      return Time(a) += b;
   }

   inline const Time operator-(const Time& a, const Time& b) 
   {
      return Time(a) -= b;
   }
}

#endif // _TC_TIME_H_

