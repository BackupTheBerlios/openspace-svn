// $Id$
/////////////////////////////////////////////////////////////////////////////
// This file is part of OpenSpace project.
//
// OpenSpace is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// OpenSpace is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenSpace; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/////////////////////////////////////////////////////////////////////////////
//
// C++ Implementation: OSMimeType
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSMimeType.h"
#include "OSUtils.h"

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <cctype>

using namespace std;

bool OSMimeType::isInitialized = false;
std::map<std::string, std::string> OSMimeType::mimeMap;
std::map<std::string, std::string> OSMimeType::mimeCache;

bool OSMimeType::_matches( std::string pattern, std::string ext )
{
    if ( pattern[ 0 ] == '*' )
    {
        pattern = pattern.substr( 1 );
        std::string::size_type pos = ext.rfind( pattern );
        if ( pos == std::string::npos )
        {
            return false;
        }
        
        return (ext.length() - pos == pattern.length());
    }

    if ( pattern[ pattern.length() - 1 ] == '*' )
    {
        pattern = pattern.substr( 0, pattern.length () - 1 );
        return ext.find( pattern ) == 0;
    }

    return (ext == pattern);
}

std::string OSMimeType::getMimeFromName( std::string name )
{
    std::transform( name.begin (), name.end (), name.begin (), ::tolower );

    if ( !isInitialized )
    {
        _initialize ();
    }

    if ( !mimeCache[ name ].empty() )
    {
        return mime2xml( mimeCache[ name ] );
    }

    std::string ext;

    if ( name.find( "." ) == std::string::npos )
    {
        ext = name;
    }
    else
    {
        ext = name.substr( name.rfind ( "." ) );

        if ( name.length () >= 6 && name.substr ( name.length () - 6, 6 ) == "tar.gz" )
        {
            return "application/x-compressed-tar";  //ext="tar.gz";
        }

        if ( name.length () >= 7 && name.substr ( name.length () - 7, 7 ) == "tar.bz2" )
        {
            return "application/x-bzip-compressed-tar"; //ext="tar.bz2";
        }
    }

    std::string rv = "application/octet-stream";

    for ( std::map<std::string, std::string>::iterator it = mimeMap.begin(); it != mimeMap.end(); ++it )
    {
        if ( _matches( it->first, ext ) )
        {
            rv = it->second;
            break;
        }
    }

    mimeCache[ name ] = rv;

    return mime2xml( rv );
}

void OSMimeType::_initialize( void )
{
    if ( isInitialized )
    {
        return ;
    }

#ifndef WIN32
    std::ifstream ifs( "/usr/share/mime/globs", ios::in );

    if ( !ifs )
    {
        cerr << "ifs() failed" << endl;
        return ;
    }

    mimeMap.clear();
    mimeCache.clear();

    while ( !ifs.eof () )
    {
        std::string tmp;
        getline ( ifs, tmp );

        if ( tmp.empty() || tmp[ 0 ] == '#' )
        {
            continue;
        }

        std::string mime = tmp.substr( 0, tmp.find ( ":" ) );
        std::string pattern = tmp.substr( tmp.find ( ":" ) + 1 );

        mimeMap[ pattern ] = mime;
    }

    ifs.close ();
#else
    _NOT_IMPLEMENTED_;
#endif

    // Some missing globs:
    mimeMap[ "*.asc" ] = "application/pgp-encrypted";
    mimeMap[ "*.com" ] = "application/x-ms-dos-executable";
    mimeMap[ "*.conf" ] = "text/x-config";
    mimeMap[ "*.dll" ] = "application/x-ms-dos-executable";
    mimeMap[ "*.ebuild" ] = "text/ebuild";
    mimeMap[ "*.eclass" ] = "text/eclass";
    mimeMap[ "*.gpg" ] = "application/pgp-keys";
    mimeMap[ "*.h++" ] = "text/x-c++hdr";
    mimeMap[ "*.hpp" ] = "text/x-c++hdr";
    mimeMap[ "*.ini" ] = "text/x-config";
    mimeMap[ "*.jsp" ] = "text/x-jsp";
    mimeMap[ "*.key" ] = "application/pgp-keys";
    mimeMap[ "*.l" ] = "text/x-lex";
    mimeMap[ "*.latex" ] = "text/x-tex";
    mimeMap[ "*.mak" ] = "text/x-makefile";
    mimeMap[ "*.ogm" ] = "video/vorbis";
    mimeMap[ "*.tbz2" ] = "application/x-bzip-compresses-tar";
    mimeMap[ "*.wma" ] = "application/x-ms-wma";
    mimeMap[ "*.y" ] = "text/x-yacc";
    mimeMap[ "*rc" ] = "text/x-config";

    isInitialized = true;
}
