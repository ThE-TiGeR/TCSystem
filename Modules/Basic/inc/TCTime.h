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
// see https://bitbucket.org/the____tiger/tcsystem for details.
// Copyright (C) 2003 - 2018 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCTime.h,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#ifndef _TC_TIME_H_
#define _TC_TIME_H_

#include "TCStream.h"

namespace tc
{
   /**
   * @addtogroup TC_BASE
   * @{
   */

   /**
   * @file 
   * This header file provides the definition of tc::Time. 
   * 
   * @author Thomas Goessler 
   */

   /**
   * @brief Class holding information about absolute time in seconds +  nano seconds
   */
   class TCBASE_API Time
   {
   public:
      static const uint64_t ONE_SECOND_AS_MILLI_SECONDS = 1000;
      static const uint64_t ONE_SECOND_AS_MICRO_SECONDS = ONE_SECOND_AS_MILLI_SECONDS * 1000;
      static const uint64_t ONE_SECOND_AS_NANO_SECONDS = ONE_SECOND_AS_MICRO_SECONDS * 1000;
      static const uint64_t ONE_MILLI_SECOND_AS_MICRO_SECONDS = 1000;
      static const uint64_t ONE_MILLI_SECOND_AS_NANO_SECONDS = ONE_MILLI_SECOND_AS_MICRO_SECONDS * 1000;
      static const uint64_t ONE_MICRO_SECOND_AS_NANO_SECONDS = 1000;

   public:
      /** Create a Time object with specified seconds */
      static Time FromSeconds(uint64_t seconds);
      /** Create a Time object with specified milli seconds */
      static Time FromMilliSeconds(uint64_t milli_seconds);
      /** Create a Time object with specified micro seconds */
      static Time FromMicroSeconds(uint64_t micro_seconds);
      /** Create a Time object with specified nano seconds */
      static Time FromNanoSeconds(uint64_t nanos_seconds);
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
      Time(uint64_t secs, uint64_t nsecs);

      /** Get the seconds of the time object */
      uint64_t Seconds() const {return m_secs;}
      /** Get the nano seconds of the time object */
      uint64_t NanoSeconds() const {return m_nsecs;}

      /** Get Total time in seconds */
      uint64_t ToSeconds() const
      {
         return m_secs + m_nsecs / ONE_SECOND_AS_NANO_SECONDS;
      }

      /** Get Total time in milli seconds */
      uint64_t ToMilliSeconds() const
      {
         return m_secs * ONE_SECOND_AS_MILLI_SECONDS + m_nsecs / ONE_MILLI_SECOND_AS_NANO_SECONDS;
      }

      /** Get Total time in micro seconds */
      uint64_t ToMicroSeconds() const
      {
         return m_secs * ONE_SECOND_AS_MICRO_SECONDS + m_nsecs / ONE_MICRO_SECOND_AS_NANO_SECONDS;
      }

      /** Get Total time in nano seconds */
      uint64_t ToNanoSeconds() const
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
      uint64_t m_secs;
      uint64_t m_nsecs;
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

