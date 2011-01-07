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
   TC::uint32 src_len = static_cast<TC::uint32>(str.length());
   TC::uint32 dst_len = (src_len+2)/3*4;
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

class HttpPageReceiver: public TC::Net::SocketServer::DataReceiver
{
public:
   HttpPageReceiver(TC::Net::ReadWriteSocketPtr socket)
      :m_socket(socket),
      m_header_finished(false),
      m_current_frame(),
      m_data_received_event()
   {
      m_data_received_event = TC::MT::Factory::CreateEvent(false, false);
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
   bool Receive(TC::Net::ReadWriteSocketPtr socket, std::string& data_received)
   {
      char data[100];
      TC::uint64 data_len = socket->ReadBytes(data, sizeof(data)-1);
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

   bool ReceiveHeader(TC::Net::ReadWriteSocketPtr socket)
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
            TC::String::Split(header_data, header_end, header, content);
            TC::String::Split(header, "\r\n", m_current_frame.m_header_lines);
            m_current_frame.m_content = content;
            m_header_finished = true;
            TCTRACES("TestBasic", 5,header);
            return true;
         }
      }
   }

   bool ReceiveContent(TC::Net::ReadWriteSocketPtr socket)
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

   TC::Net::ReadWriteSocketPtr m_socket;
   // if header of frame is finished
   bool m_header_finished;
   // frame which is currently received
   HttpFrame m_current_frame;
   // data of last page + event when received
   TC::MT::EventPtr m_data_received_event;
};

//
class UDPReceiver: public TC::Net::SocketServer::DataReceiver
{
public:
   UDPReceiver(TC::Net::BroadcastReadSocketPtr socket)
      :m_socket(socket)
   {
   }

   virtual bool OnNewData()
   {
      char buffer[200];
      TC::Net::Address address;
      TC::uint64 len = m_socket->ReadBytesFrom(buffer, TC::Util::ArraySize(buffer)-1, address);
      if (len == 0)
      {
         return false;
      }

      buffer[len] = 0;

      TCTRACES("UDP", 1, buffer << " from " << address);
      return true;
   }
private:
   TC::Net::BroadcastReadSocketPtr m_socket;
};

class MTTraceTarget: public TC::Output::PrintTarget,
   public TC::MT::ObjectLevelLockable<MTTraceTarget>
{
public:
   MTTraceTarget()
   {
      m_stream = TC::Factory::CreateStdOutStream();
   }
   virtual void Print(const char* text)
   {
      Locker lock(this);
      m_stream << text << TC::endl;
   }
private:
   TC::StreamPtr m_stream;
};

static void RunSocketTest(int narg, char** argv)
{
   TC::Time time = TC::Time::Since(TC::Time::Now());

   bool use_proxy = false;
   std::string user_name = "gg\\g017421";
   std::string passwd = "thom1234";

   if (narg == 2 && argv[1][0] == 'p')
   {
      use_proxy = true;
   }

   TC::Net::PortNumber port[] ={ 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
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
   TC::Net::SocketServerPtr server = TC::Net::Factory::CreateSocketServer();
   server->Start(true);

   TC::RngPtr rng = TC::Factory::Create69069Rng();
   TC::MT::EventPtr event = TC::MT::Factory::CreateEvent(false, false);
   TC::Time next_access;
   while (event->TryWait(next_access) == false)
   {
      TC::uint64 idx = rng->GetRandomNumber(0, TC::Util::ArraySize(port)-1);

      TC::Net::ReadWriteSocketPtr socket;

      TCTRACES("TestBasic", 5,"connecting ... ");
      if (use_proxy)
      {
         socket = TC::Net::Factory::Connect(TC::Net::Address("BSISAP05"), 3128, TC::Net::TCP);
         if (!socket)
         {
            TCTRACES("TestBasic", 5,"failed connecting");
            continue;
         }

      }
      else
      {
         socket = TC::Net::Factory::Connect(TC::Net::Address(host_name[idx]), port[idx], TC::Net::TCP);
         if (!socket)
         {
            TCTRACES("TestBasic", 5,"failed connecting");
            continue;
         }
      }
      TCTRACES("TestBasic", 5,"done.");

      TC::SharedPtr<HttpPageReceiver> receiver(new HttpPageReceiver(socket));
      server->AddSocket(socket, receiver);

      std::string command;
      command = std::string("GET ") + "http://" + host_name[idx] + file_name[idx] + " HTTP/1.1\r\n";
      if (use_proxy)
      {
         command += "Proxy-Authorization: Basic " + make_base64_string(user_name + ":" + passwd) + "\r\n";
      }
      command += "Host: " + host_name[idx] + ":" + TC::String::ToString(port[idx]) + "\r\n";
      command += "Connection: Close Connection\r\n";
      command += "\r\n";

      TCTRACES("TestBasic", 5,"Loading page http://" << host_name[idx] << file_name[idx] << " ");
      if (socket->WriteBytes(command.c_str(), static_cast<TC::uint32>(command.length())) != command.length())
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
      TCTRACES("TestBasic", 5,"  received page size was " << static_cast<TC::uint32>(data.length()));

      server->RemoveSocket(socket);

      next_access = TC::Time::FromMilliSeconds(rng->GetRandomNumber(500, 5000));
      TCTRACES("TestBasic", 5,"next page access in " << next_access.ToMilliSeconds() << " milli seconds");
   }
}

static void RunUDPTest()
{
   TC::Net::BroadcastReadSocketPtr socket = TC::Net::Factory::CreateUdpListenSocket(8042, 10);
   TC::SharedPtr<UDPReceiver> receiver(new UDPReceiver(socket));

   TC::Net::SocketServerPtr server = TC::Net::Factory::CreateSocketServer();
   server->AddSocket(socket, receiver);
   server->Start(true);

   TC::Net::BroadcastWriteSocketPtr bs = TC::Net::Factory::CreateUdpBroadcastSocket();
   for (TC::uint32 i=0; i<100, bs->IsOpened(); i++)
   {
      std::string test("hello world");
      bs->WriteBytesTo(test.c_str(), test.length()+1, TC::Net::Address::GetAnyAddress(), 8042);
      TC::System::Sleep(1000);
   }

   server->RemoveSocket(socket);
   server->Stop(true);
}

static char s_charset[] = "0123456789.";
static TC::uint32 s_num_chars = sizeof(s_charset) - 1;
class PassWordFinder
{
public:
   PassWordFinder(TC::uint32 min_pwd_len, TC::uint32 max_pwd_len)
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
      for (TC::uint32 i=0; i<m_current_pwd_len; i++)
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
      for (TC::uint32 i=0; i<m_current_pwd_len; i++)
      {
         m_current_char[i] = 0;
      }
   }

   void SetPassword(char* password)
   {
      for (TC::uint32 i=0; i<m_current_pwd_len; i++)
      {
         password[i] = s_charset[m_current_char[i]];
      }

      password[m_current_pwd_len] = 0;
   }

private:
   TC::uint32 m_min_pwd_len;
   TC::uint32 m_max_pwd_len;
   TC::uint32 m_current_pwd_len;
   std::vector<TC::uint32> m_current_char;
};

