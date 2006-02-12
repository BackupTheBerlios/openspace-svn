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
// C++ Implementation: $MODULE$
//
// Author: Mateusz Dworak <http://openspace.linux.pl>, (C) 2005
//
// Description:
//  FTP plugin derived from OpenSpace file list VFS plugin.
//
/////////////////////////////////////////////////////////////////////////////


#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif

#include "OSVfsFtp.h"
#include "../../OSVirtualFileSystemBase.h"
#include "../../OSFile.h"
#include "../../OSPathType.h"
#include "../../OSConfigure.h"
#include "../../OSThreadExec.h"

#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include <string>
#include <vector>
#include <map>
using namespace std;


SimpleLogger::SimpleLogger()
{
    te = NULL;
}

void SimpleLogger::start( uint32_t bytes, const FXString & name, uint32_t size )
{
    if ( te != NULL && name != "" )
    {
        te->act_file_size = 0;
        te->act_file_name = name.text ();
        if ( size != 0 )
            te->file_size = size;
    }
}
bool SimpleLogger::update( uint32_t bytes, const FXString & s )
{
    if ( te != NULL )
    {
        te->act_file_size += bytes;
        te->act_total_size += bytes;

        if ( te->cancel == true )
            return false;
    }
    return true;
}
void SimpleLogger::end( uint32_t bytes, const FXString & )
{}

void SimpleLogger::choke()
{}

void SimpleLogger::error( int error )
{}

void SimpleLogger::logLine( FXString & line )
{
    fxmessage( "LOG=%s\n", line.text() );
}
void SimpleLogger::error( FXString & error )
{
    fxmessage( "ERROR=%s\n", error.text() );
}

OSVirtualFileSystemInfo OSVfsFtp::setup ( void )
{
    OSVirtualFileSystemInfo v;

    v.vfsheaders.push_back( OSVirtualFileSystemHeader( "name" ) );
    v.vfsheaders.push_back( OSVirtualFileSystemHeader( "size", "size" ) );
    v.vfsheaders.push_back( OSVirtualFileSystemHeader( "owner" ) );
    v.vfsheaders.push_back( OSVirtualFileSystemHeader( "group" ) );
    v.information = "FTP - default plugin";
    v.version = "1";
    v.type = "network";
    return v;

}


int OSVfsFtp::priv_osopendir ( std::string dir, std::string prefix, std::map <std::string, OSFile> & filesMap, std::map <std::string, OSFile>::iterator & iter )
{

    filesMap.clear();


    FXString di = dir.c_str();
    pftp->setDir( di );
    FXString di2;
    pftp->pwd( di2 );

    if ( FXFile::name( di ) != FXFile::name( di2 ) || di2 == "" )
        return -1;



    FXMemoryStream str;
    str.open( FXStreamSave, NULL );


    pftp->list( str );

    // nil terminate the buffer
    FXuchar nil = 0;
    str.save( &nil, 1 );

    // and get it
    FXuchar* buffer;
    unsigned long sp;
    str.takeBuffer( buffer, sp );

    char * ptr = reinterpret_cast<char *>( buffer );
    char * next = strchr( ptr, '\n' );
    char * start = ptr;


    //get attributes # user group size date =(3 or 4? fields) name
    while ( next && ( ptr - start < ( int ) sp ) )
    {

        next[ 0 ] = 0;
        if ( next[ -1 ] == '\r' )
            next[ -1 ] = 0;

        // Some FTP servers send other lines than raw directory
        if ( ptr[ 0 ] != 'd' && ptr[ 0 ] != '-' )
        {
            ptr = next + 1;
            next = strchr( ptr, '\n' );
            continue;
        }



        OSFile os_file;
        os_file.type = 0;
        if ( ptr[ 0 ] == 'd' )
            os_file.type = os_file.type | FOLDER;
        else
            os_file.type = 0;

        std::stringstream parser ( ptr );
        std::string field;

        std::string user;
        std::string group;
        std::string size;
        std::string date;
        std::string attrib;
        std::string name;

        parser >> attrib;

        attrib = attrib.substr( 1, attrib.length() - 1 );

        parser >> field;
        parser >> user;
        parser >> group;
        parser >> size;
        os_file.size = atoi( size.c_str() );


        parser >> date;
        parser >> date;
        parser >> date;


        os_file.user = user;
        os_file.group = group;
        os_file.mode = attrib;



        std::string n;
        while ( parser >> n )
        {
            if ( name != "" )
                name = name + " " + n;
            else
                name = n;
        }

        os_file.name = prefix + name;


        if ( prefix == "" )
        {


            for ( int i = 0; i < fieldsnum - 1; i++ )
            {

                if ( fields[ i + 1 ] == "size" )
                    os_file.attrib.push_back( size );
                else if ( fields[ i + 1 ] == "owner" )
                    os_file.attrib.push_back( user );
                else if ( fields[ i + 1 ] == "group" )
                    os_file.attrib.push_back( group );
                else if ( fields[ i + 1 ] == "mode" )
                    os_file.attrib.push_back( attrib );


            }

        }
        filesMap[ os_file.name ] = os_file;



        if ( prefix != "" )
        {
            filesMapGlobal[ os_file.name ] = os_file;
        }
        ptr = next + 1;
        next = strchr( ptr, '\n' );
    }
    FXFREE( &buffer );

    iter = filesMap.begin();

}



