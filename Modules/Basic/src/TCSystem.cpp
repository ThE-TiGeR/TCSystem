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

#include "TCSystem.h"

#include "TCException.h"
#include "TCFile.h"
#include "TCFileName.h"
#include "TCString.h"
#include "TCScopedArray.h"
#include "TCTime.h"
#include "TCUtil.h"

#include <cstring>

#ifdef TCOS_WINCE_40
#   include <windows.h>
#   include <winbase.h>
#   include <winsock.h>
#   include <iphlpapi.h>
#elif TCOS_WINDOWS
#   include <windows.h>
#   include <winbase.h>
#   include <io.h>
#   include <process.h>
#   include <direct.h>
#   include <time.h>
#   include <signal.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <fcntl.h>
#   include <winsock2.h>
#   include <iphlpapi.h>
#elif TCOS_FUJITSU
#   include <sys/types.h>
#   include <sys/resource.h>
#   include <sys/time.h>
#   include <sys/stat.h>
#   include <unistd.h>
#   include <pwd.h>
#   include <grp.h>
#   define _SYS_NMLN  9
struct utsname {
   char  sysname[_SYS_NMLN];
   char  nodename[_SYS_NMLN];
   char  release[_SYS_NMLN];
   char  version[_SYS_NMLN];
   char  machine[_SYS_NMLN];
};
extern "C" int uname(struct utsname *);
#   include <time.h>
#   include <signal.h>
#   include <fcntl.h>
#   include <errno.h>
#else
#   include <sys/utsname.h>
#   include <sys/resource.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <sys/ioctl.h> 
#   include <arpa/inet.h> 
#   include <net/if.h>
#   include <unistd.h>
#   include <pwd.h>
#   include <grp.h>
#   include <time.h>
#   include <signal.h>
#   include <fcntl.h>
#   include <errno.h>
#   include <cstdio>
#   include <string.h>
#   include <stdlib.h>
#endif

//----------------------------------------
// includes for time functions
//----------------------------------------
#ifdef TCOS_LINUX
#include <fstream>
#endif

//----------------------------------------
// includes for getting out number of cpus
//----------------------------------------
#ifdef TCOS_IBM
#include <sys/systemcfg.h>
#include <nlist.h>
#endif

#ifdef TCOS_SGI
#include <sys/sysmp.h>
#endif

#ifdef TCOS_HP
#include <sys/param.h>
#include <sys/pstat.h>
#endif

#ifdef TCOS_ALPHA
#include <sys/sysinfo.h>
#include <machine/hal_sysinfo.h>
#endif

#ifdef TCOS_WINDOWS
#undef GetUserName
#endif

#include "TCNewEnable.h"

#define SYS_CHAR_LEN 512

namespace tc
{

   std::string system::GetHostName()
   {
#ifdef TCOS_WINCE_40
      /** @todo find lib for methode gethostnam on wince */
#elif TCOS_WINDOWS
      char FAR name[SYS_CHAR_LEN];
      unsigned long len = SYS_CHAR_LEN;

      if (::GetComputerNameA(name, &len))
         return name;
#else
      struct utsname unamedata;
      std::memset((void*)&unamedata, 0, sizeof(unamedata));

      if (uname(&unamedata) != -1)
      {
         return (std::string)unamedata.nodename;
      }
#endif
      return "";
   }

   std::string system::GetUserName()
   {
#ifdef TCOS_WINCE_40
      return "";
#elif TCOS_WINDOWS
      char name[SYS_CHAR_LEN];
      unsigned long len = SYS_CHAR_LEN;

      if (::GetUserNameA(name, &len))
      {
         return name;
      }
#else
      struct passwd *pwd = ::getpwuid(::geteuid());
      if (pwd)
      {
         return pwd->pw_name;
      }
#endif

      return "";
   }

   std::string system::GetOSName()
   {
#if TCOS_WINDOWS || TCOS_WINCE_40
      OSVERSIONINFO info;
      std::memset((void*)&info, 0, sizeof(info));

      info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      ::GetVersionEx(&info);

      //VER_PLATFORM_TCOS_WINDOWS_WINDOWS
      if (info.dwPlatformId == VER_PLATFORM_WIN32_NT)
      {
         if (info.dwMajorVersion == 6)
            return "Windows Vista";
         else if (info.dwMajorVersion == 5 && info.dwMinorVersion > 0)
            return "Windows XP";
         else if (info.dwMajorVersion == 5)
            return "Windows 2000";
         else
            return "Windows NT";
      }
#ifdef TCOS_WINCE_40
      if (info.dwPlatformId == VER_PLATFORM_WIN32_CE)
         return "Windows CE";
#endif
#else
      struct utsname unamedata;
      std::memset((void*)&unamedata, 0, sizeof(unamedata));

      if (::uname(&unamedata) != -1)
      {
         return (std::string)unamedata.sysname;
      }
#endif
      return "unknown";
   }

