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
// C++ Implementation: OSCommandTypeInfo
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSCommandTypeInfo.h"
#include "OSObjectManager.h"
#include <vector>
#include <string>

OSCommandTypeInfo::OSCommandTypeInfo( void )
{
    // construction
}

OSCommandTypeInfo::OSCommandTypeInfo( std::string name, std::string exec ) : name( name ), exec( exec )
{
    capture = false;
    rescan = false;
}

void OSCommandTypeInfo::save( void )
{
    std::string com(name);
    std::string value(exec);

    if ( type != "INTERNAL" && type != "PLUGIN" )
    {
        if ( !conf->saveonestring ( "/OpenspaceConfig/commands/" + com + "/exec", value ) )
        {
            conf->addstring ( "/OpenspaceConfig/commands", com, "" );

            if ( !exec.empty() )
            {
                conf->addstring ( "/OpenspaceConfig/commands/" + com, "exec", value );
            }
        }
    }
    else
    {
        if ( !conf->saveonestring ( "/OpenspaceConfig/commands/" + com + "/type", type ) )
        {
            conf->addstring ( "/OpenspaceConfig/commands", com, "" );
            conf->addstring ( "/OpenspaceConfig/commands/" + com, "type", type );
        }
    }

    std::string options;
    if ( capture )
    {
        options += " capture";
    }
    
    if ( rescan )
    {
        options += " rescan";
    }

    if ( !options.empty() )
    {
        if ( !conf->saveonestring ( "/OpenspaceConfig/commands/" + com + "/options", options ) )
            conf->addstring ( "/OpenspaceConfig/commands/" + com, "options", options );
    }
    else
    {
        conf->removestring ( "/OpenspaceConfig/commands/" + com + "/options" );
    }

    if ( !text.empty() )
    {
        if ( !conf->saveonestring ( "/OpenspaceConfig/commands/" + com + "/text", text ) )
            conf->addstring ( "/OpenspaceConfig/commands/" + com, "text", text );
    }
    else
    {
        conf->removestring ( "/OpenspaceConfig/commands/" + com + "/text" );
    }

    if ( !icon.empty() )
    {
        if ( !conf->saveonestring ( "/OpenspaceConfig/commands/" + com + "/icon", icon ) )
            conf->addstring ( "/OpenspaceConfig/commands/" + com, "icon", icon );
    }
    else
    {
        conf->removestring ( "/OpenspaceConfig/commands/" + com + "/icon" );
    }

    if ( !key.empty() )
    {
        if ( !conf->saveonestring ( "/OpenspaceConfig/commands/" + com + "/key", key ) )
            conf->addstring ( "/OpenspaceConfig/commands/" + com, "key", key );
    }
    else
    {
        conf->removestring ( "/OpenspaceConfig/commands/" + com + "/key" );
    }

    if ( !key_mask.empty() )
    {
        if ( !conf->saveonestring ( "/OpenspaceConfig/commands/" + com + "/key_mask", key ) )
            conf->addstring ( "/OpenspaceConfig/commands/" + com, "key_mask", key );
    }
    else
    {
        conf->removestring ( "/OpenspaceConfig/commands/" + com + "/key_mask" );
    }

} // save