int OSVfsFtp::osopendir ( std::string dir )
{


    this->dir = dir;

    return priv_osopendir( dir, "", filesMap, iter );



}

OSFile OSVfsFtp::priv_osreaddir ( std::map <std::string, OSFile> & filesMap, std::map <std::string, OSFile>::iterator & iter )
{
    OSFile os_file;

    if ( iter != filesMap.end() )
    {
        os_file = iter->second;

        iter++;
        return os_file;
    }



    os_file.name = "";
    return os_file;

}


OSFile OSVfsFtp::osreaddir ( void )
{

    return priv_osreaddir( filesMap, iter );
}
int OSVfsFtp::mkdir ( std::string dir, int mode )
{
    pftp->mkDir( dir.c_str() );
}


static const int BUCKETSIZE = 128;
struct Bucket
{
    uint8_t bucket[ BUCKETSIZE ];
};








void OSVfsFtp::local_totalsize ( std::string path, unsigned long &size )
{

    if ( FXFile::isDirectory ( path.c_str () ) )
    {
        struct stat status;
        struct dirent *dp;
        DIR *dirp;


        dirp = opendir ( path.c_str () );

        while ( ( dp = readdir ( dirp ) ) != NULL )
        {
            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {

                std::string file = path;
                file.append ( dp->d_name );
#ifndef WIN32
                lstat ( file.c_str (), &status );
#else
                stat ( file.c_str (), &status );
#endif
                if ( S_ISDIR ( status.st_mode ) )
                {
                    size = size + ( unsigned int ) status.st_size;
                    file.append ( "/" );
                    totalsize ( file, size );

                }
                else
                {
                    size = size + status.st_size;
                }
            }
        }

        closedir ( dirp );
    }
    else
    {
        size += FXFile::size ( path.c_str () );

    }

}







int OSVfsFtp::copy ( OSThreadExec * te )
{
    log->te = te;
    unsigned long size = 0;

    std::string::size_type pos = te->options.find ( "upload" );
    if ( pos == std::string::npos )
    {

        std::string dir_copy = this->dir;

        std::vector < std::string >::iterator iter;

        for ( iter = te->src.begin (); iter != te->src.end(); iter++ )
        {
            std::string sr = ( *iter );
            if ( FXFile::isDirectory ( sr.c_str () ) )
                sr.append ( "/" );
            local_totalsize ( sr, size );
        }

        te->total_size = size;

        for ( iter = te->src.begin (); iter != te->src.end(); iter++ )
        {

            FXString file = iter->c_str();
            goLocalRecursive( file.text(), "", te );
            pftp->setDir( dir_copy.c_str() );

        }
        this->dir = dir_copy;

    }
    else
    {



        getRecursiveFiles( te->src, size );

        te->total_size = size;
        for ( iterGlobal = --filesMapGlobal.end();; iterGlobal-- )
        {
            if ( te->cancel == true )
                break;

            if ( iterGlobal->second.type & FOLDER )
            {

                std::string filename = te->dst + "/" + iterGlobal->first;
                FXFile::createDirectory( filename.c_str(), str_mode_int( iterGlobal->second.mode ) );

            }
            else
            {
                std::string filename = te->dst + "/" + iterGlobal->first;
                FXFile::createDirectory( FXFile::directory( filename.c_str() ), str_mode_int( iterGlobal->second.mode ) );
                te->file_size = iterGlobal->second.size;

                pftp->download( iterGlobal->first.c_str(), filename.c_str(), false );

                FXFile::mode( filename.c_str(), str_mode_int( iterGlobal->second.mode ) );
            }
            te->act_file_name = iterGlobal->first;
            if ( iterGlobal == filesMapGlobal.begin() ) break;

        }





    }
    log->te = NULL;
}