   std::string system::GetOSVersion()
   {
      char ver[SYS_CHAR_LEN];

#if TCOS_WINDOWS || TCOS_WINCE_40
      OSVERSIONINFO info;
      std::memset((void*)&info, 0, sizeof(info));

      info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      GetVersionEx(&info);

      string::Snprintf(ver, util::ArraySize(ver), "%d.%.d.%d (%s)", info.dwMajorVersion,
         info.dwMinorVersion,
         info.dwBuildNumber,
         info.szCSDVersion);
#else
      struct utsname unamedata;
      std::memset((void*)&unamedata, 0, sizeof(unamedata));

      if (::uname(&unamedata) == -1)
         return "";

#ifdef TCOS_IBM
      string::Snprintf(ver, util::ArraySize(ver), "%s.%s", unamedata.version, unamedata.release);
#elif TCOS_LINUX
      string::Snprintf(ver, util::ArraySize(ver), "%s", unamedata.release);
#else
      string::Snprintf(ver, util::ArraySize(ver), "%s %s", unamedata.release, unamedata.version);
#endif
#endif
      return ver;
   }

   std::string system::GetOSCompany()
   {
#if TCOS_WINDOWS || TCOS_WINCE_40
      return "Microsoft";
#elif TCOS_IBM
      return "IBM";
#elif TCOS_SGI
      return "Silicon Graphics";
#elif TCOS_HP
      return "HP";
#elif TCOS_ALPHA
      return "Digital";
#elif TCOS_SUN
      return "Sun";
#elif TCOS_CRAY
      return "Cray";
#elif TCOS_FUJITSU
      return "Fujitsu";
#elif TCOS_CYGWIN
      return "Gnu";
#elif TCOS_LINUX
      if (file::IsFile("/etc/SuSE-release"))
         return "SuSE";
      else if (file::IsFile("/etc/redhat-release"))
         return "RedHat";
      else
         return "unknown";
#elif TCOS_QNX
      return "QNX";
#else
#error "unknown TCOS_TYPE"
#endif
   }

   std::string system::GetCpuModel()
   {
#if TCOS_WINDOWS || TCOS_WINCE_40
      SYSTEM_INFO sinfo;
      std::memset(&sinfo, 0, sizeof(sinfo));
      ::GetSystemInfo(&sinfo);

      char ver[SYS_CHAR_LEN];
      string::Snprintf(ver, util::ArraySize(ver), "Processor %d Level %d", sinfo.dwProcessorType,
         sinfo.wProcessorLevel);
      return ver;
#else
      struct utsname unamedata;
      std::memset(&unamedata, 0, sizeof(unamedata));

      if (::uname(&unamedata) != -1)
         return (std::string)unamedata.machine;

      return "";
#endif
   }

   std::string system::GetDate()
   {
      char date[SYS_CHAR_LEN];

      time_t timeNow   = ::time(0);
      struct tm* tmNow = ::localtime(&timeNow);

      ::strftime(date, sizeof(date)-1, "%d-%b-%Y %H:%M:%S", tmNow);

      return date;
   }

   std::string system::GetEnvironment(const std::string& ename)
   {
#if defined TCOS_WINCE_40 || defined TCOS_WINDOWS
      DWORD size = ::GetEnvironmentVariableA(ename.c_str(), 0, 0);
      size++;
      char* evalue = new char[size];
      ScopedArray<char> evalue_ptr(evalue);
      ::GetEnvironmentVariableA(ename.c_str(), evalue, size);
#else
      const char *evalue = ::getenv(ename.c_str());
#endif
      if (!evalue)
      {
         return "";
      }
      return evalue;
   }

   uint32_t system::GetProcessID()
   {
#if TCOS_WINDOWS || TCOS_WINCE_40
      return GetCurrentProcessId();
#else
      return getpid();
#endif
   }

