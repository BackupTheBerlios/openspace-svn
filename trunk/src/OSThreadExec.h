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
// C++ Interface: OSThreadExec
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description: 
//
/////////////////////////////////////////////////////////////////////////////
#ifndef THREAD_ELEM
#define THREAD_ELEM

#include <fx.h>
#include "OSVirtualFileSystemBase.h"
#include <string>
#include <vector>
#include <ctime>

/**
 * Worker theads executor.
 * In order to execute something as thread you have to create object of this 
 * class it is used to send information beetwen main program and threads.
 * 
 * @todo Fill missing documentation for class interface.
 * @todo Again, all data members are public!
 */
class OSThreadExec
{
public:

    /**
     * Default constructor.
     */
    OSThreadExec(void);
    
    /**
     * Constructor.
     * @param fb
     * @param command
     * @param options
     * @param src
     * @param dst
     */
    OSThreadExec(OSVirtualFileSystemBase * fb, std::string command, std::string options, std::string src, std::string dst = "" );
    
    /**
     * Constructor.
     * @param fb
     * @param command
     * @param options
     * @param src
     * @param dst
     */
    OSThreadExec(OSVirtualFileSystemBase * fb, std::string command, std::string options, std::vector<std::string> src = std::vector<std::string>(), std::string dst = "" );
    
    /**
     * Initializes thread procedure object.
     */
    void init( void );

    FXMutex mutex;
    void *gui;
    void *filel;
    OSVirtualFileSystemBase *fb;

    std::string command;
    std::vector<std::string> src;
    std::string dst;
    std::string options;
    std::string msg;
    int p[2];
    time_t* tim;

    std::string str1;
    std::string str2;
    std::string str3;

    unsigned long total_size;
    unsigned long act_total_size;

    unsigned long file_size;
    unsigned long act_file_size;

    std::string act_file_name;

    bool cancel;
    bool end;
    bool question;
    bool answer;
    bool overwrite;
    bool all;
    bool error;
    pthread_t pid;
    FXMenuPane *pane;
    long id;


};

#endif

