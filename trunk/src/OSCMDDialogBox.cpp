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
// C++ Implementation: OSCMDDialogBoxBox
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSCMDDialogBox.h"
#include <fx.h>
#include <string>


FXIMPLEMENT( OSCMDDialogBox, FXDialogBox, NULL, 0 )

OSCMDDialogBox::OSCMDDialogBox( FXWindow* owner, OSVirtualFileSystemBase* fb, std::vector<std::string> src ) :
        FXDialogBox( owner, "command", DECOR_TITLE | DECOR_BORDER )
{

    this->src = src;
    this->fb = fb;
    FXObject *tgt = ( FXObject* ) owner;
    contents = new FXVerticalFrame( this, LAYOUT_SIDE_TOP | FRAME_NONE | LAYOUT_FILL_X | PACK_UNIFORM_WIDTH );

    buttons = new FXHorizontalFrame( this, LAYOUT_SIDE_TOP | FRAME_NONE | LAYOUT_FILL_X | LAYOUT_FILL_Y | PACK_UNIFORM_WIDTH );
    ok = new FXButton( buttons, "OK", NULL, tgt, ID_COMMAND, BUTTON_INITIAL | FRAME_RAISED, 0, 0, 0, 0, 0, 0, 0, 0 );
    cancel = new FXButton( buttons, "CANCEL", NULL, tgt, ID_CANCEL_COMMAND, FRAME_RAISED, 0, 0, 0, 0, 0, 0, 0, 0 );
}

OSCMDDialogBox::~OSCMDDialogBox(void)
{
    delete contents;
    delete buttons;
}
