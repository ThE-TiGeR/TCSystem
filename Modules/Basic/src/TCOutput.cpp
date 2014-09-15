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
// Copyright (C) 2003 - 2014 Thomas Goessler. All Rights Reserved. 
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

#include "TCOutput.h"

#include "TCFactory.h"
#include "TCString.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

using namespace std;

namespace tc
{
   namespace output
   {
      class DefaultPrintFormatter: public PrintFormatter
      {
      public:
         DefaultPrintFormatter(const char* type)
            :m_type(type)
         {
         }

         virtual std::string Print(const char* module, uint32_t level, const char* function, uint32_t line_number)
         {
            std::string s;
            if (level == 0)
            {
               s = m_type + string::Print("(%s, %6s), %s(%u): ", system::GetDate().c_str(), module, function, line_number);
            }
            else
            {
               s = m_type + string::Print("(%s, %6s[% 4d]), %s(%u): ", system::GetDate().c_str(), module, level, function, line_number);
            }

            return s;
         }

      private:
         std::string m_type;
      };

      static int32_t s_trace_level = 10;
      static PrintTargetPtr s_error_target;
      static PrintTargetPtr s_warning_target;
      static PrintTargetPtr s_info_target;
      static PrintTargetPtr s_trace_target;

      
      static PrintFormatterPtr s_error_formatter(new DefaultPrintFormatter("%E"));
      static PrintFormatterPtr s_warning_formatter(new DefaultPrintFormatter("%W"));
      static PrintFormatterPtr s_info_formatter(new DefaultPrintFormatter("%I"));
      static PrintFormatterPtr s_trace_formatter(new DefaultPrintFormatter("%T"));

      void SetTraceLevel(int32_t level)
      {
         s_trace_level = level;
         TCTRACES("TCBASE", -1, "setting trace level to " << s_trace_level);
      }

      int32_t GetTraceLevel()
      {
         return s_trace_level;
      }

      void SetOuputTarget(PrintTargetPtr target)
      {
         SetErrorTarget(target);
         SetWarningTarget(target);
         SetInfoTarget(target);
         SetTraceTarget(target);
      }

      void SetErrorTarget(PrintTargetPtr target)
      {
         s_error_target = target;
      }
      void SetWarningTarget(PrintTargetPtr target)
      {
         s_warning_target =target;
      }
      void SetInfoTarget(PrintTargetPtr target)
      {
         s_info_target = target;
      }
      void SetTraceTarget(PrintTargetPtr target)
      {
         s_trace_target = target;
      }

      void Error(const char* module, const char* function, 
         uint32_t line_number, const char* format, ...)
      {
         std::string s = s_error_formatter->Print(module, 0, function, line_number);

         va_list arguments;
         va_start(arguments, format);
         s += string::VPrint(format, arguments);
         va_end(arguments);

         if (s_error_target)
         {
            s_error_target->Print(s.c_str());
         }
         else
         {
            std::fprintf(stderr, "%s\n", s.c_str());
            std::fflush(stderr);
         }
      }

      void Warning(const char* module, const char* function, 
         uint32_t line_number, const char* format, ...)
      {
         std::string s = s_warning_formatter->Print(module, 0, function, line_number);

         va_list arguments;
         va_start(arguments, format);
         s += string::VPrint(format, arguments);
         va_end(arguments);

         if (s_warning_target)
         {
            s_warning_target->Print(s.c_str());
         }
         else
         {
            std::fprintf(stderr, "%s\n", s.c_str());
            std::fflush(stderr);
         }
      }

      void Info(const char* module, const char* function, 
         uint32_t line_number, const char* format, ...)
      {
         std::string s = s_info_formatter->Print(module, 0, function, line_number);

         va_list arguments;
         va_start(arguments, format);
         s += string::VPrint(format, arguments);
         va_end(arguments);

         if (s_info_target)
         {
            s_info_target->Print(s.c_str());
         }
         else
         {
            std::fprintf(stdout, "%s\n", s.c_str());
            std::fflush(stdout);
         }
      }

      void Trace(const char* module, int32_t level, const char* function, 
         uint32_t line_number, const char* format, ...)
      {
         if (level < s_trace_level)
         {
            std::string s = s_trace_formatter->Print(module, level, function, line_number);

            va_list arguments;
            va_start(arguments, format);
            s += string::VPrint(format, arguments);
            va_end(arguments);

            if (s_trace_target)
            {
               s_trace_target->Print(s.c_str());
            }
            else
            {
               std::fprintf(stdout, "%s\n", s.c_str());
               std::fflush(stdout);
            }
         }
      }

      void SetOuputFormatter(PrintFormatterPtr output_formatter)
      {
         s_error_formatter = output_formatter;
         s_info_formatter = output_formatter;
         s_warning_formatter = output_formatter;
         s_trace_formatter = output_formatter;
      }

   }
}
