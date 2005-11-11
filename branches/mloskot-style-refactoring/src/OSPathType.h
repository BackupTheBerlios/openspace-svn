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
// C++ Interface: OSPathType
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSPATHTYPE_H__
#define __OSPATHTYPE_H__

#include <string>

/**
 * 
 * Contains information for remote filelist
 */
class OSPathType
{
public:
    
    OSPathType(std::string dir, std::string type = "local", std::string server, std::string user, std::string password, std::string port);

    std::string dir;
    std::string type;
    std::string server;
    std::string user;
    std::string password;
    std::string port;
};

#endif // #ifndef __OSPATHTYPE_H__

