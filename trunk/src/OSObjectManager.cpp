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
// C++ Implementation: OSObjectManager
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSObjectManager.h"

OSObjectManager* OSObjectManager::pinstance = 0; // initialize pointer

OSObjectManager* OSObjectManager::instance ( FXApp *app )
{
    if ( pinstance == 0 )   // is it the first call?
    {
        pinstance = new OSObjectManager( app ); // create sole instance

    }
    
    return pinstance; // address of sole instance
}

OSObjectManager::OSObjectManager( FXApp *app )
{
    //... perform necessary instance initializations

    captionfont1 = new FXFont( app, conf->readonestring ( "/NaoConfig/fonts/captionfont1" ).c_str() );
    captionfont1->create ();

    captionfont2 = new FXFont( app, conf->readonestring ( "/NaoConfig/fonts/captionfont2" ).c_str() );
    captionfont2->create ();

    captionfont3 = new FXFont( app, conf->readonestring ( "/NaoConfig/fonts/captionfont3" ).c_str() );
    captionfont3->create ();

    maincolor = fxcolorfromname( conf->readonestring ( "/NaoConfig/colors/main" ).c_str() );

    key_map[ "a" ] = 0x0061;
    key_map[ "b" ] = 0x0062;
    key_map[ "c" ] = 0x0063;
    key_map[ "d" ] = 0x0064;
    key_map[ "e" ] = 0x0065;
    key_map[ "f" ] = 0x0066;
    key_map[ "g" ] = 0x0067;
    key_map[ "h" ] = 0x0068;
    key_map[ "i" ] = 0x0069;
    key_map[ "j" ] = 0x006A;
    key_map[ "k" ] = 0x006B;
    key_map[ "l" ] = 0x006C;
    key_map[ "m" ] = 0x006D;
    key_map[ "n" ] = 0x006E;
    key_map[ "o" ] = 0x006F;
    key_map[ "p" ] = 0x0070;
    key_map[ "q" ] = 0x0071;
    key_map[ "r" ] = 0x0072;
    key_map[ "s" ] = 0x0073;
    key_map[ "t" ] = 0x0074;
    key_map[ "u" ] = 0x0075;
    key_map[ "v" ] = 0x0076;
    key_map[ "w" ] = 0x0077;
    key_map[ "x" ] = 0x0078;
    key_map[ "y" ] = 0x0079;
    key_map[ "z" ] = 0x007A;


    key_map[ "0" ] = 0x0030;
    key_map[ "1" ] = 0x0031;
    key_map[ "2" ] = 0x0032;
    key_map[ "3" ] = 0x0033;
    key_map[ "4" ] = 0x0034;
    key_map[ "5" ] = 0x0035;
    key_map[ "6" ] = 0x0036;
    key_map[ "7" ] = 0x0037;
    key_map[ "8" ] = 0x0038;
    key_map[ "9" ] = 0x0039;


    key_map[ "BackSpace" ] = 0xFF08;
    key_map[ "Tab" ] = 0xFF09;
    key_map[ "Linefeed" ] = 0xFF0A;
    key_map[ "Clear" ] = 0xFF0B;
    key_map[ "Return" ] = 0xFF0D;
    key_map[ "Pause" ] = 0xFF13;
    key_map[ "Scroll_Lock" ] = 0xFF14;
    key_map[ "Sys_Req" ] = 0xFF15;
    key_map[ "Escape" ] = 0xFF1B;
    key_map[ "Delete" ] = 0xFFFF;
    key_map[ "Multi_key" ] = 0xFF20;


    key_map[ "F1" ] = 0xFFBE;
    key_map[ "F2" ] = 0xFFBF;
    key_map[ "F3" ] = 0xFFC0;
    key_map[ "F4" ] = 0xFFC1;
    key_map[ "F5" ] = 0xFFC2;
    key_map[ "F6" ] = 0xFFC3;
    key_map[ "F7" ] = 0xFFC4;
    key_map[ "F8" ] = 0xFFC5;
    key_map[ "F9" ] = 0xFFC6;
    key_map[ "F10" ] = 0xFFC7;
    key_map[ "F11" ] = 0xFFC8;
    key_map[ "F12" ] = 0xFFC9;

    key_map[ "Space" ] = 0x0020;
}
