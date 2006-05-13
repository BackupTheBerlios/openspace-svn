// $Id$
/////////////////////////////////////////////////////////////////////////////
// This file is part of nao file manager project.
//
// nao file manager is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// nao file manager is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with nao file manager; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/////////////////////////////////////////////////////////////////////////////
//
// C++ Interface: OSVirtualFileSystemHeader
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSVIRTUALFILESYSTEMHEADER_H__
#define __OSVIRTUALFILESYSTEMHEADER_H__

#include <string>

/**
 * File system header container.
 * 
 * @todo Encapsulate public data members and add accessors/mutators.
 * @todo What about _ or m_ prefix for data members names?
 */
class OSVirtualFileSystemHeader
{
public:

    /**
     * Default constructor.
     */
    OSVirtualFileSystemHeader( void ) {}

    /**
     * Constructor.
     * @param n
     * @param t
     * @param w
     */
    OSVirtualFileSystemHeader( std::string n, std::string t = "", std::string w = "100" ) 
    : name( n ), type( t ), width( w ) {}

public:

    std::string name;
    std::string type;
    std::string width;

}; // OSVFSHeaderContainer

#endif // #ifndef __OSVIRTUALFILESYSTEMHEADER_H__

