#include "TCString.h"
#include "TCOutput.h"

#include "TCDebugNew.h"

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
   std::string file_name_in("PSK_Umsatzliste.csv");
   std::string file_name_out("PSK_Import.csv");

   tc::StreamPtr file_in = tc::factory::CreateFileStream(file_name_in, tc::Stream::STREAM_READ, tc::factory::CreateAsciiCodec());
   tc::StreamPtr file_out = tc::factory::CreateFileStream(file_name_out, tc::Stream::STREAM_WRITE, tc::factory::CreateAsciiCodec());

   file_out << "konto_nummer;buchungs_datum;betrag;auftraggeber;verwendung;\r\n";

   while (file_in->IsOk())
   {
      std::string line;
      file_in >> line;
      if (line.length())
      {
         ProcessCsvLine(line, file_out);
      }
   }
}

int main(int /*narg*/, char** /*argv*/)
{
   try
   {
      ConvertPSKCvsToHaushaltsbuchCvs();
   }

   catch (tc::Exception& e)
   {
      TCERRORS("TestBasic", "Unhandled Exception: ");
      TCERRORS("TestBasic", "\t" << e.what());
   }

   return 0;
}


