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
// $Id$
//
// C++ Interface: OSCMDDialogBox.h
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __OSVIRTUALFILESYSTEMBASE_H__
#define __OSVIRTUALFILESYSTEMBASE_H__

#include "OSVirtualFileSystemInfo.h"
#include "OSPathType.h"
#include <string>
#include <vector>

// Forward declarations
class OSConfigure;
class OSFile;
class OSThreadExec;

/**
 * Base class for VFS plugins.
 * It is abstract class declaring common interface for OpenSpace plugins.
 * All OpenSpace plugins are required to inherit from this class.
 * 
 * @todo Add virtual constructor because it's an abstract class.
 * @todo Check if this design and virtual polymorphism work correctly.
 * @todo Should we change names of functions to something like openDir instead of osopendir?
 */
class OSVirtualFileSystemBase
{
public:
    long id;
    
    virtual int osopendir( std::string dir ) = 0;
    virtual OSFile osreaddir( void ) = 0;
    virtual int mkdir( std::string dir, int mode = 0 ) = 0;
    virtual int copy( OSThreadExec* te ) = 0;
    virtual int move( OSThreadExec* te ) = 0;
    virtual int remove( OSThreadExec* te ) = 0;
    virtual int rename( std::string orgname, std::string newname ) = 0;
    virtual int init( long id,std::vector<std::string>* vector_name, OSPathType pt, OSConfigure* conf ) = 0;
    virtual int mode( std::string file ) = 0;
    virtual std::string owner( std::string file ) = 0;
    virtual std::string group( std::string file ) = 0;
    virtual bool mode( std::string file, unsigned int, bool recursive ) = 0;
    virtual bool owner( std::string file, std::string, bool recursive ) = 0;
    virtual bool group( std::string file, std::string, bool recursive ) = 0;
    virtual std::string info( void ) = 0;
    virtual void totalsize(std::string path, unsigned long &size ) = 0;
    virtual std::string symlink( std::string path ) = 0;
    virtual bool symlink( std::string src, std::string dst ) = 0;
    virtual bool hardlink( std::string src, std::string dst ) = 0;
    virtual int quit( void ) = 0;
    virtual std::string getinitialdir( void )
    {
        return "/";
    }

    virtual OSVirtualFileSystemInfo setup( void ) = 0;
};

#endif// #ifndef __OSVIRTUALFILESYSTEMBASE_H__
