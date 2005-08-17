#ifndef FILELIST_sftp
#define FILELIST_sftp
#include "../../filelist_base.h"
class filelist_sftp:public filelist_base
{
    string dir;  

    int p[2];
    int o[2];

    char readbuffer[20];
    int nbytes;
    int end;
    string prompt;

  public:
    int filelist_sftp::osopendir (string dir);
    osfile filelist_sftp::osreaddir (void);
    int filelist_sftp::mkdir (string dir, int mode = 0);
    int filelist_sftp::copy (thread_elem * te);
    int filelist_sftp::move (thread_elem * te);
    int filelist_sftp::remove (thread_elem * te);
    int filelist_sftp::rename (string orgname, string newname);
    int filelist_sftp::init (vector < string > *vector_name, pathtype pt, configure * conf);
    int filelist_sftp::mode (string file);
    string filelist_sftp::owner (string file);
    string filelist_sftp::group (string file);
    bool filelist_sftp::mode (string file, unsigned int, bool recursive);
    bool filelist_sftp::owner (string file, string, bool recursive);
    bool filelist_sftp::group (string file, string, bool recursive);
    string filelist_sftp::info (void);
    void filelist_sftp::totalsize (string path, unsigned long &size);
    string filelist_sftp::symlink (string path);
    bool filelist_sftp::symlink (string src, string dst);
    bool filelist_sftp::hardlink (string src, string dst);
    int filelist_sftp::supportedfunctions (void);
    int filelist_sftp::quit (void);
    
    
    bool filelist_sftp::getprompt (void);
};
#endif
