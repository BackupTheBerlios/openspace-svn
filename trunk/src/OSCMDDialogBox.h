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
// C++ Interface: OSCMDDialogBox
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description: 
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSCMDDIALOGBOX_H__
#define __OSCMDDIALOGBOX_H__

#include <fx.h>
#include <vector>
#include <string>

#include "OSVirtualFileSystemBase.h"
#include "OSObjectManager.h"

using namespace std;


/**
 * Command dialog base class.
 * Abstract class definint common command dialog interface.
 * 
 * @todo Why there is no \e delete call on OSCMDDialogBox#ok and OSCMDDialogBox#cancel in the destructor?
 */
class OSCMDDialogBox : public FXDialogBox
{
    FXDECLARE( OSCMDDialogBox )
        
public:    // Public method declarations

    /**
     * Default constructor.
     */
    OSCMDDialogBox( void ) {}

    /**
     * Constructor.
     * 
     * @param owner - pointer to owner window.
     * @param fb - 
     * @param src -
     */
    OSCMDDialogBox( FXWindow * owner, OSVirtualFileSystemBase* fb = NULL, std::vector<std::string> src = vector < string >() );

    /**
     * Destructor.
     * Must be virtual because of abstract class.
     */
    virtual ~OSCMDDialogBox( void );

    /**
     * Execute command.
     */
    virtual int exec( void ) {};

    /**
     * UI commands constants.
     */
    enum
    {
        ID_EXEC = FXDialogBox::ID_LAST + 666,
        ID_COMMAND,
        ID_CANCEL_COMMAND, 
        ID_LAST
    };

protected:

    FXVerticalFrame* contents;

    FXHorizontalFrame* buttons;

    FXButton* ok;

    FXButton* cancel;

    OSVirtualFileSystemBase* fb;

    /**
     * @todo to document
     */
    std::vector<std::string> src;


};

#endif // #ifndef __OSCMDDIALOGBOX_H__
