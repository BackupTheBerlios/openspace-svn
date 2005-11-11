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
// C++ Interface: OSInfoPanel
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSINFOPANEL_H__
#define __OSINFOPANEL_H__

#include <string>

// Forward declarations
class FXButton;
class FXComposite;
class FXLabel;
class FXProgressBar;
class FXVerticalFrame;
    
/**
 * Info panel class.
 * Info panel displays details about operations being in progress i.e. during copying files.
 * 
 * @todo Is this correct to destroy only frame object in destructor? What about the rest?
 * @todo Get rid of public data members!
 */
class OSInfoPanel
{
public:
    
    /**
     * Constructor.
     * @param p - pointer to...
     * @param command - string with command...
     * 
     * @todo Add missing comments.
     */
    OSInfoPanel( FXComposite * p, std::string command );
    
    /**
     * Destructor.
     */
    ~OSInfoPanel( void );
    
    FXVerticalFrame* frame;
    FXLabel* lab0;
    FXLabel* lab1;
    FXLabel* lab2;
    FXProgressBar* pbar;
    FXProgressBar* pbar2;
    FXButton* bu;
};

#endif // #ifndef __OSINFOPANEL_H__