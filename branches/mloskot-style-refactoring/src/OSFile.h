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
// C++ Interface: OSFile
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSFILE_H__
#define __OSFILE_H__

#include <string>
#include <vector>

/**
 * @todo Move this global to the class as a public member/
 */
enum
{
    FOLDER      = 64,   /**< Directory item */
    EXECUTABLE  = 128,  /**< Executable item */
    SYMLINK     = 256,  /**< Symbolic linked item */
    CHARDEV     = 512,  /**< Character special item */
    BLOCKDEV    = 1024, /**< Block special item */
    FIFO        = 2048, /**< FIFO item */
    SOCK        = 4096  /**< Socket item */
};

/**
 * File class.
 * OpenSpace internal file description type.
 * 
 * @todo Encapsulate public data members and add accessors/mutators.
 * @todo It may be useful to add copy constructor and operator=.
 */
class OSFile
{
public:
    
    /**
     * Default constructor.
     */
    OSFile(void) {}
    
    /**
     * Destructor.
     */
    ~OSFile(void) {}
    
    unsigned int type;
    unsigned long size;
        
    std::string name;
    std::string content;
    std::string mode;
    std::string user;
    std::string group;
    std::string date;
    
    std::vector <std::string> attrib;
};

#endif // #ifndef __OSFILE_H__

