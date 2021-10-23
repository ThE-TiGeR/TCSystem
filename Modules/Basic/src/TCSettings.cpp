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
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
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
//  $Id: TCSettings.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCSettings.h"

#include "TCString.h"

#include "TCNewEnable.h"

namespace tc
{
const uint32_t NOT_FOUND = 0xffffffff;

Settings::Settings()
:m_section_string_hash_table(20, NOT_FOUND)
{
}

Settings::~Settings()
{
   for (uint32_t i=0; i<m_key_string_hash_table.size(); i++)
      delete m_key_string_hash_table[i];
}

bool Settings::GetBoolEntry(const std::string &section, const std::string &key, bool defaultValue)  const
{
   return string::ToBool(GetStringEntry(section, key, string::ToString(defaultValue)));
}

int32_t Settings::GetIntEntry(const std::string &section, const std::string &key, int32_t defaultValue) const
{
   return string::ToSint32(GetStringEntry(section, key, string::ToString(defaultValue)));
}

float Settings::GetFloatEntry(const std::string &section, const std::string &key, float defaultValue) const
{
   return string::ToFloat(GetStringEntry(section, key, string::ToString(defaultValue)));
}

double Settings::GetDoubleEntry(const std::string &section, const std::string &key, double defaultValue) const
{
   return string::ToDouble(GetStringEntry(section, key, string::ToString(defaultValue)));
}

std::string Settings::GetStringEntry(const std::string &section, const std::string &key, const std::string &defaultValue) const
{
   uint32_t pos = m_section_string_hash_table.GetMappedValue(section);
   // if did not find the section so we have to create a new one
   if (pos != NOT_FOUND)
   {
      HashTable<std::string, uint32_t> *sHash = m_key_string_hash_table[pos];
      // if not found we return the value
      pos = sHash->GetMappedValue(key);
      if (pos != NOT_FOUND) return m_data_of_string[pos];
   }
   
   // return the default value
   return defaultValue;
}

void Settings::SetBoolEntry(const std::string &section, const std::string &key, bool value)
{
   SetStringEntry(section, key, string::ToString(value));
}

void Settings::SetIntEntry(const std::string &section, const std::string &key, int32_t value)
{
   SetStringEntry(section, key, string::ToString(value));
}

void Settings::SetFloatEntry(const std::string &section, const std::string &key, float value)
{
   SetStringEntry(section, key, string::ToString(value));
}

void Settings::SetDoubleEntry(const std::string &section, const std::string &key, double value)
{
   SetStringEntry(section, key, string::ToString(value));
}

void Settings::SetStringEntry(const std::string &section, const std::string &key, const std::string &value)
{
   HashTable<std::string, uint32_t>* sHash = getSectionHashTable(section, true);
 
   uint32_t pos = sHash->GetMappedValue(key);
   // if did not find the key in the section so we have to add the key
   if (pos == NOT_FOUND) pos = addEntryToSection(sHash, key);

   m_data_of_string[pos] = value;
}


HashTable<std::string, uint32_t>* Settings::getSectionHashTable(const std::string &section, bool createNewSection)
{
   uint32_t pos = m_section_string_hash_table.GetMappedValue(section);
   // if did not find the section so we have to create a new one
   if (pos == NOT_FOUND && createNewSection)
   {
      pos = static_cast<uint32_t>(m_key_string_hash_table.size());

      m_section_string_hash_table.AddKey(section, pos);
      m_key_string_hash_table.push_back(new HashTable<std::string, uint32_t>(100, NOT_FOUND));
      m_section_name.push_back(section);
   }
   else if (createNewSection == false)
      return 0;

   return m_key_string_hash_table[pos];
}

int32_t Settings::addEntryToSection(HashTable<std::string, uint32_t> *sHash, const std::string &key)
{
   uint32_t pos = static_cast<uint32_t>(m_data_of_string.size());
   sHash->AddKey(key, pos);

   m_data_of_string.push_back("");
   return pos;
}

bool Settings::ReadFromStream(StreamPtr /*stream*/)
{
   return false;
}

bool Settings::WriteOnStream(StreamPtr /*stream*/) const
{
/*
   if (stream.IsModeBinary())
   {
      for (uint32_t s=0; s<m_key_string_hash_table.size(); s++)
      {
         const TCArray< std::vector< TCStringToIntHashElem > > &elements = m_key_string_hash_table[s]->GetElements();
         stream << m_section_name[s];

         for (uint32_t i=0; i<elements.GetSize(); i++)
            for (uint32_t j=0; j<elements[i].size(); j++) {
               stream << elements[i][j].m_string;
               stream << m_data_of_string[elements[i][j].m_Int];
            }
      }
   }
   else
   {
      for (uint32_t s=0; s<m_key_string_hash_table.size(); s++)
      {
         const TCArray< std::vector< TCStringToIntHashElem > > &elements = m_key_string_hash_table[s]->GetElements();
         stream << "[" << m_section_name[s] << "]" << endl;

         for (uint32_t i=0; i<elements.GetSize(); i++)
            for (uint32_t j=0; j<elements[i].size(); j++) {
               stream << elements[i][j].m_string << " = "
                      << m_data_of_string[ elements[i][j].m_Int]
                      << endl;
             }
         stream << endl;
      }
   }
*/
   return false;
}

void Settings::Clear()
{
   m_section_string_hash_table.Clear();
   m_section_name.clear();
   for (uint32_t i=0; i<m_key_string_hash_table.size(); i++)
      m_key_string_hash_table[i]->Clear();

   m_key_string_hash_table.clear();
   m_data_of_string.clear();
}

}