   void system::Sleep(uint64_t millisec)
   {
#if TCOS_WINDOWS || TCOS_WINCE_40
#define MAX_SLEEP_MILLI_SECONDS 0xffffffe   // 2**32-2
      if (millisec <= MAX_SLEEP_MILLI_SECONDS)
      {
         ::Sleep(static_cast<DWORD>(millisec));
         return;
      }

      uint64_t no_of_max_sleeps = millisec / MAX_SLEEP_MILLI_SECONDS;
      for (uint64_t i = 0; i < no_of_max_sleeps; i++)
      {
         ::Sleep(MAX_SLEEP_MILLI_SECONDS);
      }

      ::Sleep(static_cast<DWORD>(millisec % MAX_SLEEP_MILLI_SECONDS));
#elif TCOS_CRAY || TCOS_FUJITSU
      uint64_t t = GetWallTime();
      while (GetWallTime(t) < millisec);
#else
      timespec rqts;
      timespec remain;

      rqts.tv_sec  = millisec / 1000;
      rqts.tv_nsec = (millisec % 1000)* 1000000;
      while (nanosleep(&rqts, &remain))
      {
         rqts.tv_nsec = remain.tv_nsec;
      }
#endif
   }

   int32_t system::GetLastError()
   {
#if TCOS_WINDOWS || TCOS_WINCE_40
      return ::GetLastError();
#else
      return errno;
#endif
   }

   std::string system::GetLastErrorMessage()
   {
      return GetErrorMessage(GetLastError());
   }

   std::string system::GetErrorMessage(int32_t error_code)
   {
#if TCOS_WINDOWS || TCOS_WINCE_40
      LPSTR lpMsgBuf;

      FormatMessageA(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | 
         FORMAT_MESSAGE_FROM_SYSTEM,
         0,
         error_code,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         (LPSTR) &lpMsgBuf,
         0, 0 );

      std::string message = lpMsgBuf;
      message = string::Replace(message, '\r', ' ');
      message = string::Replace(message, '\n', ' ');

      LocalFree(lpMsgBuf);
#else
      std::string message(strerror(error_code));
#endif
      return message;
   }


   int32_t system::GetNumCPUs()
   {
      int32_t nCPU = 0;
#ifdef TCOS_LINUX
      std::ifstream file("/proc/stat");
      if (!file) return 1;

      char buffer[1024];
      while (!file.eof())
      {
         file.getline(buffer, 1024);
         if (!file.eof())
            if (strncmp(buffer, "cpu", 3) == 0 && buffer[3] != ' ') nCPU++;
      }

#elif TCOS_IBM
      struct nlist info;
      int32_t fd;

      info.n_name = "_system_configuration";

      fd = open("/dev/kmem",O_RDONLY);

      knlist(&info,1,sizeof(struct nlist));
      lseek(fd,info.n_value,0);
      read(fd,&_system_configuration,sizeof(_system_configuration));
      close(fd);

      nCPU = _system_configuration.ncpus;

#elif TCOS_SGI
      nCPU = (int32_t)sysmp(MP_NPROCS);

#elif TCOS_HP
      struct pst_dynamic buf;

      pstat_getdynamic(&buf, sizeof(buf), 1, 0);
      nCPU = buf.psd_proc_cnt;

#elif TCOS_ALPHA
      int32_t start = 0;
      struct cpu_info info;

      getsysinfo(GSI_CPU_INFO, (caddr_t)&info, sizeof(info), &start, 0);
      nCPU = info.cpus_in_box;

#elif TCOS_SUN
      nCPU = sysconf(_SC_NPROCESSORS_CONF);
#elif TCOS_WINDOWS || TCOS_WINCE_40
      SYSTEM_INFO sinfo;
      GetSystemInfo(&sinfo);
      nCPU = sinfo.dwNumberOfProcessors;
#elif TCOS_CRAY
      nCPU = sysconf(_SC_CRAY_NCPU);
#endif
      return util::Max(1, nCPU);
   }

   uint64_t system::GetCpuTime(uint64_t timeIn)
   {
      uint64_t dt;
#if defined(TCOS_WINDOWS) || defined(TCOS_WINCE_40)
      dt = GetTickCount() / CLOCKS_PER_SEC * 1000 - timeIn;
#elif defined(TCOS_CRAY) || defined(TCOS_FUJITSU)
      dt = clock() / CLOCKS_PER_SEC * 1000 - timeIn;
#else
      struct rusage rusage;

      getrusage(RUSAGE_SELF, &rusage);

      uint64_t q = rusage.ru_utime.tv_sec * 1000;

      dt = q + rusage.ru_utime.tv_usec / 1000 - timeIn;
#endif

      return dt;
   }

   uint32_t system::GetCurrentThreadID()
   {
#if TCOS_WINDOWS
      return static_cast<uint32_t>(::GetCurrentThreadId());
#elif defined TCOS_POSIX
      return static_cast<uint32_t>(::pthread_self());
#endif
   }

