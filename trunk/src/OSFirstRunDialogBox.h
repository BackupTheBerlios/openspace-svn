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
// C++ Interface: OSFirstRunDialogBox
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSFIRSTRUNDIALOGBOX_H_
#define __OSFIRSTRUNDIALOGBOX_H_

#include <fx.h>

/**
 * First run dialog box class.
 * This dialog pops up when nao file manager detects user has never launched it before
 * or all existing configuration was removed.
 */
class OSFirstRunDialogBox : public FXDialogBox
{
    FXDECLARE( OSFirstRunDialogBox ) 
    
protected:

private:
    
    OSFirstRunDialogBox( void );

public:

    enum
    {
        ID_CONF_MANUAL = FXDialogBox::ID_LAST,
        ID_CONF_SEMI,
        ID_CONF_AUTOMATIC,
        ID_LAST,
    };

    /**
     * Constructor.
     * @param owner - pointer to the dialog box owner window object.
     */
    OSFirstRunDialogBox( FXWindow * owner );

    /**
     * Virtual destructor.
     */
    virtual ~OSFirstRunDialogBox( void );
    
    /**
     * xxx event handler
     */
    long onConfigure( FXObject* sender, FXSelector sel, void* );
};

#endif // #ifndef __OSFIRSTRUNDIALOGBOX_H_
