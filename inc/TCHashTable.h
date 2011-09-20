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
// Copyright (C) 2003 - 2010 Thomas Goessler. All Rights Reserved. 
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

#ifndef TC_HASHTABLE_H
#define TC_HASHTABLE_H

#include "TCTypes.h"

#include <vector>
#include <string>
#include <functional>

namespace TC
{
   /**
    * @addtogroup TC_BASE
    * @{
    */

   /**
    * @file
    * @brief This header file provides the definition of the class TC::HashTable
    *
    * @author Thomas Goessler
    */

   /** @brief Functor for calculating a hash value */
   template <class KEY_TYPE> struct Hash { };

   /** @brief Functor specialization (sint8) for calculating a hash value */
   template<> struct Hash<sint8>
   {
       uint32 operator() (sint8 val) const { return val; }
   };
   /** @brief Functor specialization (uint8) for calculating a hash value */
   template<> struct Hash<uint8>
   {
       uint32 operator() (uint8 val) const { return val; }
   };
   /** @brief Functor specialization (sint16) for calculating a hash value */
   template<> struct Hash<sint16>
   {
       uint32 operator() (sint16 val) const { return val; }
   };
   /** @brief Functor specialization (uint16) for calculating a hash value */
   template<> struct Hash<uint16>
   {
       uint32 operator() (uint16 val) const { return val; }
   };
   /** @brief Functor specialization (sint32) for calculating a hash value */
   template<> struct Hash<sint32>
   {
       uint32 operator() (sint32 val) const { return val; }
   };
   /** @brief Functor specialization (uint32) for calculating a hash value */
   template<> struct Hash<uint32>
   {
       uint32 operator() (uint32 val) const { return val; }
   };

   /** @brief Functor specialization (char*) for calculating a hash value */
   template<> struct Hash<char*>
   {
       uint32 operator() (const char* text) const
       {
           const char *s = text;
           uint32 h=0;
           uint32 c;
           while((c=*s++) != '\0')
           {
               h = ((h << 5) + h) ^ c;
           }
           return h;
       }
   };

   /** @brief Functor specialization (std::string) for calculating a hash value */
   template<> struct Hash<std::string>
   {
       uint32 operator() (const std::string& text) const
       {
           const char *s = text.c_str();
           uint32 h=0;
           uint32 c;
           while((c=*s++) != '\0')
           {
               h = ((h << 5) + h) ^ c;
           }
           return h;
       }
   };

   /**
    * Generic hash table
    *
    * Class implements a generic hash table by using a 2-Dimensional array
    * and adding the elements by calculating the first array position because of
    * the modulo of their hash value
    *
    * The key(KEY_TYPE) which is added to the hash table has to implement
    * the compare equal operator and the Hash template for this type
    *
    * Elements can be added/removed in the hash table with  AddKey or RemoveKey
    * To get then the mapped value for the key use the method GetMappedValue
    *
    *
    * @author Thomas Goessler
    */
   template< class KEY_TYPE,
             class MAP_TYPE=sint32,
             class HASH_FUNC=Hash< KEY_TYPE>,
             class KEY_VALUE_COMPARE_FUNC=std::equal_to<KEY_TYPE> >
   class HashTable
   {
   private:
       /**
        * @brief Internal hash table structure which stores
        * the key value and the mapped value of one entry
        */
       struct _Element
       {
           KEY_TYPE key_value;
           MAP_TYPE map_value;
       };

   public:
       /**
        * Construct the hash table with specified hash size
        * @param size The size of the hash table int the first direction
        * @param notFound The value which should be returned when an searched
        *        element is not found in the hash table
        */
       HashTable(uint32 size, const MAP_TYPE& notFound)
           :m_size(size),
            m_not_found(notFound)
       {
           Clear();
       }

       /** Destruct the hash table */
       virtual ~HashTable() {}

       /** @return The size of the hash table in the first direction */
       inline uint32 GetHashSize() const { return m_size; }

       /**
        * Set the size of the hash table
        * @param size The size of the hash table int the first direction
        */
       inline void SetHashSize(uint32 size)
       {
           m_size = size;
           Clear();
       }

       /**
        * Clear the hash table
        * All added elements get removed
        */
       inline void Clear()
       {
           m_data.clear();
           m_data.reserve(m_size);
           for (uint32 i=0; i<m_size; i++)
           {
               m_data.push_back(std::vector< _Element >());
           }
       }

       /**
        * Get the value which is returned when an key is not found
        * @return The not found value
        */
       inline const MAP_TYPE& GetNotFoundValue() const { return m_not_found; }

       /**
        * Set the value which is returned when an key is not found
        * @param notFound The new not found value
        */
       inline void SetNotFoundValue(const MAP_TYPE& notFound) { m_not_found = notFound; }

