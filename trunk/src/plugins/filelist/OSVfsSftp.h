// $Id: OSVfsSftp.h 255 2006-02-18 17:08:23Z compbatant $
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
// C++ Interface: $MODULE$
//
// Author: Mateusz Dworak <http://nao.linux.pl>, (C) 2005
//
// Description: 
//
/////////////////////////////////////////////////////////////////////////////
#ifndef FILELIST_LOCAL
#define FILELIST_LOCAL

#include"../../OSVirtualFileSystemBase.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <vector>


#include <libssh/libssh.h>
#include <libssh/sftp.h>

// Forward declarations
class OSConfigure;
class OSFile;
class OSPathType;
class OSThreadExec;

/**
 * Local file list plugin class.
 * 
 * @todo This plugin will be reorganized/renamed.
 */
class OSVfsSftp: public OSVirtualFileSystemBase
{
private:

    int fieldsnum;
    std::string dir;
    std::vector<std::string> fields;
    int copymove( OSThreadExec * te, bool copy = true );
    SSH_SESSION *session;
    SFTP_SESSION *sftp;
    SFTP_DIR *dirsftp;
    int version;

public:

    int osopendir ( std::string dir );
    OSFile osreaddir ( void );
    int mkdir ( std::string dir, int mode );
    int copy ( OSThreadExec * te );
    int move ( OSThreadExec * te );
    int remove ( OSThreadExec * te );
    int rename ( std::string orgname, std::string newname );
    int init ( long id, std::vector<std::string> *vector_name, OSPathType pt, OSConfigure* conf );
    int mode ( std::string file );
    std::string owner ( std::string file );
    std::string group ( std::string file );
    bool mode ( std::string file, unsigned int, bool recursive = false );
    bool owner ( std::string file, std::string ownername, bool recursive = false );
    bool group ( std::string file, std::string groupname, bool recursive = false );
    std::string info ( void );
    int quit ( void );
    std::string symlink ( std::string path );
    bool symlink ( std::string src, std::string dst );
    bool hardlink ( std::string src, std::string dst );
    OSVirtualFileSystemInfo setup ( void );
    void totalsize ( std::string path, unsigned long &size );
    void local_totalsize ( std::string path, unsigned long &size );
    void goLocalRecursive ( std::string path, std::string prefix, OSThreadExec *te );
    void goRecursive ( std::string path, std::string prefix, OSThreadExec *te );
    void chmodRecursive(std::string path, unsigned int mod);
    int removeRecursive ( std::string path, OSThreadExec *te );
    int  uploadFile (std::string from, std::string to, OSThreadExec* te);
    int  downloadFile (std::string from, std::string to, OSThreadExec* te);
    std::string getinitialdir( void );

};

#endif