   bool system::GetNetworkDeviceInfos(std::vector<NetworkDeviceInfo>& infos)
   {
#if TCOS_WINDOWS
      ULONG size = 0;
      ::GetAdaptersInfo( 0, &size);
      IP_ADAPTER_INFO* adapters = (IP_ADAPTER_INFO*)new uint8_t[size];
      if (::GetAdaptersInfo(adapters, &size) != NO_ERROR)
      {
         return false;
      }

      IP_ADAPTER_INFO* current_adapter = adapters;
      while (current_adapter)
      {
         std::string mac_address;
         for (uint32_t i = 0; i < current_adapter->AddressLength ; i++)
         {
            mac_address += string::Print("%.2X", current_adapter->Address[i]);
            if (i != current_adapter->AddressLength - 1)
            {
               mac_address += "-";
            }
         }

         NetworkDeviceInfo info = {mac_address, current_adapter->IpAddressList.IpAddress.String};
         infos.push_back(info);

         current_adapter = current_adapter->Next;
      }

      delete[] (uint8_t*)adapters;
#else
      for (int ed=0; ed<10; ed++)
      {
         std::string dev_name = string::Print("eth%d", ed);
         int s = ::socket(PF_INET, SOCK_DGRAM, 0);

         struct ifreq buffer;
         std::memset(&buffer, 0, sizeof(buffer));
         strcpy(buffer.ifr_name, dev_name.c_str());

         if (::ioctl(s, SIOCGIFHWADDR, &buffer) != -1)
         {
            std::string mac_address;
            for(int i = 0; i<6; i++)
            {
               mac_address += string::Print("%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[i]);
               if (i != 6 - 1)
               {
                  mac_address += "-";
               }
            }

            std::memset(&buffer, 0, sizeof(buffer));
            std::strcpy(buffer.ifr_name, dev_name.c_str());
            ::ioctl(s, SIOCGIFADDR, &buffer);
            in_addr ip_address_in = (reinterpret_cast<sockaddr_in *>(&buffer.ifr_ifru.ifru_addr))->sin_addr;
            std::string ip_address = inet_ntoa(ip_address_in);

            NetworkDeviceInfo info = {mac_address, ip_address};
            infos.push_back(info);
         }
         close(s);
      }
#endif

      return true;
   }

   static system::ConsoleStopHandlerPtr s_stop_handler;
   static void ConsoleSignalHandler(int type)
   {
      ::signal(type, SIG_DFL);
      if (s_stop_handler)
      {
         s_stop_handler->OnConsoleStop();
      }
   }

   system::ConsoleStopHandlerPtr system::SetConsoleStopHandler(system::ConsoleStopHandlerPtr handler)
   {
      std::swap(s_stop_handler, handler);
      if (s_stop_handler)
      {
         ::signal(SIGINT, ConsoleSignalHandler);
         ::signal(SIGTERM, ConsoleSignalHandler);
#ifdef TCOS_POSIX
         ::signal(SIGQUIT, ConsoleSignalHandler);
#elif defined TCOS_WINDOWS
         ::signal(SIGBREAK, ConsoleSignalHandler);
#endif
      }
      else
      {
         ::signal(SIGINT, SIG_DFL);
         ::signal(SIGTERM, SIG_DFL);
#ifdef TCOS_POSIX
         ::signal(SIGQUIT, SIG_DFL);
#elif defined TCOS_WINDOWS
         ::signal(SIGBREAK, SIG_DFL);
#endif
      }
      return handler;
   }

   std::string system::GetTmpDir()
   {
      std::string dir = GetEnvironment("TEMP");
      if (dir.length() == 0)
      {
         dir = GetEnvironment("TMP");
      }

      if (dir.length() == 0)
      {
#if TCOS_POSIX
         dir = "/tmp";
#else
         dir = file_name::AddPaths(GetEnvironment("SystemRoot"), "TEMP");
#endif
      }

      if (!file::IsDirectory(dir))
      {
         throw Exception("Unable to determine temp directory");
      }

      return dir;
   }

   std::string system::GetTmpFileName()
   {
      static interlocked::Type s_tmp_file_count;
      interlocked::Type current = interlocked::Increment(s_tmp_file_count);

      std::string id = string::Print("%d_%d", GetProcessID(), current);
      std::string dir = GetTmpDir();
      std::string file_name = file_name::AddFileNameAndPath("tcs_temp_file_" + id, dir);
      file_name = file_name::AddFileNameAndExtension(file_name, "tmp");

      return file_name;
   }
}
