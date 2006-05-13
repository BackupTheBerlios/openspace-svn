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
// C++ Implementation: OSFirstRunDialogBox
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSFirstRunDialogBox.h"
#include "OSPreferences.h"
#include "OSMimeApp.h"
#include <fx.h>

FXDEFMAP( OSFirstRunDialogBox ) FirstRunMap[] =
{
    //________Message_Type_____________________ID____________Message_Handler_______
    FXMAPFUNCS ( SEL_COMMAND, OSFirstRunDialogBox::ID_CONF_MANUAL, OSFirstRunDialogBox::ID_CONF_AUTOMATIC, OSFirstRunDialogBox::onConfigure ),
};

FXIMPLEMENT ( OSFirstRunDialogBox, FXDialogBox, FirstRunMap, ARRAYNUMBER ( FirstRunMap ) )
         
OSFirstRunDialogBox::OSFirstRunDialogBox( void )
{
    // construction
}

OSFirstRunDialogBox::OSFirstRunDialogBox( FXWindow* owner ) : FXDialogBox( owner, "OSFirstRunDialogBox", DECOR_ALL, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4 )
{
    FXVerticalFrame* vertical = new FXVerticalFrame ( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y );

    FXGroupBox *vv = new FXGroupBox ( vertical, "", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0 );
    new FXLabel( vv, "Best for newbie and first run" );
    FXButton *but3 = new FXButton ( vv, "full automatic configuration", NULL, this, ID_CONF_AUTOMATIC );

    vv = new FXGroupBox ( vertical, "", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0 );
    new FXLabel( vv, "Choose which program will open given file type" );
    FXButton *but2 = new FXButton ( vv, "semiautomatic configuration", NULL, this, ID_CONF_SEMI );

    vv = new FXGroupBox ( vertical, "", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0 );
    new FXLabel( vv, "Not recomended - try better semi-auto configuration\n and then manual rather than only manual" );
    FXButton *but1 = new FXButton ( vv, "manual configuration", NULL, this, ID_CONF_MANUAL );
}

OSFirstRunDialogBox::~OSFirstRunDialogBox( void )
{
    // destruction
}

long OSFirstRunDialogBox::onConfigure( FXObject * sender, FXSelector sel, void * )
{
    FXushort id = FXSELID( sel );
    
    if ( id == ID_CONF_MANUAL )
    {
        OSPreferences * prefer = new OSPreferences( this );
        prefer->create();
        prefer->execute( PLACEMENT_OWNER );
    }
    else if ( id == ID_CONF_SEMI )
    {
        OSMimeApp * mimeapp = new OSMimeApp( this );
        mimeapp->create();
        mimeapp->execute( PLACEMENT_OWNER );
    }
    else if ( id == ID_CONF_AUTOMATIC )
    {
        OSMimeApp * mimeapp = new OSMimeApp( this );
        mimeapp->doAutomaticConfiguration();

    }
    
    FXMessageBox about( this, "restart", "restart nao to apply changes",
                        NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER );
    about.execute();
}
