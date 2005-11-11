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
// C++ Interface: OSFileListItem
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSFILELISTITEM_H__
#define __OSFILELISTITEM_H__

#include "OSFile.h"

/**
 * File list item class.
 * This class adds new features to class FXIconItem from FOX Toolkit,
 * in example big icon view.
 * 
 * @todo Get rid of private data members!
 */
class OSFileListItem : public FXIconItem
{
    FXDECLARE( OSFileListItem )

public:
    
    /**
     * Default constructor.
     */
    OSFileListItem( void );
    
    /**
     * Constructor.
     */
    OSFileListItem( FXIconList* list, osfile osf, FXColor col, FXColor backcol, FXIcon* ic1 = NULL, FXIcon* ic2 = NULL );
    
    OSFile osf;
    FXColor color;
    FXColor backcolor;
    FXIconList* list;
    
protected:
    
    void drawDetails ( const FXIconList * list, FXDC & dc, FXint x, FXint y, FXint w, FXint h ) const;
    void drawBigIcon ( const FXIconList * list, FXDC & dc, FXint x, FXint y, FXint w, FXint h ) const;
    FXint hitItem ( const FXIconList * list, FXint rx, FXint ry, FXint rw, FXint rh ) const;
    FXint getHeight ( const FXIconList * list ) const;

    /*
      // Overrides the base class version of draw()
     
    virtual void drawMiniIcon(const FXIconList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
    virtual void drawBigIcon(const FXIconList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
    virtual FXint hitItem(const FXIconList* list,FXint rx,FXint ry,FXint rw=1,FXint rh=1) const;
    */

};

#endif // #ifndef __OSFILELISTITEM_H__