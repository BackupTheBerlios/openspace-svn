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
// C++ Implementation: OSThreadExec
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////

#include "OSThreadExec.h"
#include <string>
#include <vector>

OSThreadExec::OSThreadExec()
{
    gui = NULL;
}

OSThreadExec::OSThreadExec(OSVirtualFileSystemBase *fb, std::string command, std::string options, std::string src, std::string dst )
{
    std::vector<std::string> src2;
    src2.push_back( src );

    this->command = command;
    this->src = src2;
    this->dst = dst;
    this->fb = fb;
    this->options = options;
    this->id=fb->id;



    init();

}

OSThreadExec::OSThreadExec(OSVirtualFileSystemBase *fb, std::string command, std::string options, std::vector<std::string> src, std::string dst )
{
    this->command = command;
    this->src = src;
    this->dst = dst;
    this->fb = fb;
    this->options = options;
    this->id=fb->id;

    init();
}

void OSThreadExec::init( void )
{
    str1 = "start";
    gui = NULL;

    act_total_size = 0;
    total_size = 1;
    file_size = 1;
    act_file_size = 0;

    tim = new time_t;
    time ( tim );

    end = false;
    question = false;
    answer = false;
    overwrite = false;
    cancel = false;
    all = false;
    error = false;

    act_file_name = "";
    msg = "operation failed";
}

