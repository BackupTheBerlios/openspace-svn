#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif
#include "filelist_ftp.h"





int filelist_ftp::level=0;




int filelist_ftp::osopendir (string dir)
{

	
	FXString di=dir.c_str();
	fxmessage("DIR=%s\n",dir.c_str());
	pftp->setDir(di);
	FXString di2;
	pftp->pwd(di2);

	if(di!=di2)
	return -1;

this->dir=dir;


filesMap.clear();

	
    FXMemoryStream str;
    str.open(FXStreamSave, NULL);


    pftp->list(str);

    // nil terminate the buffer
    FXuchar nil = 0;
    str.save(&nil, 1);

    // and get it
    FXuchar* buffer;
    unsigned long sp;
    str.takeBuffer(buffer, sp);

    char * ptr = reinterpret_cast<char *>(buffer);
    char * next = strchr(ptr, '\n');
    char * start = ptr;


    //get attributes # user group size date =(3 or 4? fields) name
    while(next && (ptr-start < (int)sp))
    {

        next[0] = 0;
        if(next[-1] == '\r')
            next[-1] = 0;

        // Some FTP servers send other lines than raw directory
        if(ptr[0] != 'd' && ptr[0] != '-')
        {
            ptr = next+1;
            next = strchr(ptr, '\n');
            continue;
        }

       osfile os_file;
       if(ptr[0]=='d')
           os_file.type = os_file.type | FOLDER;
       else	   
           os_file.type=0;

       
       // item.attributes = ptr;

        char * cursor = strchr(ptr, ' ');
        *cursor = 0;
        ++cursor;
        while(isspace(*cursor))
            ++cursor;
        // skip the group number
        while(!isspace(*cursor))
            ++cursor;
        // go for the user
        while(isspace(*cursor))
            ++cursor;
       // item.user = cursor;
        // crawl over it and terminate
        while(!isspace(*cursor))
            ++cursor;
        *cursor = 0;
        ++cursor;
        // go for the group
        while(isspace(*cursor))
            ++cursor;
       // item.group = cursor;
        // crawl over it and terminate
        while(!isspace(*cursor))
            ++cursor;
        *cursor = 0;
        ++cursor;
        // go for the size
        while(isspace(*cursor))
            ++cursor;
       // item.size = cursor;
        // crawl over it and terminate
        while(!isspace(*cursor))
            ++cursor;
        *cursor = 0;
        ++cursor;
        // go for the date
        while(isspace(*cursor))
            ++cursor;
       // item.date = cursor;
        while(!isspace(*cursor))
            ++cursor; // over month
        while(isspace(*cursor))
            ++cursor; // gap
        while(!isspace(*cursor))
            ++cursor; // day
        while(isspace(*cursor))
            ++cursor; // gap
        while(!isspace(*cursor))
            ++cursor; // time or year
        *cursor = 0;
        ++cursor;
        // go for the name
        while(isspace(*cursor))
            ++cursor;
        os_file.name = cursor;
	
		
      //  files.push_back(os_file);
	filesMap[os_file.name]=os_file;

        ptr = next+1;
        next = strchr(ptr, '\n');
    }

   iter=filesMap.begin();

    FXFREE(&buffer);
	



}
osfile filelist_ftp::osreaddir (void)
{
osfile os_file;

if(iter!=filesMap.end())
{
	  os_file=iter->second;
	   
	    iter++;
	    return os_file;
}		    


    
    os_file.name = "";
    return os_file;
    
}
int filelist_ftp::mkdir (string dir, int mode)
{
	pftp->mkDir(dir.c_str());
}
int filelist_ftp::copy (thread_elem * te)
{
}
int filelist_ftp::move (thread_elem * te)
{
}





void filelist_ftp::gorecursive(string file,string operation)
{

fxmessage("\nLEVEL=%d\n",level);

level++;
	   string sr = file;	
	   string onlyname=FXFile::name(sr.c_str()).text();
	   
	    if(filesMap[onlyname].type&FOLDER)
	    {
		map <string,osfile> filesMap_copy=filesMap;
		map <string,osfile>::iterator iter_copy=this->iter;
		

	   	if(osopendir(sr)!=-1)
		 while (1)
   		 {	
		 osfile os_file = osreaddir ();
		 	if (os_file.name == "")
	   		break;
			
			string dirfile=sr+"/"+os_file.name;			
			gorecursive(dirfile,operation);
			
		 }
	    	
	     filesMap=filesMap_copy; 
	     this->iter=iter_copy;
	     
	     fxmessage("KASUJE KATALOG=%s\n",sr.c_str());
	     pftp->rmDir(sr.c_str());
	     
	    }
	    else
	    {
	    fxmessage("KASUJE PLIK=%s\n",sr.c_str());
	    pftp->del(sr.c_str());
	    }	
level--;

}



int filelist_ftp::remove (thread_elem * te)
{

	    
bool canc = false;

    vector < string >::iterator iter;

    for (iter = te->src.begin (); iter != te->src.end(); iter++)
    {
    fxmessage("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n\n\n");
	gorecursive(*iter,"remove");

    }


}
int filelist_ftp::rename (string orgname, string newname)
{
}
int filelist_ftp::init (vector < string > *vector_name, pathtype pt, configure * conf)
{
log=new SimpleLogger;
pftp=new PFTP(pt.server.c_str(),pt.user.c_str(),pt.password.c_str(),"",log);

	if(pftp->isConnected())
        {	
	fxmessage("CONNECTED :d :d :d");

	}
}
int filelist_ftp::mode (string file)
{
}
string filelist_ftp::owner (string file)
{
}
string filelist_ftp::group (string file)
{
return "";
}
bool filelist_ftp::mode (string file, unsigned int, bool recursive)
{
}
bool filelist_ftp::owner (string file, string, bool recursive)
{
}
bool filelist_ftp::group (string file, string, bool recursive)
{
}
string filelist_ftp::info (void)
{
return "";
}
void filelist_ftp::totalsize (string path, unsigned long &size)
{
}
string filelist_ftp::symlink (string path)
{
return "";
}
bool filelist_ftp::symlink (string src, string dst)
{
}
bool filelist_ftp::hardlink (string src, string dst)
{
}
int filelist_ftp::supportedfunctions (void)
{
}
int filelist_ftp::quit (void)
{
}

string filelist_ftp::getinitialdir(void)
{
	FXString base;
	pftp->pwd(base);
fxmessage("INITIAL DIR=%s\n",base.text());	
return base.text();
}



EXPORTFUNCTION filelist_base *get_filelist (void)
{
    FXTRACE ((5, "PLUGIN LOAD\n"));
    return new filelist_ftp ();
}
