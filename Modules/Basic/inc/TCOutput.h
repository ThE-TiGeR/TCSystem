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

#ifndef _TC_OUTPUT_H_
#define _TC_OUTPUT_H_

#include "TCFactory.h"

namespace tc
{
   /**
    * @addtogroup TC_BASE_DEBUG
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of tc::output and Macros for 
    *        writing traces, infos warnings and errors
    *
    * @author Thomas Goessler
    */

   /**
    * class used for debug output
    */
   namespace output
   {
      /** Pure virtual base class for printing trace output */
      class PrintTarget
      {
      public:
         virtual void Print(const char* text) = 0;

         /** @brief Virtual destructor for the interface */
         virtual ~PrintTarget() {}
      };

	  /** typedef for a print target object */
	  typedef SharedPtr<PrintTarget> PrintTargetPtr;

      /**
       * Method for setting the trace level
       * @param level is the level number
       * @see tc::Debug::m_trace_level
       */
      TCBASE_API void SetTraceLevel(sint32 level);
      /**
       * @return trace level
       */
      TCBASE_API sint32 GetTraceLevel();

      /** @brief Set the print target object for printing errors, earnigs, info and traces */
      TCBASE_API void SetOuputTarget(PrintTargetPtr error_target);

      /** @brief Set the print target object for printing errors */
      TCBASE_API void SetErrorTarget(PrintTargetPtr error_target);
      /** @brief Set the print target object for printing warnings */
      TCBASE_API void SetWarningTarget(PrintTargetPtr warning_target);
      /** @brief Set the print target object for printing infos */
      TCBASE_API void SetInfoTarget(PrintTargetPtr info_target);
      /** @brief Set the print target object for printing the traces */
      TCBASE_API void SetTraceTarget(PrintTargetPtr trace_target);

      /**
       * @brief Write output to the current active error print target
       * @param module Name of the module to which this error belongs to
       * @param function Name of the function, to which this error belongs to
       * @param line_number line number of this error
       * @param format of the output used by snprintf
       * @param ... parameters needed because of the format by snprintf
       */
      TCBASE_API void Error(const char* module, const char* function, 
         uint32 line_number, const char* format, ...);
      /**
      * @brief Write output to the current active warning print target
      * @param module Name of the module to which this warning belongs to
      * @param function Name of the function, to which this warning belongs to
      * @param line_number line number of this warning
      * @param format of the output used by snprintf
      * @param ... parameters needed because of the format by snprintf
      */
      TCBASE_API void Warning(const char* module, const char* function, 
         uint32 line_number, const char* format, ...);
      /**
      * @brief Write output to the current active info print target
      * @param module Name of the module to which this info belongs to
      * @param function Name of the function, to which this info belongs to
      * @param line_number line number of this info
      * @param format of the output used by snprintf
      * @param ... parameters needed because of the format by snprintf
      */
      TCBASE_API void Info(const char* module, const char* function, 
         uint32 line_number, const char* format, ...);
      /**
      * Write output to the current active trace print target
      * when the level is lower then the global trace level tc::Debug::m_trace_level
      * @param module Name of the module to which this trace belongs to
      * @param level trace level of this output
      * @param function Name of the function, to which this trace belongs to
      * @param line_number line number of this trace
      * @param format of the output used by snprintf
      * @param ... parameters needed because of the format by snprintf
      */
      TCBASE_API void Trace(const char* module, sint32 level, const char* function, 
         uint32 line_number, const char* format, ...);

   }// namespace output

   /**
    * @}
    */

} // namespace tc

/**
 * @addtogroup TC_BASE_DEBUG
 * @{
 */

#ifdef _MSC_VER
#  define _TC_FUNCTION_ __FUNCTION__
#else
#  define _TC_FUNCTION_ __PRETTY_FUNCTION__
#endif

#ifndef TCTRACE_ENABLE
#  ifdef DEBUG
#     define TCTRACE_ENABLE
#  endif
#endif

#ifdef TCTRACE_ENABLE
#   define TCTRACE(MODULE, LEVEL, FORMAT)                          tc::output::Trace(MODULE, LEVEL, _TC_FUNCTION_, __LINE__, FORMAT)
#   define TCTRACE1(MODULE, LEVEL, FORMAT, ARG1)                   tc::output::Trace(MODULE, LEVEL, _TC_FUNCTION_, __LINE__, FORMAT, ARG1)
#   define TCTRACE2(MODULE, LEVEL, FORMAT, ARG1, ARG2)             tc::output::Trace(MODULE, LEVEL, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2)
#   define TCTRACE3(MODULE, LEVEL, FORMAT, ARG1, ARG2, ARG3)       tc::output::Trace(MODULE, LEVEL, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2, ARG3)
#   define TCTRACE4(MODULE, LEVEL, FORMAT, ARG1, ARG2, ARG3, ARG4) tc::output::Trace(MODULE, LEVEL, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2, ARG3, ARG4)
#   define TCTRACEF(MODULE, LEVEL)                                 tc::output::Trace(MODULE, LEVEL, _TC_FUNCTION_, __LINE__, "")
#   define TCTRACES(MODULE, LEVEL, STREAM) \
    { \
        std::string val; \
        tc::StreamPtr stream(tc::factory::CreateStringStream(val)); \
        stream << STREAM; \
        tc::output::Trace(MODULE, LEVEL, _TC_FUNCTION_, __LINE__, "%s", val.c_str()); \
    }

