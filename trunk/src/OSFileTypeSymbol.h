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
// C++ Interface: OSFileTypeSymbol
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSFILETYPESYMBOL_H__
#define __OSFILETYPESYMBOL_H__

#include <fx.h>

/**
 * File type symbology class.
 * Associates icon and colors with file type.
 * 
 * @todo Again, public data members!
 */
class OSFileTypeSymbol
{
public:
        
    /**
     * Default constructor.
     */
    OSFileTypeSymbol( void );
    
    /**
     * Constructor.
     */
    OSFileTypeSymbol( FXIcon* ic, FXColor col, FXColor backcol );

public:
    
    FXIcon* icon;
    FXColor color;
    FXColor backcolor;
};

#endif // #ifndef __OSFILETYPESYMBOL_H__
