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
#ifndef _TCSETTINGS_H_
#define _TCSETTINGS_H_

#include "TCHashTable.h"
#include "TCNonCopyable.h"
#include "TCStreamAble.h"

namespace tc
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * @brief This file provides the definition of TCSettings
    *
    * @author Thomas Goessler
    */

   /**
    * Class TCSettings handles settings of any kind of object
    * The settings are subclasses in sections
    * To have fast access to the settings a string to int hashtable is used
    */
   class TC_DLL_LOCAL Settings: public StreamAble, protected NonCopyAble
   {
   public:
      /** construct an empty settings object */
      Settings();
      /** destruct an settings object */
      virtual ~Settings();

      /**
       * Get the bool value of a specified key in the specified section
       *
       * @param section Name of the section in which to get the value
       * @param key String for which to get the value
       * @param defaultValue The return value if no value is stored for this key
       * @return The value for the key
       */
      bool GetBoolEntry(const std::string &section, const std::string &key, bool defaultValue) const;
      /**
       * Get the integer value of a specified key in the specified section
       *
       * @param section Name of the section in which to get the value
       * @param key String for which to get the value
       * @param defaultValue The return value if no value is stored for this key
       * @return The value for the key
       */
      sint32 GetIntEntry(const std::string &section, const std::string &key, sint32 defaultValue) const;
      /**
       * Get the double value of a specified key in the specified section
       *
       * @param section Name of the section in which to get the value
       * @param key String for which to get the value
       * @param defaultValue The return value if no value is stored for this key
       * @return The value for the key
       */
      float GetFloatEntry(const std::string &section, const std::string &key, float defaultValue) const;
      /**
       * Get the double value of a specified key in the specified section
       *
       * @param section Name of the section in which to get the value
       * @param key String for which to get the value
       * @param defaultValue The return value if no value is stored for this key
       * @return The value for the key
       */
      double GetDoubleEntry(const std::string &section, const std::string &key, double defaultValue) const;
      /**
       * Get the string value of a specified key in the specified section
       *
       * @param section Name of the section in which to get the value
       * @param key String for which to get the value
       * @param defaultValue The return value if no value is stored for this key
       * @return The value for the key
       */
      std::string GetStringEntry(const std::string &section, const std::string &key, const std::string &defaultValue) const;

      /**
       * Set the bool value of a specified key in the specified section
       *
       * @param section Name of the section in which to set the value
       * @param key String for which to set the value
       * @param value The value to set
       */
      void SetBoolEntry(const std::string &section, const std::string &key, bool value);
      /**
       * Set the integer value of a specified key in the specified section
       *
       * @param section Name of the section in which to set the value
       * @param key String for which to set the value
       * @param value The value to set
       */
      void SetIntEntry(const std::string &section, const std::string &key, sint32 value);
      /**
       * Set the float value of a specified key in the specified section
       *
       * @param section Name of the section in which to set the value
       * @param key String for which to set the value
       * @param value The value to set
       */
      void SetFloatEntry(const std::string &section, const std::string &key, float value);
      /**
       * Set the double value of a specified key in the specified section
       *
       * @param section Name of the section in which to set the value
       * @param key String for which to set the value
       * @param value The value to set
       */
      void SetDoubleEntry(const std::string &section, const std::string &key, double value);
      /**
       * Set the string value of a specified key in the specified section
       *
       * @param section Name of the section in which to set the value
       * @param key String for which to set the value
       * @param value The value to set
       */
      void SetStringEntry(const std::string &section, const std::string &key, const std::string &value);

      /**
       * read the object data from a TCStream
       * @param stream stream where to read the data from
       * @return the stream
       */
      bool ReadFromStream(StreamPtr stream);
      /**
       * write the object data to a TCStream
       * @param stream stream where to write the data to
       * @return the stream
       */
      bool WriteOnStream(StreamPtr stream) const;

      /**
       * clear all settings
       */
      void Clear();

   protected:
      /** @return the section hash table for specified section */
      HashTable<std::string, uint32>* getSectionHashTable(const std::string &section, bool createNewSection);
      /** add the entry to the specified section */
      sint32 addEntryToSection(HashTable<std::string, uint32> *sHash, const std::string &key);

   private:
      /** The hashtable for fast access to the sections */
      HashTable<std::string, uint32> m_section_string_hash_table;
      /** Array of section names */
      std::vector< std::string > m_section_name;
      /** The hashtable for fast access to the keys of each section */
      std::vector< HashTable<std::string, uint32>* > m_key_string_hash_table;
      /** The array of values of all keys converted to strings */
      std::vector< std::string > m_data_of_string;
   };

   /**
    * @}
    */
} // namespace tc

#endif // _TCSETTINGS_H_