#else
#   define TCTRACE(MODULE, LEVEL, FORMAT) ((void)0)
#   define TCTRACE1(MODULE, LEVEL, FORMAT, ARG1) ((void)0)
#   define TCTRACE2(MODULE, LEVEL, FORMAT, ARG1, ARG2) ((void)0)
#   define TCTRACE3(MODULE, LEVEL, FORMAT, ARG1, ARG2, ARG3) ((void)0)
#   define TCTRACE4(MODULE, LEVEL, FORMAT, ARG1, ARG2, ARG3, ARG4) ((void)0)
#   define TCTRACEF(MODULE, LEVEL)((void)0)
#   define TCTRACES(MODULE, LEVEL, STREAM) ((void)0)
#endif

#define TCINFO(MODULE, FORMAT)                          tc::output::Info(MODULE, _TC_FUNCTION_, __LINE__, FORMAT)
#define TCINFO1(MODULE, FORMAT, ARG1)                   tc::output::Info(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1)
#define TCINFO2(MODULE, FORMAT, ARG1, ARG2)             tc::output::Info(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2)
#define TCINFO3(MODULE, FORMAT, ARG1, ARG2, ARG3)       tc::output::Info(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2, ARG3)
#define TCINFO4(MODULE, FORMAT, ARG1, ARG2, ARG3, ARG4) tc::output::Info(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2, ARG3, ARG4)
#define TCINFOS(MODULE, STREAM) \
    { \
       std::string _tc_tmp_stream_val; \
       tc::StreamPtr stream(tc::factory::CreateStringStream(_tc_tmp_stream_val)); \
       stream << STREAM; \
       tc::output::Info(MODULE, _TC_FUNCTION_, __LINE__, "%s", _tc_tmp_stream_val.c_str()); \
   }

#define TCWARNING(MODULE, FORMAT)                          tc::output::Warning(MODULE, _TC_FUNCTION_, __LINE__, FORMAT)
#define TCWARNING1(MODULE, FORMAT, ARG1)                   tc::output::Warning(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1)
#define TCWARNING2(MODULE, FORMAT, ARG1, ARG2)             tc::output::Warning(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2)
#define TCWARNING3(MODULE, FORMAT, ARG1, ARG2, ARG3)       tc::output::Warning(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2, ARG3)
#define TCWARNING4(MODULE, FORMAT, ARG1, ARG2, ARG3, ARG4) tc::output::Warning(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2, ARG3, ARG4)
#define TCWARNINGS(MODULE, STREAM) \
    { \
       std::string _tc_tmp_stream_val; \
       tc::StreamPtr stream(tc::factory::CreateStringStream(_tc_tmp_stream_val)); \
       stream << STREAM; \
       tc::output::Warning(MODULE, _TC_FUNCTION_, __LINE__, "%s", _tc_tmp_stream_val.c_str()); \
   }

#define TCERROR(MODULE, FORMAT)                          tc::output::Error(MODULE, _TC_FUNCTION_, __LINE__, FORMAT)
#define TCERROR1(MODULE, FORMAT, ARG1)                   tc::output::Error(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1)
#define TCERROR2(MODULE, FORMAT, ARG1, ARG2)             tc::output::Error(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2)
#define TCERROR3(MODULE, FORMAT, ARG1, ARG2, ARG3)       tc::output::Error(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2, ARG3)
#define TCERROR4(MODULE, FORMAT, ARG1, ARG2, ARG3, ARG4) tc::output::Error(MODULE, _TC_FUNCTION_, __LINE__, FORMAT, ARG1, ARG2, ARG3, ARG4)
#define TCERRORS(MODULE, STREAM) \
    { \
       std::string _tc_tmp_stream_val; \
       tc::StreamPtr stream(tc::factory::CreateStringStream(_tc_tmp_stream_val)); \
       stream << STREAM; \
       tc::output::Error(MODULE, _TC_FUNCTION_, __LINE__, "%s", _tc_tmp_stream_val.c_str()); \
   }

/**
 * @}
 */

#endif // _TC_OUTPUT_H_
