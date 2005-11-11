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
// C++ Implementation: $MODULE$
//
// Author: $AUTHOR$ <$EMAIL$>, (C) $YEAR$
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSMimeApp.h"
#include "OSCommandTypeInfo.h"
#include "OSFileTypeInfo.h"
#include "OSObjectManager.h"
#include "OSUtils.h"

#include <fx.h>
#include <string>
#include <fstream>

#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif

#ifndef PATH_CFG
#define PATH_CFG        "/usr/local/etc/"
#endif


FXDEFMAP ( OSMimeApp ) OSMimeAppMap[] =
    {
        //________Message_Type_____________________ID____________Message_Handler_______
        FXMAPFUNCS ( SEL_COMMAND, OSMimeApp::ID_NEXT, OSMimeApp::ID_PREVIOUS, OSMimeApp::onNextPrevious ),
        FXMAPFUNC ( SEL_CLOSE, 0, OSMimeApp::onClose ),
    };

FXIMPLEMENT ( OSMimeApp, FXDialogBox, OSMimeAppMap, ARRAYNUMBER ( OSMimeAppMap ) )

OSMimeApp::OSMimeApp( void )
{
    // construction
}

OSMimeApp::OSMimeApp( FXWindow* owner ) :
        FXDialogBox ( owner, "OSMimeApp", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4 )
{
    FXVerticalFrame * vertical = new FXVerticalFrame ( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y );

    mime_label = new FXLabel( vertical, "" );
    programsbox = new FXComboBox ( vertical, 20, NULL, 0, FRAME_THICK | LAYOUT_SIDE_TOP | COMBOBOX_STATIC );
    programsbox->setNumVisible( 10 );

    FXHorizontalFrame *hori = new FXHorizontalFrame ( vertical, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y );
    FXButton *but1 = new FXButton ( hori, "PREVIOUS", NULL, this, ID_PREVIOUS );
    FXButton *but3 = new FXButton ( hori, "NEXT", NULL, this, ID_NEXT );

    std::string file = std::string( PATH_CFG ) + std::string( SEPARATOR ) + std::string( "mimeapp" );
    std::ifstream ifs ( file.c_str(), ios::in );

    while ( !ifs.eof () )
    {
        std::string tmp;
        getline ( ifs, tmp );

        if ( tmp.empty() || tmp[ 0 ] == '#' )
        {
            continue;
        }
        mime_vector.push_back( mime2xml( tmp ) );
    }
    ifs.close();
    iter = mime_vector.begin ();
    fill( *iter );

}

OSMimeApp::~OSMimeApp( void )
{
    // destruction
}

long OSMimeApp::onClose( FXObject * sender, FXSelector sel, void *ptr )
{
    std::string program = programsbox->getText().text();
    std::string mime = mime_label->getText().text();
    save( mime, program );

    FXDialogBox::onCmdAccept( sender, sel, ptr );

}

void OSMimeApp::doAutomaticConfiguration( void )
{
    vector <std::string>::iterator iter;

    for ( iter = mime_vector.begin(); iter != mime_vector.end(); iter++ )
    {
        fill( *iter );
        save( mime_label->getText().text(), programsbox->getText().text() );
    }

}

long OSMimeApp::onNextPrevious( FXObject * sender, FXSelector sel, void * )
{
    FXushort id = FXSELID( sel );

    if ( id == ID_NEXT && iter + 1 != mime_vector.end() )
    {
        iter++;
    }
    else if ( id == ID_PREVIOUS && iter != mime_vector.begin() )
    {
        iter--;
    }

    std::string program = programsbox->getText().text();
    std::string mime = mime_label->getText().text();
    save( mime, program );


    fill( *iter );

}

void OSMimeApp::fill( std::string tmp )
{
    std::string mime, program;
    mime = tmp.substr ( 0, tmp.rfind ( ":" ) );
    program = tmp.substr ( tmp.rfind ( ":" ) + 1 );

    mime_label->setText( mime.c_str() );

    programsbox->clearItems();

    std::stringstream parser ( program );
    std::string prog;
    while ( parser >> prog )
    {

        std::string command = "which " + prog + " >/dev/null 2>/dev/null";
        if ( system( command.c_str() ) == 0 )
        {
            programsbox->appendItem( prog.c_str() );
        }
    }
    
    programsbox->appendItem( "" );
}

void OSMimeApp::save( std::string mime, std::string program )
{
    if ( program.empty() )
    {
        return ;
    }

    std::string::size_type pos = mime.find( "/" );

    std::string mime_major;
    std::string mime_minor;
    std::string command_name = "open_";
    std::string command_exec = program + " {f}" ;

    std::string reg2;

    if ( pos != std::string::npos )
    {
        mime_major = mime.substr( 0, pos );
        mime_minor = mime.substr( pos + 1 );

        command_name += mime_major + "_" + mime_minor;

        reg2 = conf->readonestring ( "/OpenspaceConfig/file_types/" + mime_major + "/default" );
        std::string command_major = conf->readonestring ( "/OpenspaceConfig/commands/" + reg2 + "/exec" );

        if ( reg2.empty() || ( command_major != command_exec && reg2 != command_name ) )
        {
            OSFileTypeInfo ct = OSFileTypeInfo();
            ct.load( mime );
            ct.command = command_name;
            ct.save();

        }
        else
        {
            return ;
        }
    }
    else // major mime type: audio,video,etc
    {
        command_name += mime;
        std::string reg = conf->readonestring ( "/OpenspaceConfig/file_types/" + mime + "/default" );
        std::string cmd = conf->readonestring ( "/OpenspaceConfig/commands/" + reg + "/exec" );

        if ( reg.empty() || cmd != command_exec )
        {

            OSFileTypeInfo ct = OSFileTypeInfo();
            ct.load( mime );
            ct.command = command_name;
            ct.save();
        }
        else
        {
            return;
        }
    }

    OSFileTypeInfo cm = OSFileTypeInfo( command_name, command_exec );
    cm.save();
}
