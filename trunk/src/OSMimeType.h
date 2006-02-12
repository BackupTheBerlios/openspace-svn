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
// C++ Interface: OSMimeType
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSMIMETYPE_H__
#define __OSMIMETYPE_H__

#include <string>
#include <map>

/**
 * MIME type class.
 * 
 * @todo Why constructor/destructor are private?
 * @todo Is #__initialize() "Named constructor"?
 * @todo Do you really want to prevent anyone of destroying object of this class?
 */
class OSMimeType
{
public:
    
    static std::string getMimeFromName( std::string name );
    static std::map<std::string, std::string> mimeMap;
    static void _initialize( void );

private:
    
    /**
     * Default constructor
     */
    OSMimeType( void );
    
    /**
     * Destructor.
     */
    ~OSMimeType( void );

    /**
     * @todo What it exactly does?
     */
    static bool _matches( std::string pattern, std::string fname );
    
private:
    
    static std::map<std::string, std::string> mimeCache;
    
    /**
     * Flag indicates if object is initialized.
     */
    static bool isInitialized;

};

#endif // #ifndef __OSMIMETYPE_H__
