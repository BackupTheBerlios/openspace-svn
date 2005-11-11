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
// C++ Implementation: OSShutter
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSShutter.h"
#include <fx.h>

FXDEFMAP ( OSShutter ) shutterMap[] =
{
    FXMAPFUNC( SEL_MOUSEWHEEL, 0, OSShutter::onMouseWheel ),
    FXMAPFUNC( SEL_COMMAND, OSShutter::ID_OPEN_SHUTTERITEM, OSShutter::onOpenItem )
};

FXIMPLEMENT( OSShutter, FXShutter, shutterMap, ARRAYNUMBER ( shutterMap ) )
        
OSShutter::OSShutter( void )
{
    // Construction
}

OSShutter::OSShutter( FXComposite * p, FXWindow * x ) : FXShutter( p, x, 6667, FRAME_NONE | LAYOUT_FILL_Y | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | LAYOUT_FIX_WIDTH, 0, 0, 90, 0, 0, 0, 0, 0, 0, 0 )
{
    setLayoutHints( FRAME_NONE | LAYOUT_FILL_Y | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT );
}

long OSShutter::onMouseWheel( FXObject * sender, FXSelector, void *ptr )
{
    FXEvent * event = ( FXEvent * ) ptr;
    
    int x;
    if ( event->code < 0 )
    {
        x = getCurrent () + 1;
        if ( x > numChildren () - 1 )
            x = 0;
        setCurrent ( x );
    }

    if ( event->code > 0 )
    {
        x = getCurrent () - 1;
        if ( x == -1 )
            x = numChildren () - 1;
        setCurrent ( x );
    }

    FXMenuPane *menu = ( FXMenuPane * ) this->getParent ();
    menu->resize ( 90, getheight() );

    return 1;
}

long OSShutter::onOpenItem( FXObject * sender, FXSelector sel, void *ptr )
{
    FXShutter::onOpenItem ( sender, sel, ptr );

    FXMenuPane *menu = ( FXMenuPane * ) ( ( FXWindow * ) sender ) ->getParent () ->getParent ();
    menu->resize ( 90, getheight () );

    return 1;
}

int OSShutter::getheight( void )
{
    int height_size = 0;
    FXShutterItem *test = ( FXShutterItem * ) this->getFirst ();
    int x = getCurrent ();
    int zz = 0;

    while ( test )
    {
        if ( zz == x )
        {
            FXWindow * wind = test->getContent () ->getFirst ();

            while ( wind )
            {
                height_size += wind->getDefaultHeight ();
                wind = wind->getNext ();
            }

        }
        test = ( FXShutterItem * ) test->getNext ();
        zz++;
    }
    if ( this->getFirst () != NULL )
        height_size += ( ( ( FXShutterItem * ) this->getFirst () ) ->getButton () ->getDefaultHeight () + 2 ) * zz - 1;
    return height_size;
}
