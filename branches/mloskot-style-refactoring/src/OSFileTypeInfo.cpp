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
// C++ Implementation: OSFileTypeInfo
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSFileTypeInfo.h"
#include <vector>
#include <string>

OSFileTypeInfo::OSFileTypeInfo( void )
{
    // construction
}

OSFileTypeInfo::OSFileTypeInfo( std::string name, std::string command ) : name( name ), command( command )
{
    // destruction
}

bool OSFileTypeInfo::load( std::string name )
{
    this->name = name;

    std::string::size_type pos = name.find( "/" );
    std::string path;
    std::string mime_major;
    std::string mime_minor;
    if ( pos != std::string::npos )
    {
        mime_major = name.substr( 0, pos );
        mime_minor = name.substr( pos + 1 );
        path = "/OpenspaceConfig/file_types/" + mime_major + "/types/" + mime_minor;
    }
    else
    {
        path = "/OpenspaceConfig/file_types/" + name;
    }

    command = conf->readonestring ( path + "/default" );
    icon = conf->readonestring( path + "/icon" ) ;
    color = conf->readonestring( path + "/color" ) ;
    backcolor = conf->readonestring( path + "/backcolor" ) ;

    configure conflocal2 = *conf;
    if ( conflocal2.openxpath( path + "/commands/command" ) != -1 )
    {
        std::string commandstr;
        while ( conflocal2.getnextstring( commandstr ) )
        {
            commands.push_back( commandstr.c_str() );
        }
    }

} // load

void OSFileTypeInfo::save( void )
{
    //fxmessage("\nNAME=%s COMMAND=%s\n",name.c_str(),command.c_str());

    if ( name.empty() )
        return ;

    std::string path;
    std::string mime_major;
    std::string mime_minor;

    std::string::size_type pos = name.find( "/" );

    if ( pos != std::string::npos )
    {
        mime_major = name.substr( 0, pos );
        mime_minor = name.substr( pos + 1 );
        std::string ret;
        if ( conf->readonestring ( "/OpenspaceConfig/file_types/" + mime_major, ret ) == false )
        {
            conf->addstring ( "/OpenspaceConfig/file_types", mime_major, "" );
        }

        if ( conf->readonestring ( "/OpenspaceConfig/file_types/" + mime_major + "/types", ret ) == false )
        {
            conf->addstring ( "/OpenspaceConfig/file_types/" + mime_major , "types", "" );
        }

        if ( conf->readonestring ( "/OpenspaceConfig/file_types/" + mime_major + "/types/" + mime_minor, ret ) == false )
        {
            conf->addstring ( "/OpenspaceConfig/file_types/" + mime_major + "/types" , mime_minor, "" );
        }

        path = "/OpenspaceConfig/file_types/" + mime_major + "/types/" + mime_minor;

    }
    else  // major mime type: audio,video,etc
    {
        std::string ret;
        if ( conf->readonestring ( "/OpenspaceConfig/file_types/" + name, ret ) == false )
        {
            conf->addstring ( "/OpenspaceConfig/file_types/" , name, "" );
        }

        path = "/OpenspaceConfig/file_types/" + name;
    }

    if ( !command.empty() )
    {
        if ( !conf->saveonestring ( path + "/default", command ) )
        {
            conf->addstring( path , "default", command );
        }
    }
    else
    {
        conf->removestring ( path + "/default" );
    }


    if ( !commands.empty() )
    {
        conf->removestring( path + "/commands" );
        std::vector <std::string>::iterator iter;
        conf->addstring( path, "commands", "" );
        for ( iter = commands.begin();iter != commands.end();iter++ )
        {
            conf->addstring( path + "/commands", "command", iter->c_str() );
        }
    }

    if ( !conf->saveonestring ( path + "/color", color ) )
    {
        conf->addstring( path , "color", color );
    }
    if ( !conf->saveonestring ( path + "/backcolor", backcolor ) )
    {
        conf->addstring( path , "backcolor", backcolor );
    }
    if ( !icon.empty() )
    {
        if ( !conf->saveonestring ( path + "/icon", icon ) )
        {
            conf->addstring( path , "icon", icon );
        }
    }
    else
    {
        conf->removestring ( path + "/icon" );
    }

} // save