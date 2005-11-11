#ifndef FILELIST_SCP
#define FILELIST_SCP

#include"../../OSVirtualFileSystemBase.h"

#include <stdio.h>


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>




class filelist_scp:public filelist_base
{
    string dir;
    string display_size;
    int fieldsnum;
    string *fields;


    int p[2];
    int o[2];

    char readbuffer[20];

    int nbytes;

    int end;

    string prompt;


  public:

    int osopendir (string dir);
    osfile osreaddir (void);
    int mkdir (string dir, int mode);
    int copy (thread_elem * te);
    int move (thread_elem * te);
    int remove (thread_elem * te);
    int rename (string orgname, string newname);
    int init (string ** name, unsigned int **type, unsigned int **width, pathtype pt, configure * conf);
    unsigned int mode (string file);
    bool mode (string file, unsigned int, bool recursive = false);
    string info (void);
    int quit (void);

    void totalsize (string path, unsigned long &size);
    int copymove (thread_elem * te, bool copy = true);

    bool getprompt (void);


};


#endif