       /**
        * @brief Add a new key to the hash table
        *        If the key already exits it is replaced
        * @param key_value The key value to add
        * @param map_value The mapped value for the key value
        */
       void AddKey(const KEY_TYPE &key_value, const MAP_TYPE& map_value)
       {
           uint32 p1;
           sint32 p2;
           GetElementPosition(key_value, p1, p2);

           _Element elem;
           elem.key_value = key_value;
           elem.map_value = map_value;
           if (p2 == -1)
           {
               m_data[p1].push_back(elem);
           }
           else
           {
               m_data[p1][p2] = elem;
           }
       }

       /**
        * Remove an existing key from the hash table
        * @param key_value The key value of the element to remove
        *
        * Bug fix:
        * use vector's erase() method
        */
       void RemoveKey(const KEY_TYPE &key_value)
       {
           uint32 p1;
           sint32 p2;
           GetElementPosition(key_value, p1, p2);


           if (p2 != -1)
           {
               m_data[p1].erase(m_data[p1].begin() + p2);
           }
       }

       /**
        * Get the mapped value of an element
        * It searches in the table for the key_value and returns the mapped value for this key
        * If the key  value is not found it returns the not found value
        *
        * @param key_value The key value for which to get the map value
        */
       const MAP_TYPE& GetMappedValue(const KEY_TYPE& key_value) const
       {
           uint32 p1;
           sint32 p2;
           GetElementPosition(key_value, p1, p2);

           if (p2 == -1)
           {
               return m_not_found;
           }
           else
           {
               return m_data[p1][p2].map_value;
           }
       }

       /** Get the value because of the key */
       const MAP_TYPE& operator[](const KEY_TYPE& key_value) const { return GetMappedValue(key_value);}

       /** Get all keys in the hash table */
       void GetAllKeys(std::vector< KEY_TYPE >& keys)
       {
           for (uint32 i=0; i<m_data.size(); i++)
           {
               for (uint32 j=0; j<m_data[i].size(); j++)
               {
                   keys.push_back(m_data[i][j].key_value);
               }
           }
       }

       /** Get all keys in the hash table */
       void GetAllKeysAndValues(std::vector< KEY_TYPE >& keys, std::vector< MAP_TYPE >& values)
       {
           for (uint32 i=0; i<m_data.size(); i++)
           {
               for (uint32 j=0; j<m_data[i].size(); j++)
               {
                   keys.push_back(m_data[i][j].key_value);
                   values.push_back(m_data[i][j].map_value);
               }
           }
       }

       /**
        * Return statistic information about the hash table.
        * @param  numEmptyLists  [OUT] Number of empty lists in the hash table.
        * @param  maxListLength  [OUT] Length of the longest list in the hash table.
        * @param  avgListLength  [OUT] Average length of the lists in the hash table.
        */
       void GetStatistics(uint32& numEmptyLists, uint32& maxListLength, double& avgListLength) const
       {
           numEmptyLists = 0;
           maxListLength = 0;
           avgListLength = 0;
           for (uint32 i = 0; i < m_data.size(); i++)
           {
               uint32 thisListLength = m_data[i].size();
               if (thisListLength == 0) numEmptyLists++;
               if (maxListLength < thisListLength) maxListLength = thisListLength;
               avgListLength += thisListLength;
           }
           avgListLength /= m_data.size();
       }

   private:
       /**
        * Get the element which your are searching for
        * This looks a little bit stupied to search for an element by another element
        * To get this working the compare operator needs to be defined in that why
        * tha you only compare the value which you are searching for
        */
       void GetElementPosition(const KEY_TYPE &key_value, uint32 &p1, sint32 &p2) const
       {
           KEY_VALUE_COMPARE_FUNC compare;
           p1 = GetTableEntryPosition(key_value);
           p2 = -1;
           for (uint32 i=0; i<m_data[p1].size(); i++)
           {
               if (compare(m_data[p1][i].key_value, key_value))
               {
                   p2 = i;
                   break;
               }
           }
       }

       /**
        * Methode calculates out of the identification of the element
        * the hash table position. This is done by a simple modulo with the size of
        * the hash table
        *
        * @param  key_value The element for which to get the hash table position
        * @return The hash table position
        */
       uint32 GetTableEntryPosition(const KEY_TYPE &key_value) const
       {
           HASH_FUNC func;
           return (func(key_value) % m_size);
       }

   private:
       /** The size of the hash table */
       uint32 m_size;
       /** holding the value when a element is not found **/
       MAP_TYPE m_not_found;
       /** 2-Dimensional array storing the elements */
       std::vector< std::vector< _Element > > m_data;
   };

   /**
    * @}
    */
} // namespace TC

#endif // TC_HASHTABLE_H
