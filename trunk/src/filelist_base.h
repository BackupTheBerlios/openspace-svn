#ifndef FILELIST_BASE
#define FILELIST_BASE

#include <string>
using namespace std;

#include "osfile.h"
#include "configure.h"
#include "thread_elem.h"
#include "pathtype.h"

//filelist plugins need to inherit from this class
class filelist_base
{
  public:
    virtual int osopendir (string dir) = 0;
    virtual osfile osreaddir (void) = 0;
    virtual int mkdir (string dir, int mode = 0) = 0;
    virtual int copy (thread_elem * te) = 0;
    virtual int move (thread_elem * te) = 0;
    virtual int remove (thread_elem * te) = 0;
    virtual int rename (string orgname, string newname) = 0;
    virtual int init (vector < string > *vector_name, pathtype pt, configure * conf) = 0;
    virtual int mode (string file) = 0;
    virtual string owner (string file) = 0;
    virtual string group (string file) = 0;
    virtual bool mode (string file, unsigned int, bool recursive) = 0;
    virtual bool owner (string file, string, bool recursive) = 0;
    virtual bool group (string file, string, bool recursive) = 0;
    virtual string info (void) = 0;
    virtual void totalsize (string path, unsigned long &size) = 0;
    virtual string symlink (string path) = 0;
    virtual bool symlink (string src, string dst) = 0;
    virtual bool hardlink (string src, string dst) = 0;
    virtual int supportedfunctions (void) = 0;
    virtual int quit (void) = 0;
    virtual string getinitialdir(void)
    {
    return "/";
    }

};
#endif
