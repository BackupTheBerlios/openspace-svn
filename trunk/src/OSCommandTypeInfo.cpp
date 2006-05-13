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
// C++ Implementation: OSCommandTypeInfo
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
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
        if ( !conf->saveonestring ( "/NaoConfig/commands/" + com + "/exec", value ) )
        {
            conf->addstring ( "/NaoConfig/commands", com, "" );

            if ( !exec.empty() )
            {
                conf->addstring ( "/NaoConfig/commands/" + com, "exec", value );
            }
        }
    }
    else
    {
        if ( !conf->saveonestring ( "/NaoConfig/commands/" + com + "/type", type ) )
        {
            conf->addstring ( "/NaoConfig/commands", com, "" );
            conf->addstring ( "/NaoConfig/commands/" + com, "type", type );
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
        if ( !conf->saveonestring ( "/NaoConfig/commands/" + com + "/options", options ) )
            conf->addstring ( "/NaoConfig/commands/" + com, "options", options );
    }
    else
    {
        conf->removestring ( "/NaoConfig/commands/" + com + "/options" );
    }

    if ( !text.empty() )
    {
        if ( !conf->saveonestring ( "/NaoConfig/commands/" + com + "/text", text ) )
            conf->addstring ( "/NaoConfig/commands/" + com, "text", text );
    }
    else
    {
        conf->removestring ( "/NaoConfig/commands/" + com + "/text" );
    }

    if ( !icon.empty() )
    {
        if ( !conf->saveonestring ( "/NaoConfig/commands/" + com + "/icon", icon ) )
            conf->addstring ( "/NaoConfig/commands/" + com, "icon", icon );
    }
    else
    {
        conf->removestring ( "/NaoConfig/commands/" + com + "/icon" );
    }

    if ( !key.empty() )
    {
        if ( !conf->saveonestring ( "/NaoConfig/commands/" + com + "/key", key ) )
            conf->addstring ( "/NaoConfig/commands/" + com, "key", key );
    }
    else
    {
        conf->removestring ( "/NaoConfig/commands/" + com + "/key" );
    }

    if ( !key_mask.empty() )
    {
        if ( !conf->saveonestring ( "/NaoConfig/commands/" + com + "/key_mask", key ) )
            conf->addstring ( "/NaoConfig/commands/" + com, "key_mask", key );
    }
    else
    {
        conf->removestring ( "/NaoConfig/commands/" + com + "/key_mask" );
    }

} // save