void OSVfsFtp::goLocalRecursive ( std::string path, std::string prefix, OSThreadExec *te )
{

    if ( te->cancel == true )
        return ;

    if ( FXFile::isDirectory ( path.c_str () ) )
    {
        struct stat status;
        struct dirent *dp;
        DIR *dirp;

        prefix = prefix + FXFile::name( path.c_str() ).text();
        std::string fulldir = this->dir + "/" + prefix;

        pftp->mkDir( fulldir.c_str() );
        this->mode ( fulldir, FXFile::mode( path.c_str() ), false );
        pftp->setDir( fulldir.c_str() );

        dirp = opendir ( path.c_str () );

        while ( ( dp = readdir ( dirp ) ) != NULL )
        {
            if ( dp->d_name[ 0 ] != '.' || ( dp->d_name[ 1 ] != '\0' && ( dp->d_name[ 1 ] != '.' || dp->d_name[ 2 ] != '\0' ) ) )
            {

                std::string file = path + "/";
                file.append ( dp->d_name );
                goLocalRecursive( file, prefix + "/", te );
            }
        }

        closedir ( dirp );
    }
    else
    {
        std::string pre = this->dir + "/" + prefix.substr( 0, prefix.length() - 1 );
        if ( this->dir != pre )
        {
            pftp->setDir( pre.c_str() );
            this->dir = pre;
        }

        FXString fil = path.c_str();
        pftp->upload( fil, 0, false );
        this->mode ( path, FXFile::mode( fil ), false );


    }

}








int OSVfsFtp::move ( OSThreadExec * te )
{}


void OSVfsFtp::gorecursive( std::string file, unsigned long &size )
{


    std::map <std::string, OSFile> filesMap;
    std::map <std::string, OSFile>::iterator iter;

    if ( priv_osopendir( file, file + "/", filesMap, iter ) != -1 )
    {

        while ( 1 )
        {
            OSFile os_file = priv_osreaddir ( filesMap, iter );
            if ( os_file.name == "" )
                break;

            if ( os_file.type & FOLDER )
            {
                gorecursive( os_file.name, size );
            }
            else
                size = size + os_file.size;
        }

    }


}

void OSVfsFtp::getRecursiveFiles( std::vector < std::string >src, unsigned long &size )
{

    filesMapGlobal.clear();


    std::vector < std::string >::iterator iter_files;

    for ( iter_files = src.begin (); iter_files != src.end(); iter_files++ )
    {

        std::string name = FXFile::name( iter_files->c_str() ).text();
        OSFile os_file;
        os_file.name = name;
        os_file.type = filesMap[ name ].type;
        os_file.size = filesMap[ name ].size;
        os_file.mode = filesMap[ name ].mode;
        os_file.group = filesMap[ name ].user;
        os_file.user = filesMap[ name ].group;
        filesMapGlobal[ name ] = os_file;

    }


    for ( iterGlobal = filesMapGlobal.begin();iterGlobal != filesMapGlobal.end(); iterGlobal++ )
    {


        if ( iterGlobal->second.type & FOLDER )
        {
            gorecursive( iterGlobal->first, size );
            FXString di = this->dir.c_str();
            pftp->setDir( di );
        }
        else
            size = size + iterGlobal->second.size;

    }
}

int OSVfsFtp::remove ( OSThreadExec * te )
{
    unsigned long size;
    getRecursiveFiles( te->src, size );

    for ( iterGlobal = --filesMapGlobal.end();; iterGlobal-- )
    {


        if ( iterGlobal->second.type & FOLDER )
            pftp->rmDir( iterGlobal->first.c_str() );
        else
            pftp->del( iterGlobal->first.c_str() );

        te->act_file_name = iterGlobal->first;
        if ( iterGlobal == filesMapGlobal.begin() ) break;

    }



}
int OSVfsFtp::rename ( std::string orgname, std::string newname )
{

    FXString tmp;
    pftp->sendCmd( "RNFR ", FXFile::name( orgname.c_str() ), tmp );
    pftp->sendCmd( "RNTO ", FXFile::name( newname.c_str() ), tmp );

}
int OSVfsFtp::init ( std::vector < std::string > *vector_name, OSPathType pt, OSConfigure * conf )
{

    fieldsnum = vector_name->size ();
    fields = ( *vector_name );

    log = new SimpleLogger;
    pftp = new PFTP( pt.server.c_str(), pt.user.c_str(), pt.password.c_str(), "", log );

    if ( pftp->isConnected() && getinitialdir() != "" )
    {}
    else return -1;
}

