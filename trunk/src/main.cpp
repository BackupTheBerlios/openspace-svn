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
// C++ Implementation: $MODULE$
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
// Application entry point.
// Nothing special here, create and display main window and
// required directories if they don't exist.
/////////////////////////////////////////////////////////////////////////////

#include "OSMainWindow.h"
#include "config.h"
#include <fx.h>
#include <unistd.h>

/**
 * Application entry point.
 */
int main ( int argc, char **argv )
{
    for ( int i = 0;i < argc;i++ )
    {
        if ( !strcmp( argv[ i ], "-v" ) || !strcmp( argv[ i ], "--version" ) )
        {
            fxmessage( "\n%s\n", PACKAGE_STRING );
            return 0;
        }

        if ( !strcmp( argv[ i ], "-h" ) || !strcmp( argv[ i ], "--help" ) )
        {
            fxmessage( "\n-h, --help print this help\n" );
            fxmessage( "-v, --version print version\n" );
            return 0;
        }
    }

    string tmpdir = "/tmp/nao";
    if ( !FXFile::exists( tmpdir.c_str() ) )
    {
        FXFile::createDirectory( tmpdir.c_str(), 493 );
    }

    string dir = FXFile::getUserDirectory ( "" ).text ();
    dir = dir + "/.nao";
    string file = dir + "/nao.conf";

    if ( !FXFile::exists( file.c_str() ) )
    {
        FXFile::createDirectory( dir.c_str(), 493 );
        string dir2 = dir + "/icons";
        FXFile::createDirectory( dir2.c_str(), 493 );
        dir += "/plugins";
        FXFile::createDirectory( dir.c_str(), 493 );
        dir2 = dir + "/cmddialog";
        FXFile::createDirectory( dir2.c_str(), 493 );
        dir2 = dir + "/filelist";
        FXFile::createDirectory( dir2.c_str(), 493 );


        string src = PATH_CFG + string( "/nao.conf" );
        FXFile::copy( src.c_str(), file.c_str() );
    }
    fxTraceLevel = 0;
    FXApp *application = new FXApp ( "nao", NULL );
    application->init ( argc, argv );

    new OSMainWindow ( application );
    application->create ();
    return application->run ();
}
