#ifndef FILELIST_TAR
#define FILELIST_TAR

#include"../../filelist_base.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>




class filelist_tar : public filelist_base
{


string dir;
string server;
int fieldsnum;
string *fields;
string display_size;
FILE *pipe;
char readbuf[80];
public:
	
	int osopendir(string dir);
	osfile osreaddir(void);
	int mkdir(string dir, int mode);
	int copy(thread_elem* te);
	int move(thread_elem* te);
	int remove(thread_elem* te);
	int rename(string orgname,string newname);
	int init(string **name,unsigned int **type,unsigned int **width,pathtype pt,configure *conf);
	unsigned int mode(string file);
	bool mode(string file,unsigned int,bool recursive=false);
	string info(void);
	int quit(void);
	
	
	void totalsize(string path,unsigned long &  size);
	int copymove(thread_elem* te,bool copy=true);
};


#endif
