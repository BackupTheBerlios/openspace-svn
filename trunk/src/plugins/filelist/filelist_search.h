#ifndef FILELIST_search
#define FILELIST_search
#include "../../filelist_base.h"
#include "filelist_local.h"

class filelist_search:public filelist_base
{
  public:
  
  FILE *pipe;
  char readbuf[1024];
  vector < string > files;
  vector < string >::iterator iter;

  filelist_local *fil_local;
  
    int filelist_search::osopendir (string dir);
    osfile filelist_search::osreaddir (void);
    int filelist_search::mkdir (string dir, int mode = 0);
    int filelist_search::copy (thread_elem * te);
    int filelist_search::move (thread_elem * te);
    int filelist_search::remove (thread_elem * te);
    int filelist_search::rename (string orgname, string newname);
    int filelist_search::init (vector < string > *vector_name, pathtype pt, configure * conf);
    int filelist_search::mode (string file);
    string filelist_search::owner (string file);
    string filelist_search::group (string file);
    bool filelist_search::mode (string file, unsigned int, bool recursive);
    bool filelist_search::owner (string file, string, bool recursive);
    bool filelist_search::group (string file, string, bool recursive);
    string filelist_search::info (void);
    void filelist_search::totalsize (string path, unsigned long &size);
    string filelist_search::symlink (string path);
    bool filelist_search::symlink (string src, string dst);
    bool filelist_search::hardlink (string src, string dst);
    int filelist_search::supportedfunctions (void);
    int filelist_search::quit (void);
};
#endif
