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
//  $Id: TCOutput.h,v b8c6fe8666eb 2014/09/15 21:38:48 thomas $
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

      /** typedef for a print target shared pointer */
      typedef SharedPtr<PrintTarget> PrintTargetPtr;

      class PrintFormatter
      {
      public:
         virtual std::string Print(const char* module, uint32_t level, const char* function, uint32_t line_number) = 0;

         /** @brief Virtual destructor for the interface */
         virtual ~PrintFormatter() {}
      };

	  /** typedef for a formatter shared pointer */
      typedef SharedPtr<PrintFormatter> PrintFormatterPtr;

      /**
       * Method for setting the trace level
       * @param level is the level number
       * @see tc::Debug::m_trace_level
       */
      TCBASE_API void SetTraceLevel(int32_t level);
      /**
       * @return trace level
       */
      TCBASE_API int32_t GetTraceLevel();

      /** @brief Set the print target object for printing errors, warnings, info and traces */
      TCBASE_API void SetOuputTarget(PrintTargetPtr output_target);
      /** @brief Set the print formatter object for printing errors, warnings, info and traces */
      TCBASE_API void SetOuputFormatter(PrintFormatterPtr output_formatter);

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
         uint32_t line_number, const char* format, ...);
      /**
      * @brief Write output to the current active warning print target
      * @param module Name of the module to which this warning belongs to
      * @param function Name of the function, to which this warning belongs to
      * @param line_number line number of this warning
      * @param format of the output used by snprintf
      * @param ... parameters needed because of the format by snprintf
      */
      TCBASE_API void Warning(const char* module, const char* function, 
         uint32_t line_number, const char* format, ...);
      /**
      * @brief Write output to the current active info print target
      * @param module Name of the module to which this info belongs to
      * @param function Name of the function, to which this info belongs to
      * @param line_number line number of this info
      * @param format of the output used by snprintf
      * @param ... parameters needed because of the format by snprintf
      */
      TCBASE_API void Info(const char* module, const char* function, 
         uint32_t line_number, const char* format, ...);
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
      TCBASE_API void Trace(const char* module, int32_t level, const char* function, 
         uint32_t line_number, const char* format, ...);

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
#   define TCTRACES(MODULE, LEVEL, STREAM) \
    { \
        std::string _tc_tmp_stream_val; \
        tc::StreamPtr _tc_tmp_stream(tc::factory::CreateStringStream(_tc_tmp_stream_val)); \
        _tc_tmp_stream << STREAM; \
        tc::output::Trace(MODULE, LEVEL, _TC_FUNCTION_, __LINE__, "%s", _tc_tmp_stream_val.c_str()); \
    }

#else
#   define TCTRACES(MODULE, LEVEL, STREAM) ((void)0)
#endif

#define TCINFOS(MODULE, STREAM) \
    { \
       std::string _tc_tmp_stream_val; \
       tc::StreamPtr _tc_tmp_stream(tc::factory::CreateStringStream(_tc_tmp_stream_val)); \
       _tc_tmp_stream << STREAM; \
       tc::output::Info(MODULE, _TC_FUNCTION_, __LINE__, "%s", _tc_tmp_stream_val.c_str()); \
   }

#define TCWARNINGS(MODULE, STREAM) \
    { \
       std::string _tc_tmp_stream_val; \
       tc::StreamPtr _tc_tmp_stream(tc::factory::CreateStringStream(_tc_tmp_stream_val)); \
       _tc_tmp_stream << STREAM; \
       tc::output::Warning(MODULE, _TC_FUNCTION_, __LINE__, "%s", _tc_tmp_stream_val.c_str()); \
   }

#define TCERRORS(MODULE, STREAM) \
    { \
       std::string _tc_tmp_stream_val; \
       tc::StreamPtr _tc_tmp_stream(tc::factory::CreateStringStream(_tc_tmp_stream_val)); \
       _tc_tmp_stream << STREAM; \
       tc::output::Error(MODULE, _TC_FUNCTION_, __LINE__, "%s", _tc_tmp_stream_val.c_str()); \
   }

/**
 * @}
 */

#endif // _TC_OUTPUT_H_
