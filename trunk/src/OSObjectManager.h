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
// C++ Interface: OSObjectManager
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSOBJECTMANAGER_H__
#define __OSOBJECTMANAGER_H__

#include "OSConfigure.h"
#include "OSFileTypeSymbol.h"
#include "OSThreadExec.h"

#include <fx.h>
#include <map>

using namespace std;

extern OSConfigure *conf;

FXColor readcolor ( string col );
FXColor readcolor2 ( string col );

/**
 * \e (missing documentation)
 * 
 * @todo This class seems to be of type of Singleton. If it is needs some fixes.
 * @todo Documentation
 */
class OSObjectManager
{
public:
    
    static OSObjectManager* instance( FXApp *app );
    
    FXFont *captionfont1, *captionfont2, *captionfont3;
    FXApp *app;
    
    std::vector<OSThreadExec*> thread_vec;
    
    std::map<std::string, OSFileTypeSymbol*> file_type_settings;
    
    std::map<std::string, FXIcon*> osicons;
    
    FXIcon *specialicons[ 30 ];
    
    std::map<std::string, unsigned int > key_map;
    
    FXColor maincolor;

protected:
    
    /**
     * Constructor.
     * @param FXApp object pointer
     */
    OSObjectManager( FXApp *app );
     
    /**
     * Copy constructor.
     */
    OSObjectManager( const OSObjectManager& );
    
    /**
     * Assignment operator.
     * @param objectmanager - FXApp instance being copied.
     */
    OSObjectManager& operator= ( const OSObjectManager& );
    
private:
    
    static OSObjectManager* pinstance;
};

#endif // #ifndef __OSOBJECTMANAGER_H__
