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
// C++ Interface: OSFileTypeInfo
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSFILETYPEINFO_H__
#define __OSFILETYPEINFO_H__

#include <vector>
#include <string>

/**
 * File type definition class.
 * 
 * @todo Get rid of public data members.
 */
class OSFileTypeInfo
{
public:
    
    /**
     * Default constructor.
     */
    OSFileTypeInfo(void);
    
    /**
     * Constructor.
     * @param name
     * @param command
     */
    OSFileTypeInfo( std::string name, std::string command );
    
    /**
     * @todo Add comments
     */
    void save( void );
    
    /**
     * 
     * @param name
     * 
     * @todo Add comments.
     */
    bool load( std::string name );

public:
    
    std::vector<std::string> commands;
    
    std::string name;
    std::string command;
    std::string icon;
    std::string color;
    std::string backcolor;
};

#endif // #ifndef __OSFILETYPEINFO_H__
