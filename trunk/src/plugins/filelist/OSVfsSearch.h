#ifndef FILELIST_search
#define FILELIST_search

#include "../../OSVirtualFileSystemBase.h"
#include "../../OSPathType.h"
#include "OSVfsLocal.h"

#include <string>
#include <vector>

// Forward declarations
class OSThreadClass;
class OSConfigure;

/**
 * Search plugin class
 *
 * @todo To rename.
 */
class OSVfsSearch : public OSVirtualFileSystemBase
{
  
private:
  
  FILE *pipe;
  char readbuf[1024];
  std::vector<std::string> files;
  std::vector<std::string>::iterator iter;

  OSVfsLocal *fil_local;
  
public:
  
    int osopendir (std::string dir);
    OSFile osreaddir (void);
    int mkdir (std::string dir, int mode = 0);
    int copy (OSThreadExec * te);
    int move (OSThreadExec * te);
    int remove (OSThreadExec * te);
    int rename (std::string orgname, std::string newname);
    int init (long id, std::vector<std::string>* vector_name, OSPathType pt, OSConfigure * conf);
    int mode (std::string file);
    std::string owner (std::string file);
    std::string group (std::string file);
    bool mode (std::string file, unsigned int, bool recursive);
    bool owner (std::string file, std::string, bool recursive);
    bool group (std::string file, std::string, bool recursive);
    std::string info (void);
    void totalsize (std::string path, unsigned long &size);
    std::string symlink (std::string path);
    bool symlink (std::string src, std::string dst);
    bool hardlink (std::string src, std::string dst);
    OSVirtualFileSystemInfo setup(void);
    int quit (void);
};
#endif
