#ifndef FILELIST_LOCAL
#define FILELIST_LOCAL

#include"../../filelist_base.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>




class filelist_local : public filelist_base
{

DIR *dp;
dirent *dirrec;
struct stat status;
string dir;
string display_size;
int fieldsnum;
vector<string>fields;


public:
	
	int osopendir(string dir);
	osfile osreaddir(void);
	int mkdir(string dir, int mode);
	int copy(thread_elem* te);
	int move(thread_elem* te);
	int remove(thread_elem* te);
	int rename(string orgname,string newname);
	int init(vector<string>*vector_name,vector<int>*vector_type,vector<int>*vector_width,pathtype pt,configure *conf);
	unsigned int filelist_local::mode(string file);
	bool filelist_local::mode(string file,unsigned int,bool recursive=false);
	string filelist_local::info(void);
	int quit(void);
	
	
	void totalsize(string path,unsigned long &  size);
	int copymove(thread_elem* te,bool copy=true);
};


#endif
