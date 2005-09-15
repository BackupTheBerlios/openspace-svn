#ifndef FILELIST_ftp
#define FILELIST_ftp
#include "../../filelist_base.h"
#include "pftp.h"
#include <map>



class SimpleLogger : public Logger 
{
public:
    thread_elem *te;
    
    SimpleLogger::SimpleLogger();
    void SimpleLogger::start(uint32_t bytes, const FXString & name,uint32_t size);
    bool SimpleLogger::update(uint32_t bytes, const FXString & s);
    void SimpleLogger::end(uint32_t bytes, const FXString & );
    void SimpleLogger::choke();
    void SimpleLogger::error(int error);
    void SimpleLogger::logLine(FXString & line);
    void SimpleLogger::error(FXString & error);

};


class filelist_ftp:public filelist_base
{

private:

PFTP *pftp;
SimpleLogger *log;
string dir;


map <string,osfile> filesMap;
map <string,osfile>::iterator iter;

map <string,osfile> filesMapGlobal;
map <string,osfile>::iterator iterGlobal;

    int fieldsnum;
    vector < string > fields;

    int filelist_ftp::priv_osopendir (string dir,string prefix,map <string,osfile> & filesMap,map <string,osfile>::iterator & iter);
    void filelist_ftp::gorecursive(string file,unsigned long &size=0);
    osfile filelist_ftp::priv_osreaddir (map <string,osfile> & filesMap,map <string,osfile>::iterator & iter2);
    void filelist_ftp::getRecursiveFiles(vector < string >src,unsigned long &size=0);
    void filelist_ftp::goLocalRecursive (string path,string prefix,thread_elem *te);
    void filelist_ftp::local_totalsize (string path, unsigned long &size);
    int filelist_ftp::str_mode_int(string per);

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
    vfs filelist_ftp::setup (void);
    int filelist_ftp::quit (void);
    string filelist_ftp::getinitialdir(void);
    
    
};
#endif
