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
// C++ Implementation: OSInfoPanel
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSInfoPanel.h"

#include <fx.h>
#include <string>


OSInfoPanel::OSInfoPanel( FXComposite * p, std::string command )
{
    frame = new FXVerticalFrame( p, LAYOUT_FILL_X | FRAME_THICK, 0, 0, 0, 0, 0, 0 );

    lab0 = new FXLabel( frame, "NONE" );

    if ( command == "copy" || command == "move" )
    {
        lab1 = new FXLabel( frame, "NONE" );
        lab2 = new FXLabel( frame, "NONE" );
        pbar = new FXProgressBar( frame, NULL, 0, LAYOUT_FILL_X | FRAME_THICK | PROGRESSBAR_PERCENTAGE );
        pbar2 = new FXProgressBar( frame, NULL, 0, LAYOUT_FILL_X | FRAME_THICK | PROGRESSBAR_PERCENTAGE );
        bu = new FXButton( frame, "CANCEL", NULL, NULL, 999, BUTTON_TOOLBAR );
        bu->setBackColor( FXRGB ( 145, 134, 201 ) );
        pbar->setBarColor( FXRGB ( 145, 134, 201 ) );
        pbar2->setBarColor( FXRGB ( 145, 134, 201 ) );
    }
    else if ( command == "remove" )
    {
        lab1 = new FXLabel( frame, "NONE" );
        bu = new FXButton( frame, "CANCEL", NULL, NULL, 999, BUTTON_TOOLBAR );
        bu->setBackColor( FXRGB ( 145, 134, 201 ) );
    }
    
    frame->create();
    frame->recalc();
}

OSInfoPanel::~OSInfoPanel()
{
    delete frame;
}