int OSVfsFtp::str_mode_int( std::string per )
{
    unsigned int mode = 0;
    if ( per[ 0 ] == 'r' )
        mode = mode | S_IRUSR;
    if ( per[ 1 ] == 'w' )
        mode = mode | S_IWUSR;
    if ( per[ 2 ] == 'x' )
        mode = mode | S_IXUSR;
    if ( per[ 3 ] == 'r' )
        mode = mode | S_IRGRP;
    if ( per[ 4 ] == 'w' )
        mode = mode | S_IWGRP;
    if ( per[ 5 ] == 'x' )
        mode = mode | S_IXGRP;
    if ( per[ 6 ] == 'r' )
        mode = mode | S_IROTH;
    if ( per[ 7 ] == 'w' )
        mode = mode | S_IWOTH;
    if ( per[ 8 ] == 'x' )
        mode = mode | S_IXOTH;
    return mode;
}

int OSVfsFtp::mode ( std::string file )
{

    std::string per = filesMap[ FXFile::name( file.c_str() ).text() ].mode;

    return str_mode_int( per );

}
std::string OSVfsFtp::owner ( std::string file )
{
    return filesMap[ FXFile::name( file.c_str() ).text() ].user;
}
std::string OSVfsFtp::group ( std::string file )
{
    return filesMap[ FXFile::name( file.c_str() ).text() ].group;
}
bool OSVfsFtp::mode ( std::string file, unsigned int per, bool recursive )
{

    unsigned int mode = per;

    int x = ( ( mode & S_IRUSR ) | ( mode & S_IWUSR ) | ( mode & S_IXUSR ) ) / 64;
    int y = ( ( mode & S_IRGRP ) | ( mode & S_IWGRP ) | ( mode & S_IXGRP ) ) / 8;
    int z = ( ( mode & S_IROTH ) | ( mode & S_IWOTH ) | ( mode & S_IXOTH ) );

    file = FXFile::name( file.c_str() ).text();

    FXString tmp;
    char chstr[ 20 ];
    sprintf ( chstr, "%d%d%d", x, y, z );

    if ( recursive == false )
    {
        std::string cmd = "CHMOD " + std::string( chstr ) + " " + file;
        pftp->sendCmd( "SITE ", cmd.c_str(), tmp );
    }
    else
    {
        unsigned long size;
        std::vector < std::string >src;
        src.push_back( file );
        getRecursiveFiles( src, size );

        for ( iterGlobal = --filesMapGlobal.end();; iterGlobal-- )
        {


            std::string cmd = "CHMOD " + std::string( chstr ) + " " + iterGlobal->first;
            pftp->sendCmd( "SITE ", cmd.c_str(), tmp );


            if ( iterGlobal == filesMapGlobal.begin() ) break;

        }


    }



}
bool OSVfsFtp::owner ( std::string file, std::string, bool recursive )
{}
bool OSVfsFtp::group ( std::string file, std::string, bool recursive )
{}
std::string OSVfsFtp::info ( void )
{
    return "";
}
void OSVfsFtp::totalsize ( std::string path, unsigned long &size )
{
    size = 0;

    std::string file = FXFile::name( path.c_str() ).text();
    if ( file == "" )
        file = FXFile::name( path.substr( 0, path.length() - 1 ).c_str() ).text();



    if ( filesMap[ file ].type & FOLDER )
    {
        std::vector < std::string >src;
        src.push_back( file );
        getRecursiveFiles( src, size );
    }
    else
        size = filesMap[ file ].size;



}
std::string OSVfsFtp::symlink ( std::string path )
{
    return "";
}
bool OSVfsFtp::symlink ( std::string src, std::string dst )
{}
bool OSVfsFtp::hardlink ( std::string src, std::string dst )
{}

int OSVfsFtp::quit ( void )
{
    pftp->logout();
}

std::string OSVfsFtp::getinitialdir( void )
{
    FXString base;
    pftp->pwd( base );

    return base.text();
}



EXPORTFUNCTION OSVirtualFileSystemBase *get_filelist ( void )
{
    FXTRACE ( ( 5, "PLUGIN LOAD\n" ) );
    return new OSVfsFtp ();
}
