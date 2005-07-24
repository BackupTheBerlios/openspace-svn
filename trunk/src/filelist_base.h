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
    virtual int init (vector < string > *vector_name, vector < int >*vector_type, vector < int >*vector_width, pathtype pt, configure * conf) = 0;
    virtual unsigned int mode (string file) = 0;
    virtual bool mode (string file, unsigned int, bool recursive) = 0;
    virtual string info (void) = 0;
    virtual void totalsize (string path, unsigned long &size) = 0;
    virtual int quit (void) = 0;

};
#endif
