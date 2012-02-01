#include "TCException.h"
#include "TCFile.h"
#include "TCFileName.h"
#include "TCNetFactory.h"
#include "TCNetSocket.h"
#include "TCNetSocketServer.h"
#include "TCMTFactory.h"
#include "TCMTLockable.h"
#include "TCOutput.h"
#include "TCString.h"
#include "TCWString.h"
#include "TCTime.h"
#include "TCUtil.h"
#include "TCSystem.h"

#include <algorithm>
#include <cmath>

#include "TCDebugNew.h"

std::string make_base64_string(const std::string& str)
{
   const char *base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   /* make base64 string */
   tc::uint32 src_len = static_cast<tc::uint32>(str.length());
   tc::uint32 dst_len = (src_len+2)/3*4;
   char* buf = new char[dst_len+1];

   int bits = 0;
   int data = 0;
   const char* src = str.c_str();
   char* dst = buf;
   while ( dst_len-- )
   {
      if ( bits < 6 )
      {
         data = (data << 8) | *src;
         bits += 8;
         if ( *src != 0 )
         {
            src++;
         }
      }
      *dst++ = base64_table[0x3F & (data >> (bits-6))];
      bits -= 6;
   }
   *dst = '\0';
   /* fix-up tail padding */
   switch ( src_len%3 ) 
   {
   case 1:
      *--dst = '=';
   case 2:
      *--dst = '=';
   }

   std::string temp = buf;
   delete [] buf;
   return temp;
}

class HttpFrame
{
public:
   HttpFrame()
      :m_header_lines(),
      m_content()
   {
   }

   std::vector<std::string> m_header_lines;
   std::string m_content;
};

class HttpPageReceiver: public tc::net::SocketServer::DataReceiver
{
public:
   HttpPageReceiver(tc::net::ReadWriteSocketPtr socket)
      :m_socket(socket),
      m_header_finished(false),
      m_current_frame(),
      m_data_received_event()
   {
      m_data_received_event = tc::multi_threading::factory::CreateEvent(false, false);
   }
   bool GetReceivedData(std::string& data)
   {
      m_data_received_event->Wait();
      data = m_current_frame.m_content;

      return data.size() != 0;
   }
protected:
   virtual bool OnNewData()
   {
      if (!m_header_finished)
      {
         if (!ReceiveHeader(m_socket))
         {
            CancelFrame();
            return false;
         }
      }
      else
      {
         if (!ReceiveContent(m_socket))
         {
            CancelFrame();
            return false;
         }

         TCTRACES("TestBasic", 5,"finished reading data ");
         m_data_received_event->Set();
         m_socket->Close();
         return true;
      }

      return true;
   }

private:
   bool Receive(tc::net::ReadWriteSocketPtr socket, std::string& data_received)
   {
      char data[100];
      tc::uint64 data_len = socket->ReadBytes(data, sizeof(data)-1);
      if (!socket->IsOpened())
      {
         m_data_received_event->Set();
         TCTRACES("TestBasic", 5,"closed ");
         return false;
      }

      if (data_len == 0)
      {
         data_received = "";
         return true;
      }
      data[data_len] = 0;
      data_received = data;

      return true;
   }

   bool ReceiveHeader(tc::net::ReadWriteSocketPtr socket)
   {
      std::string header_data;
      while(true)
      {
         std::string data;
         if (!Receive(socket, data))
         {
            return false;
         }

         header_data += data;
         std::string::size_type header_end = GetHeaderEndPos(header_data);
         if (header_end != std::string::npos)
         {
            std::string header;
            std::string content;
            tc::string::Split(header_data, header_end, header, content);
            tc::string::Split(header, "\r\n", m_current_frame.m_header_lines);
            m_current_frame.m_content = content;
            m_header_finished = true;
            TCTRACES("TestBasic", 5,header);
            return true;
         }
      }
   }

