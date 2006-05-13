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
// C++ Interface: OSMimeApp
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSMIMEAPP_H__
#define __OSMIMEAPP_H__

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <fx.h>
/**
 * @todo Add missing class documentation.
 */
class OSMimeApp : public FXDialogBox
{
FXDECLARE( OSMimeApp ) protected:

private:

    /**
     * Default constructor.
     * 
     * @todo What is the reason to declare this constructor as private?
     */
    OSMimeApp( void );

    FXComboBox *programsbox;
    FXLabel *mime_label;

    std::vector<std::string> mime_vector;
    std::vector<std::string>::iterator iter;

public:
    enum
    {
        ID_NEXT = FXDialogBox::ID_LAST,
        ID_PREVIOUS,
        ID_LAST,
    };

    /**
     * Constructor.
     */
    OSMimeApp( FXWindow * owner );

    /**
     * Virtual destructor.
     */
    virtual ~OSMimeApp( void );

    long onNextPrevious( FXObject * sender, FXSelector sel, void * );
    long onClose( FXObject * sender, FXSelector sel, void *ptr );

    void fill( std::string tmp );
    static void save( std::string mime, std::string program );
    void doAutomaticConfiguration( void );

};

#endif // #ifndef __OSMIMEAPP_H__
