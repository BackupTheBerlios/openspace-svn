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
// C++ Implementation: OSUtils
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSUtils.h"
#include <string>

//number to std::string in human readable units
std::string numtostring( unsigned long k )
{
    std::string metric ( "B" );
    if ( k > 1024 )
    {
        k /= 1024;
        metric = "KB";
        if ( k > 1024 )
        {
            k /= 1024;
            metric = "MB";
        }
    }

    char chstr[ 20 ] = { 0 };
    sprintf ( chstr, "%d", k );

    std::string retstrin = std::string ( chstr ) + " " + metric;

    return retstrin;
}

//number to std::string
std::string ntos( unsigned long k )
{
    char chstr[ 20 ] = { 0 };
    sprintf ( chstr, "%d", k );
    
    return chstr;
}

std::string mime2xml( std::string str )
{
    for ( int i = 0; str[ i ] != '\0'; ++i )
    {
        if ( str[ i ] == '+' )
            str[ i ] = 'P';
    }

    return str;
}

std::string xml2mime( std::string str )
{
    for ( int i = 0; str[ i ] != '\0'; ++i )
    {
        if ( str[ i ] == 'P' )
            str[ i ] = '+';
    }

    return str;
}