   bool ReceiveContent(tc::net::ReadWriteSocketPtr socket)
   {
      while(true)
      {
         std::string data;
         if (!Receive(socket, data))
         {
            return false;
         }

         m_current_frame.m_content += data;
         if (IsEndOfContent())
         {
            return true;
         }
      }
   }

   std::string::size_type GetHeaderEndPos(const std::string& header_data)
   {
      std::string::size_type pos = 0;
      do
      {
         std::string::size_type allowed_diff = 1;
         std::string::size_type found_pos = header_data.find_first_of("\r\n", pos);
         if (found_pos != std::string::npos)
         {
            if (header_data.at(found_pos) == '\r')
            {
               found_pos++;
               allowed_diff = 2;
            }

            if (found_pos - pos <= allowed_diff)
            {
               return found_pos;
            }
         }
         else
         {
            return std::string::npos;
         }

         pos = found_pos+1;
      }while(true);
   }

   bool CheckHeader()
   {
      if (m_current_frame.m_header_lines[0].find("HTTP/1.1 200 OK") == std::string::npos)
      {
         TCTRACES("TestBasic", 5,"failed reading data");
         return false;
      }

      return true;
   }

   bool IsEndOfContent()
   {
      // TCTRACES("TestBasic", 5,m_current_received_data);
      if (m_current_frame.m_content.find("</html>") != std::string::npos ||
         m_current_frame.m_content.find("</HTML>") != std::string::npos)
      {
         return true;
      }

      return false;
   }

   void CancelFrame()
   {
      m_current_frame = HttpFrame();
      m_data_received_event->Set();
   }

   tc::net::ReadWriteSocketPtr m_socket;
   // if header of frame is finished
   bool m_header_finished;
   // frame which is currently received
   HttpFrame m_current_frame;
   // data of last page + event when received
   tc::multi_threading::EventPtr m_data_received_event;
};

//
class UDPReceiver: public tc::net::SocketServer::DataReceiver
{
public:
   UDPReceiver(tc::net::BroadcastReadSocketPtr socket)
      :m_socket(socket)
   {
   }

   virtual bool OnNewData()
   {
      char buffer[200];
      tc::net::Address address;
      tc::uint64 len = m_socket->ReadBytesFrom(buffer, tc::util::ArraySize(buffer)-1, address);
      if (len == 0)
      {
         return false;
      }

      buffer[len] = 0;

      TCTRACES("UDP", 1, buffer << " from " << address);
      return true;
   }
private:
   tc::net::BroadcastReadSocketPtr m_socket;
};

class MTTraceTarget: public tc::output::PrintTarget,
   public tc::multi_threading::ObjectLevelLockable<MTTraceTarget>
{
public:
   MTTraceTarget()
   {
      m_stream = tc::factory::CreateStdOutStream();
   }
   virtual void Print(const char* text)
   {
      Locker lock(this);
      m_stream << text << tc::endl;
   }
private:
   tc::StreamPtr m_stream;
};

