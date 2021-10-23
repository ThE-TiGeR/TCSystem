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
//  $Id: TCWFileName.cpp,v 19a9fe061077 2014/07/01 18:05:17 thomas $
//*******************************************************************************

#include "TCWFileName.h"

#include "TCFileNameImp.h"
#include "TCWString.h"
#include "TCWStringTraits.h"

#include <cctype>

#include "TCNewEnable.h"

namespace tc
{
    class WFileNameTraits
    {
    public:
        static const wchar_t* EXTENSION_SEPERATOR;
        static const wchar_t* PATH_SEPERATOR;
        static const wchar_t* PATH_SEPERATOR_WRONG;

        static void ReplaceString(std::wstring& data, const std::wstring& token, const std::wstring& replace)
        {
            data = wstring::Replace(data, token, replace);
        }
    };

    const wchar_t* WFileNameTraits::EXTENSION_SEPERATOR = L".";
    const wchar_t* WFileNameTraits::PATH_SEPERATOR = L"/";
    const wchar_t* WFileNameTraits::PATH_SEPERATOR_WRONG = L"\\";

    typedef FileNameImpTmpl<std::wstring, WFileNameTraits, WStringTraits> WFileNameImp;

    std::wstring wfile_name::GetExtension(const std::wstring& file_name)
    {
        return WFileNameImp::GetExtension(file_name);
    }

    std::wstring wfile_name::GetExtensionIfExtension(const std::wstring& file_name, const std::wstring& additional_ext)
    {
       return WFileNameImp::GetExtensionIfExtension(file_name, additional_ext);
    }

    std::wstring wfile_name::GetName(const std::wstring& file_name)
    {
        return WFileNameImp::GetName(file_name);
    }

    std::wstring wfile_name::GetPath(const std::wstring& file_name)
    {
        return WFileNameImp::GetPath(file_name);
    }

    std::wstring wfile_name::RemoveExtension(const std::wstring& file_name)
    {
        return WFileNameImp::RemoveExtension(file_name);
    }

    std::wstring wfile_name::AddPaths(const std::wstring& path1,
        const std::wstring& path2)
    {
        return WFileNameImp::AddPaths(path1, path2);
    }

    std::wstring wfile_name::AddFileNameAndPath(const std::wstring& file_name,
        const std::wstring& path)
    {
        return WFileNameImp::AddFileNameAndPath(file_name, path);
    }

    std::wstring wfile_name::AddFileNameAndExtension(const std::wstring& file_name,
        const std::wstring& ext)
    {
        return WFileNameImp::AddFileNameAndExtension(file_name, ext);
    }

    std::wstring wfile_name::Simplify(const std::wstring& file)
    {
        return WFileNameImp::Simplify(file);
    }

    std::vector<std::wstring> wfile_name::GetDirectoriesOfFileName(const std::wstring& file_name)
    {
        return wstring::Split(GetPath(file_name).c_str(), WFileNameTraits::PATH_SEPERATOR);
    }

    std::vector<std::wstring> wfile_name::GetDirectoriesOfPath(const std::wstring& path)
    {
        return wstring::Split(Simplify(path).c_str(), WFileNameTraits::PATH_SEPERATOR);
    }

    const wchar_t* wfile_name::GetPathSeparator()
    {
        return WFileNameTraits::PATH_SEPERATOR;
    }

}
