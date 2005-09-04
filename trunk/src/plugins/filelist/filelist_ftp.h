#ifndef FILELIST_ftp
#define FILELIST_ftp
#include "../../filelist_base.h"
#include "pftp.h"
#include <map>

class SimpleLogger : public Logger 
{
public:
    virtual void start(uint32_t bytes, const FXString & )
    {

    }
    virtual bool update(uint32_t bytes, const FXString & s)
    {
// fxmessage("UPDATE=%s %d\n",s.text(),bytes);
 return true;
    }
    virtual void end(uint32_t bytes, const FXString & )
    {

    }

    virtual void choke()
    {

    }


    virtual void error(int error)
    {

    }


    virtual void logLine(FXString & line)
    {
   //  fxmessage("LOG=%s\n",line.text());
    }
    virtual void error(FXString & error)
    {
     fxmessage("ERROR=%s\n",error.text());
    }


};


class filelist_ftp:public filelist_base
{

PFTP *pftp;
SimpleLogger *log;
string dir;


map <string,osfile> filesMap;
map <string,osfile>::iterator iter;

map <string,osfile> filesMapGlobal;
map <string,osfile>::iterator iterGlobal;

    static int level;
    int fieldsnum;
    vector < string > fields;
int attrib_nr;
  public:
    int filelist_ftp::osopendir (string dir);
    osfile filelist_ftp::osreaddir (void);
    int filelist_ftp::mkdir (string dir, int mode = 0);
    int filelist_ftp::copy (thread_elem * te);
    int filelist_ftp::move (thread_elem * te);
    int filelist_ftp::remove (thread_elem * te);
    int filelist_ftp::rename (string orgname, string newname);
    int filelist_ftp::init (vector < string > *vector_name, pathtype pt, configure * conf);
    int filelist_ftp::mode (string file);
    string filelist_ftp::owner (string file);
    string filelist_ftp::group (string file);
    bool filelist_ftp::mode (string file, unsigned int, bool recursive);
    bool filelist_ftp::owner (string file, string, bool recursive);
    bool filelist_ftp::group (string file, string, bool recursive);
    string filelist_ftp::info (void);
    void filelist_ftp::totalsize (string path, unsigned long &size);
    string filelist_ftp::symlink (string path);
    bool filelist_ftp::symlink (string src, string dst);
    bool filelist_ftp::hardlink (string src, string dst);
    int filelist_ftp::supportedfunctions (void);
    int filelist_ftp::quit (void);
    string filelist_ftp::getinitialdir(void);
    
    int filelist_ftp::priv_osopendir (string dir,string prefix,map <string,osfile> & filesMap,map <string,osfile>::iterator & iter);
    void filelist_ftp::gorecursive(string file,unsigned long &size=0);
    osfile filelist_ftp::priv_osreaddir (map <string,osfile> & filesMap,map <string,osfile>::iterator & iter2);
    void filelist_ftp::getRecursiveFiles(vector < string >src,unsigned long &size=0);
    void filelist_ftp::goLocalRecursive (string path,string prefix="");
    
};
#endif
