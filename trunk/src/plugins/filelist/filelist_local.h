#ifndef FILELIST_LOCAL
#define FILELIST_LOCAL

#include"../../filelist_base.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>




class filelist_local:public filelist_base
{

    DIR *dp;
    dirent *dirrec;
    struct stat status;
    string dir;
    string display_size;
    int fieldsnum;
     vector < string > fields;


  public:

    int osopendir (string dir);
    osfile osreaddir (void);
    int mkdir (string dir, int mode);
    int copy (thread_elem * te);
    int move (thread_elem * te);
    int remove (thread_elem * te);
    int rename (string orgname, string newname);
    int init (vector < string > *vector_name, pathtype pt, configure * conf);
    int mode (string file);
    string owner (string file);
    string group (string file);
    bool mode (string file, unsigned int, bool recursive = false);
    bool owner (string file, string ownername, bool recursive = false);
    bool group (string file, string groupname, bool recursive = false);
    string info (void);
    int quit (void);
    string symlink (string path);
    bool symlink (string src, string dst);
    bool hardlink (string src, string dst);
    int supportedfunctions (void);

    void totalsize (string path, unsigned long &size);
    int copymove (thread_elem * te, bool copy = true);
};


#endif