static void RunSocketTest(int narg, char** argv)
{
   tc::Time time = tc::Time::Since(tc::Time::Now());

   bool use_proxy = false;
   std::string user_name = "gg\\g017421";
   std::string passwd = "thom1234";

   if (narg == 2 && argv[1][0] == 'p')
   {
      use_proxy = true;
   }

   tc::net::PortNumber port[] ={ 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
   std::string host_name[] = {
      "tcsystem.sourceforge.net",
      "tcsystem.sourceforge.net",
      "tcsystem.sourceforge.net",
      "tcsystem.sourceforge.net",
      "sourceforge.net",
      "sourceforge.net",
      "sourceforge.net",
      "sourceforge.net",
      "sourceforge.net",
      "sourceforge.net",
      "puzzle.dl.sourceforge.net",
      "puzzle.dl.sourceforge.net",
      "puzzle.dl.sourceforge.net",
      "puzzle.dl.sourceforge.net",
      "puzzle.dl.sourceforge.net",
      "puzzle.dl.sourceforge.net",
      "puzzle.dl.sourceforge.net"
   };
   std::string file_name[] = {
      "/",
      "/modules.html",
      "/namespaces.html",
      "/annotated.html",
      "/projects/tcsystem/",
      "/project/stats/?group_id=73675&ugn=tcsystem",
      "/project/showfiles.php?group_id=73675",
      "/project/showfiles.php?group_id=73675&package_id=73788",
      "/tracker/?group_id=73675&atid=538530",
      "/forum/?group_id=73675",
      "/sourceforge/tcsystem/TCMessenger-0.9.5_src.zip",
      "/sourceforge/tcsystem/TCMessenger-0.9.5_src.tar.gz",
      "/sourceforge/tcsystem/TCFileSync-0.9.5_src.zip",
      "/sourceforge/tcsystem/TCFileSync-0.9.5_src.tar.gz",
      "/sourceforge/tcsystem/TCSystem-1.3.0_doc.zip",
      "/sourceforge/tcsystem/TCSystem-1.3.0_src.zip",
      "/sourceforge/tcsystem/TCSystem-1.3.0_src.tar.gz",
   };

   // create a server and wait until it has started
   tc::net::SocketServerPtr server = tc::net::factory::CreateSocketServer();
   server->Start(true);

   tc::RngPtr rng = tc::factory::Create69069Rng();
   tc::multi_threading::EventPtr event = tc::multi_threading::factory::CreateEvent(false, false);
   tc::Time next_access;
   while (event->TryWait(next_access) == false)
   {
      tc::uint64 idx = rng->GetRandomNumber(0, tc::util::ArraySize(port)-1);

      tc::net::ReadWriteSocketPtr socket;

      TCTRACES("TestBasic", 5,"connecting ... ");
      if (use_proxy)
      {
         socket = tc::net::factory::Connect(tc::net::Address("BSISAP05"), 3128, tc::net::TCP);
         if (!socket)
         {
            TCTRACES("TestBasic", 5,"failed connecting");
            continue;
         }

      }
      else
      {
         socket = tc::net::factory::Connect(tc::net::Address(host_name[idx]), port[idx], tc::net::TCP);
         if (!socket)
         {
            TCTRACES("TestBasic", 5,"failed connecting");
            continue;
         }
      }
      TCTRACES("TestBasic", 5,"done.");

      tc::SharedPtr<HttpPageReceiver> receiver(new HttpPageReceiver(socket));
      server->AddSocket(socket, receiver);

      std::string command;
      command = std::string("GET ") + "http://" + host_name[idx] + file_name[idx] + " HTTP/1.1\r\n";
      if (use_proxy)
      {
         command += "Proxy-Authorization: Basic " + make_base64_string(user_name + ":" + passwd) + "\r\n";
      }
      command += "Host: " + host_name[idx] + ":" + tc::string::ToString(port[idx]) + "\r\n";
      command += "Connection: Close Connection\r\n";
      command += "\r\n";

      TCTRACES("TestBasic", 5,"Loading page http://" << host_name[idx] << file_name[idx] << " ");
      if (socket->WriteBytes(command.c_str(), static_cast<tc::uint32>(command.length())) != command.length())
      {
         TCTRACES("TestBasic", 5,"failed sending command");
         continue;
      }
      TCTRACES("TestBasic", 5,"command send.");

      std::string data;
      if (!receiver->GetReceivedData(data))
      {
         TCTRACES("TestBasic", 5,"failed reading data");
         continue;
      }
      TCTRACES("TestBasic", 5,"done.");
      TCTRACES("TestBasic", 5,"  received page size was " << static_cast<tc::uint32>(data.length()));

      server->RemoveSocket(socket);

      next_access = tc::Time::FromMilliSeconds(rng->GetRandomNumber(500, 5000));
      TCTRACES("TestBasic", 5,"next page access in " << next_access.ToMilliSeconds() << " milli seconds");
   }
}

static void RunUDPTest()
{
   tc::net::BroadcastReadSocketPtr socket = tc::net::factory::CreateUdpListenSocket(8042, 10);
   tc::SharedPtr<UDPReceiver> receiver(new UDPReceiver(socket));

   tc::net::SocketServerPtr server = tc::net::factory::CreateSocketServer();
   server->AddSocket(socket, receiver);
   server->Start(true);

   tc::net::BroadcastWriteSocketPtr bs = tc::net::factory::CreateUdpBroadcastSocket();
   for (tc::uint32 i=0; i<100, bs->IsOpened(); i++)
   {
      std::string test("hello world");
      bs->WriteBytesTo(test.c_str(), test.length()+1, tc::net::Address::GetAnyAddress(), 8042);
      tc::system::Sleep(1000);
   }

   server->RemoveSocket(socket);
   server->Stop(true);
}

static char s_charset[] = "0123456789.";
static tc::uint32 s_num_chars = sizeof(s_charset) - 1;
class PassWordFinder
{
public:
   PassWordFinder(tc::uint32 min_pwd_len, tc::uint32 max_pwd_len)
      :m_min_pwd_len(min_pwd_len),
      m_max_pwd_len(max_pwd_len),
      m_current_pwd_len(0),
      m_current_char(max_pwd_len, 0)
   {
      InitCurrentChar();
   }

   bool NextPassword(char* password)
   {
      if (m_current_pwd_len < m_min_pwd_len)
      {
         m_current_pwd_len = m_min_pwd_len;
         InitCurrentChar();
      }
      else
      {
         if (!NextCharacter())
         {
            m_current_pwd_len++;
            if (m_current_pwd_len > m_max_pwd_len)
            {
               return false;
            }

            InitCurrentChar();
         }
      }

      SetPassword(password);
      return true;
   }

private:
   bool NextCharacter()
   {
      for (tc::uint32 i=0; i<m_current_pwd_len; i++)
      {
         if (m_current_char[i] == s_num_chars-1)
         {
            m_current_char[i] = 0;
         }
         else
         {
            m_current_char[i]++;
            return true;
         }
      }

      return false;
   }

   void InitCurrentChar()
   {
      for (tc::uint32 i=0; i<m_current_pwd_len; i++)
      {
         m_current_char[i] = 0;
      }
   }

   void SetPassword(char* password)
   {
      for (tc::uint32 i=0; i<m_current_pwd_len; i++)
      {
         password[i] = s_charset[m_current_char[i]];
      }

      password[m_current_pwd_len] = 0;
   }

private:
   tc::uint32 m_min_pwd_len;
   tc::uint32 m_max_pwd_len;
   tc::uint32 m_current_pwd_len;
   std::vector<tc::uint32> m_current_char;
};

static void TestFindPassword()
{
   tc::uint64 num = 0;
   char pwd[10];
   PassWordFinder pwd_finder(1, 8);
   tc::Time start(tc::Time::Now());
   while (pwd_finder.NextPassword(pwd))
   {
      num++;
   }

   TCINFOS("TestBasic", pwd << " " << num << " " << tc::Time::Since(start));
}

static void ProcessBuchungsText(const std::string& buchungs_text, tc::StreamPtr file_out)
{
   std::string auftraggeber;
   std::string verwendung;

   std::string::size_type pos = buchungs_text.rfind('/');
   if (pos != std::string::npos)
   {
      std::string part1 = buchungs_text.substr(0, pos);
      std::string part2 = buchungs_text.substr(pos+1);

      pos = part1.rfind(' ');
      verwendung = part1.substr(0, pos);
      verwendung = tc::string::TrimmSpaces(verwendung);

      pos = part2.find_first_not_of(" 1234567890|");
      if (pos != std::string::npos)
      {
         auftraggeber = part2.substr(pos);
         auftraggeber = tc::string::TrimmSpaces(auftraggeber);

         verwendung = auftraggeber + ", " + verwendung;

         auftraggeber = auftraggeber.substr(0, auftraggeber.find('|'));
      }

      if (auftraggeber.empty())
      {
         auftraggeber = verwendung.substr(0, verwendung.find(' '));
      }
   }
   else
   {
      throw tc::Exception("Buchungs Text parsing error");
   }

   file_out << auftraggeber << ";" << verwendung << ";";
}

static void ProcessCsvLine(const std::string& line, tc::StreamPtr file_out)
{
   // [Kontonummer];[Buchungstext];[Buchungsdatum];[Valutadatum];[Betrag mit Vorzeichen];[Kontowährung]
   std::vector<std::string> parts;
   tc::string::Split(line, ";", parts);

   std::string konto_nummer    = parts[0];
   std::string buchungs_text   = parts[1];
   std::string buchungs_datum  = parts[2];
   std::string valuta_datum    = parts[3];
   std::string betrag          = tc::string::Replace(parts[4], ".", "");
   std::string waehrung        = parts[5];

   file_out << konto_nummer << ";" << buchungs_datum << ";" << betrag << ";";
   ProcessBuchungsText(buchungs_text, file_out);
   file_out  << "\r\n";

   TCINFOS("TestBasic", "");
}

static void ConvertPSKCvsToHaushaltsbuchCvs()
{
   std::string file_name_in("D:/Thomas/Dokumente/Finanzen/PSK Import/PSK_Umsatzliste.csv");
   std::string file_name_out("D:/Thomas/Dokumente/Finanzen/PSK Import/test.csv");

   tc::StreamPtr file_in = tc::factory::CreateFileStream(file_name_in, tc::Stream::stream_read, tc::factory::CreateAsciiCodec());
   tc::StreamPtr file_out = tc::factory::CreateFileStream(file_name_out, tc::Stream::stream_write, tc::factory::CreateAsciiCodec());

   file_out << "konto_nummer;buchungs_datum;betrag;auftraggeber;verwendung;\r\n";

   while (file_in->IsOk())
   {
      std::string line;
      file_in >> line;
      if (line.length())
      {
         //ProcessCsvLine(tc::string::ToString(line), file_out);
         ProcessCsvLine(line, file_out);
      }
   }
}

static void GzStreamTest()
{
    tc::StreamPtr stream =tc::factory::CreateGzFileStream("test.txt.gz", tc::Stream::stream_write, 
        tc::factory::CreateAsciiCodec());
    stream << "Hallo" << tc::endl;
    stream << "This is a gz test" << tc::endl;
    stream << 1 << " " << 127 << tc::endl;
}

static void Bz2StreamTest()
{
    tc::StreamPtr stream =tc::factory::CreateBz2FileStream("test.txt.bz2", tc::Stream::stream_write, 
        tc::factory::CreateAsciiCodec());
    stream << "Hallo" << tc::endl;
    stream << "This is a bz2 test" << tc::endl;
    stream << 1 << " " << 127 << tc::endl;
}

int main(int narg, char** argv)
{
   tc::output::PrintTargetPtr trace_target(new MTTraceTarget);
   tc::output::SetErrorTarget(trace_target);
   tc::output::SetWarningTarget(trace_target);
   tc::output::SetInfoTarget(trace_target);
   tc::output::SetTraceTarget(trace_target);
   tc::output::SetTraceLevel(50);

   try
   {
      TCINFOS("TestBasic", "Start");

      //GzStreamTest();
      //Bz2StreamTest();
      //RunUDPTest();
      //RunSocketTest(narg, argv);
      ConvertPSKCvsToHaushaltsbuchCvs();

      TCINFO("TestBasic", "End");
   }

   catch (tc::Exception& e)
   {
      TCERRORS("TestBasic", "Unhandled Exception: ");
      TCERRORS("TestBasic", "\t" << e.what());
   }


   tc::output::SetErrorTarget(tc::output::PrintTargetPtr());
   tc::output::SetWarningTarget(tc::output::PrintTargetPtr());
   tc::output::SetInfoTarget(tc::output::PrintTargetPtr());
   tc::output::SetTraceTarget(tc::output::PrintTargetPtr());

   return 0;
}


