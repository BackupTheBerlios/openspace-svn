#ifndef FILELIST_archive
#define FILELIST_archive
#include "../../OSVirtualFileSystemBase.h"
class filelist_archive:public filelist_base
{
 
private:  
    string archive_filename;
    FILE *pipe;
    char readbuf[1024];
    vector < string > files;
    vector < string >::iterator iter;
    string dir;
    string type;
 public:    
    int filelist_archive::osopendir (string dir);
    osfile filelist_archive::osreaddir (void);
    int filelist_archive::mkdir (string dir, int mode = 0);
    int filelist_archive::copy (thread_elem * te);
    int filelist_archive::move (thread_elem * te);
    int filelist_archive::remove (thread_elem * te);
    int filelist_archive::rename (string orgname, string newname);
    int filelist_archive::init (vector < string > *vector_name, pathtype pt, configure * conf);
    int filelist_archive::mode (string file);
    string filelist_archive::owner (string file);
    string filelist_archive::group (string file);
    bool filelist_archive::mode (string file, unsigned int, bool recursive);
    bool filelist_archive::owner (string file, string, bool recursive);
    bool filelist_archive::group (string file, string, bool recursive);
    string filelist_archive::info (void);
    void filelist_archive::totalsize (string path, unsigned long &size);
    string filelist_archive::symlink (string path);
    bool filelist_archive::symlink (string src, string dst);
    bool filelist_archive::hardlink (string src, string dst);
    vfs filelist_archive::setup (void);
    int filelist_archive::quit (void);
    
    int filelist_archive::filesoperation(string type,thread_elem * te);
};
#endif
