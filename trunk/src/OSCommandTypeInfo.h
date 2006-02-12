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
// C++ Interface: OSCommandTypeInfo
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
 * Command type definition class.
 * 
 * @todo Public data members!
 */
class OSCommandTypeInfo
{
public:
    
    /**
     * Default constructor.
     */
    OSCommandTypeInfo(void);
    
    /**
     * Constructor.
     * @param name
     * @param exec
     * 
     * @todo Comments
     */
    OSCommandTypeInfo(std::string name, std::string exec);
    
    /**
     * @todo Comments
     */
    void save( void );

public:
    
    bool rescan;
    bool capture;
    
    std::string name;
    std::string exec;
    std::string text;
    std::string type;
    std::string icon;
    std::string key;
    std::string key_mask;    
};

#endif