static void TestFindPassword()
{
   TC::uint64 num = 0;
   char pwd[10];
   PassWordFinder pwd_finder(1, 8);
   TC::Time start(TC::Time::Now());
   while (pwd_finder.NextPassword(pwd))
   {
      num++;
   }

   TCINFOS("TestBasic", pwd << " " << num << " " << TC::Time::Since(start));
}

static void ProcessBuchungsText(const std::string& buchungs_text, TC::StreamPtr file_out)
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
      verwendung = TC::String::TrimmSpaces(verwendung);

      pos = part2.find_first_not_of(" 1234567890|");
      if (pos != std::string::npos)
      {
         auftraggeber = part2.substr(pos);
         auftraggeber = TC::String::TrimmSpaces(auftraggeber);

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
      throw TC::Exception("Buchungs Text parsing error");
   }

   file_out << auftraggeber << ";" << verwendung << ";";
}

static void ProcessCsvLine(const std::string& line, TC::StreamPtr file_out)
{
   // [Kontonummer];[Buchungstext];[Buchungsdatum];[Valutadatum];[Betrag mit Vorzeichen];[Kontowährung]
   std::vector<std::string> parts;
   TC::String::Split(line, ";", parts);

   std::string konto_nummer    = parts[0];
   std::string buchungs_text   = parts[1];
   std::string buchungs_datum  = parts[2];
   std::string valuta_datum    = parts[3];
   std::string betrag          = TC::String::Replace(parts[4], ".", "");
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

   TC::StreamPtr file_in = TC::Factory::CreateFileStream(file_name_in, TC::Stream::stream_read, TC::Factory::CreateAsciiCodec());
   TC::StreamPtr file_out = TC::Factory::CreateFileStream(file_name_out, TC::Stream::stream_write, TC::Factory::CreateAsciiCodec());

   file_out << "konto_nummer;buchungs_datum;betrag;auftraggeber;verwendung;\r\n";

   while (file_in->IsOk())
   {
      std::string line;
      file_in >> line;
      if (line.length())
      {
         //ProcessCsvLine(TC::String::ToString(line), file_out);
         ProcessCsvLine(line, file_out);
      }
   }
}

static void GzStreamTest()
{
    TC::StreamPtr stream =TC::Factory::CreateGzFileStream("test.txt.gz", TC::Stream::stream_write, 
        TC::Factory::CreateAsciiCodec());
    stream << "Hallo" << TC::endl;
    stream << "This is a gz test" << TC::endl;
    stream << 1 << " " << 127 << TC::endl;
}

static void Bz2StreamTest()
{
    TC::StreamPtr stream =TC::Factory::CreateBz2FileStream("test.txt.bz2", TC::Stream::stream_write, 
        TC::Factory::CreateAsciiCodec());
    stream << "Hallo" << TC::endl;
    stream << "This is a bz2 test" << TC::endl;
    stream << 1 << " " << 127 << TC::endl;
}

int main(int narg, char** argv)
{
   TC::Output::PrintTargetPtr trace_target(new MTTraceTarget);
   TC::Output::SetErrorTarget(trace_target);
   TC::Output::SetWarningTarget(trace_target);
   TC::Output::SetInfoTarget(trace_target);
   TC::Output::SetTraceTarget(trace_target);
   TC::Output::SetTraceLevel(50);

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

   catch (TC::Exception& e)
   {
      TCERRORS("TestBasic", "Unhandled Exception: ");
      TCERRORS("TestBasic", "\t" << e.what());
   }


   TC::Output::SetErrorTarget(TC::Output::PrintTargetPtr());
   TC::Output::SetWarningTarget(TC::Output::PrintTargetPtr());
   TC::Output::SetInfoTarget(TC::Output::PrintTargetPtr());
   TC::Output::SetTraceTarget(TC::Output::PrintTargetPtr());

   return 0;
}


