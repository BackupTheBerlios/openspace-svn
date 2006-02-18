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
// C++ Interface: $MODULE$
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//  FTP plugin derived from OpenSpace file list VFS plugin.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef FILELIST_ftp
#define FILELIST_ftp

#include "../../OSVirtualFileSystemBase.h"
#include "../../OSPathType.h"
#include "../../OSFile.h"
#include "pftp.h"

#include <map>
#include <string>
#include <vector>

// Forward declarations
class OSThreadExec;
class OSConfigure;

/**
 * Events logger class.
 * 
 * @todo Encapsulate public data members.
 */
class SimpleLogger : public Logger
{
public:

    OSThreadExec* te;

    /**
     * Default constructor.
     */
    SimpleLogger( void );

    void start( uint32_t bytes, const FXString & name, uint32_t size );
    bool update( uint32_t bytes, const FXString & s );
    void end( uint32_t bytes, const FXString & );
    void choke( void );
    void error( int error );
    void logLine( FXString & line );
    void error( FXString & error );

}; // SimpleLogger


/**
 * FTP plugin main class.
 * 
 * @todo Pass "big" vectors through by-reference not by-value!
 */
class OSVfsFtp : public OSVirtualFileSystemBase
{

private:

    PFTP *pftp;
    SimpleLogger *log;
    std::string dir;

    std::map<std::string, OSFile> filesMap;
    std::map<std::string, OSFile>::iterator iter;
    std::map<std::string, OSFile> filesMapGlobal;
    std::map<std::string, OSFile>::iterator iterGlobal;

    int fieldsnum;
    std::vector<std::string> fields;

    int priv_osopendir( std::string dir, std::string prefix, std::map <std::string, OSFile> & filesMap, std::map <std::string, OSFile>::iterator & iter );
    void gorecursive( std::string file, unsigned long &size );
    OSFile priv_osreaddir( std::map <std::string, OSFile> & filesMap, std::map <std::string, OSFile>::iterator & iter2 );
    void getRecursiveFiles( std::vector<std::string> src, unsigned long &size );
    void goLocalRecursive( std::string path, std::string prefix, OSThreadExec* te );
    void local_totalsize( std::string path, unsigned long &size );
    int str_mode_int( std::string per );

public:
    
    int osopendir( std::string dir );
    OSFile osreaddir( void );
    int mkdir( std::string dir, int mode = 0 );
    int copy( OSThreadExec * te );
    int move( OSThreadExec * te );
    int remove( OSThreadExec * te );
    int rename( std::string orgname, std::string newname );
    int init( long id, std::vector<std::string>* vector_name, OSPathType pt, OSConfigure * conf );
    int mode( std::string file );
    std::string owner( std::string file );
    std::string group( std::string file );
    bool mode( std::string file, unsigned int, bool recursive );
    bool owner( std::string file, std::string, bool recursive );
    bool group( std::string file, std::string, bool recursive );
    std::string info( void );
    void totalsize( std::string path, unsigned long &size );
    std::string symlink( std::string path );
    bool symlink( std::string src, std::string dst );
    bool hardlink( std::string src, std::string dst );
    OSVirtualFileSystemInfo setup( void );
    int quit( void );
    std::string getinitialdir( void );


};
#endif
