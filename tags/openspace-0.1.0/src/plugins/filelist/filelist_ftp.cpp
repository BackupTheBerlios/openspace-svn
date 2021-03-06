#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif
#include "filelist_ftp.h"

#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

    SimpleLogger::SimpleLogger()
    {
    te=NULL;
    }
    
    void SimpleLogger::start(uint32_t bytes, const FXString & name,uint32_t size)
    {
    	if(te!=NULL && name!="")
   	{
		te->act_file_size=0;
		te->act_file_name = name.text ();
		if(size!=0)
		te->file_size = size;
	}
    }
    bool SimpleLogger::update(uint32_t bytes, const FXString & s)
    {
     if(te!=NULL)
     {
	te->act_file_size+=bytes;	
	te->act_total_size+=bytes;
	
	if(te->cancel==true)
	return false;
     }	
    return true;
    }
    void SimpleLogger::end(uint32_t bytes, const FXString & )
    {

    }

    void SimpleLogger::choke()
    {

    }

    void SimpleLogger::error(int error)
    {

    }

    void SimpleLogger::logLine(FXString & line)
    {
     fxmessage("LOG=%s\n",line.text());
    }
    void SimpleLogger::error(FXString & error)
    {
     fxmessage("ERROR=%s\n",error.text());
    }

vfs filelist_ftp::setup (void)
{
	vfs v;
	
	v.vfsheaders.push_back(vfsheader_container("name"));
	v.vfsheaders.push_back(vfsheader_container("size","size"));
	v.vfsheaders.push_back(vfsheader_container("owner"));
	v.vfsheaders.push_back(vfsheader_container("group"));
	v.information="FTP - default plugin";
	v.version="1";
	v.type="network";
return v;	
	
}


int filelist_ftp::priv_osopendir (string dir,string prefix,map <string,osfile> & filesMap,map <string,osfile>::iterator & iter)
{

filesMap.clear();


	FXString di=dir.c_str();	
	pftp->setDir(di);
	FXString di2;
	pftp->pwd(di2);

	if(FXFile::name(di)!=FXFile::name(di2) || di2=="")
	return -1;


	
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
       os_file.type=0;
       if(ptr[0]=='d')
           os_file.type = os_file.type | FOLDER; 
       else	   
           os_file.type=0;

std::stringstream parser (ptr);
std::string field;

   string user;
   string group;
   string size;
   string date;
   string attrib;
   string name;
   
parser>>attrib; 

attrib=attrib.substr(1,attrib.length()-1);

parser>>field;
parser>>user;
parser>>group;
parser>>size;
os_file.size=atoi(size.c_str());


parser>>date;
parser>>date;
parser>>date;


os_file.user=user;
os_file.group=group;
os_file.mode=attrib;



string n;
while(parser>>n)
{
if(name!="")
name=name+" "+n;
else
name=n;
}
 
        os_file.name = prefix+name;
	

if(prefix=="")
{	
	
	
    for (int i = 0; i < fieldsnum - 1; i++)
    {

	if (fields[i + 1] == "size")
	    os_file.attrib.push_back(size);
	else if (fields[i + 1] == "owner")
	   os_file.attrib.push_back(user);
	else if (fields[i + 1] == "group")
	   os_file.attrib.push_back(group);
        else if (fields[i + 1] == "mode")
        os_file.attrib.push_back(attrib);


    }
	
}	
filesMap[os_file.name]=os_file;	
	
	
	
	if(prefix!="")
	{
	filesMapGlobal[os_file.name]=os_file;
	}
        ptr = next+1;
        next = strchr(ptr, '\n');
    }
 FXFREE(&buffer);

iter=filesMap.begin();

}



int filelist_ftp::osopendir (string dir)
{


this->dir=dir;

return priv_osopendir(dir,"",filesMap,iter);
 


}

osfile filelist_ftp::priv_osreaddir (map <string,osfile> & filesMap,map <string,osfile>::iterator & iter)
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


osfile filelist_ftp::osreaddir (void)
{

  return priv_osreaddir(filesMap,iter);  
}
int filelist_ftp::mkdir (string dir, int mode)
{
	pftp->mkDir(dir.c_str());
}


static const int BUCKETSIZE = 128;
struct Bucket {
    uint8_t bucket[BUCKETSIZE];
};








void filelist_ftp::local_totalsize (string path, unsigned long &size)
{

    if (FXFile::isDirectory (path.c_str ()))
    {
	struct stat status;
	struct dirent *dp;
	DIR *dirp;


	dirp = opendir (path.c_str ());

	while ((dp = readdir (dirp)) != NULL)
	{
	    if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && (dp->d_name[1] != '.' || dp->d_name[2] != '\0')))
	    {

		string file = path;
		file.append (dp->d_name);
#ifndef WIN32
		lstat (file.c_str (), &status);
#else
		stat (file.c_str (), &status);
#endif
		if (S_ISDIR (status.st_mode))
		{
		    size = size + (unsigned int) status.st_size;
		    file.append ("/");
		    totalsize (file, size);

		}
		else
		{
		    size = size + status.st_size;
		}
	    }
	}

	closedir (dirp);
    }
    else
    {
	size += FXFile::size (path.c_str ());

    }

}







int filelist_ftp::copy (thread_elem * te)
{
 log->te=te;
  unsigned long size = 0;
    
    string::size_type pos = te->options.find ("upload");
	if (pos == string::npos)
	{
	
	string dir_copy=this->dir;
	
	 vector < string >::iterator iter;
	 
	 for (iter = te->src.begin (); iter != te->src.end(); iter++)
   	 {
	 string sr = (*iter);
	 if (FXFile::isDirectory (sr.c_str ()))
	    sr.append ("/");
	 local_totalsize (sr, size);
	 }

	te->total_size = size; 
	 
    for (iter = te->src.begin (); iter != te->src.end(); iter++)
    {
	
        FXString file=iter->c_str();	
	goLocalRecursive(file.text(),"",te);
	pftp->setDir(dir_copy.c_str());
	
    }
	this->dir=dir_copy;
	
	}
	else 
	{



getRecursiveFiles(te->src,size);

te->total_size = size; 
for (iterGlobal=--filesMapGlobal.end();; iterGlobal--)
    {   
    if(te->cancel==true)
    break;  
   	
	if(iterGlobal->second.type&FOLDER)
	{
	
	string filename=te->dst+"/"+iterGlobal->first;
	FXFile::createDirectory(filename.c_str(),str_mode_int(iterGlobal->second.mode));
	
	}
	else
	{
	string filename=te->dst+"/"+iterGlobal->first;
	FXFile::createDirectory(FXFile::directory(filename.c_str()),str_mode_int(iterGlobal->second.mode));
	te->file_size = iterGlobal->second.size;
	
	pftp->download(iterGlobal->first.c_str(),filename.c_str(),false);
	
	FXFile::mode(filename.c_str(),str_mode_int(iterGlobal->second.mode));
	}
	te->act_file_name=iterGlobal->first;
	if( iterGlobal == filesMapGlobal.begin())break;
	
    }


	

   
	}		
 log->te=NULL;   
}


void filelist_ftp::goLocalRecursive (string path,string prefix,thread_elem *te)
{

    if(te->cancel==true)
    return; 

    if (FXFile::isDirectory (path.c_str ()))
    {
	struct stat status;
	struct dirent *dp;
	DIR *dirp;

     prefix=prefix+ FXFile::name(path.c_str()).text();
     string fulldir=this->dir+"/"+prefix;
     
     pftp->mkDir(fulldir.c_str());
     this->mode (fulldir,FXFile::mode(path.c_str()),false);
     pftp->setDir(fulldir.c_str()); 

	dirp = opendir (path.c_str ());

	while ((dp = readdir (dirp)) != NULL)
	{
	    if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && (dp->d_name[1] != '.' || dp->d_name[2] != '\0')))
	    {

		string file = path+"/";
		file.append (dp->d_name);
		goLocalRecursive(file,prefix+"/",te);
	    }
	}

	closedir (dirp);
    }
    else
    {
    string pre=this->dir+"/"+prefix.substr(0,prefix.length()-1);
    if(this->dir!=pre)
    {
     pftp->setDir(pre.c_str()); 
     this->dir=pre;
    }
  
		FXString fil=path.c_str();
		pftp->upload(fil,0,false);
		this->mode (path,FXFile::mode(fil),false);   
		 

    }

}








int filelist_ftp::move (thread_elem * te)
{
}


void filelist_ftp::gorecursive(string file,unsigned long &size)
{


		map <string,osfile> filesMap;
		map <string,osfile>::iterator iter;
		
	   	if(priv_osopendir(file,file+"/",filesMap,iter)!=-1)
		{
		
		 while (1)
   		 {	
		 osfile os_file = priv_osreaddir (filesMap,iter);
		 	if (os_file.name == "")
	   		break;
			
			if(os_file.type&FOLDER)
			{
			gorecursive(os_file.name,size);
			}
			else
			size = size + os_file.size;
		 }

	    }


}

void filelist_ftp::getRecursiveFiles(vector < string >src,unsigned long &size)
{

filesMapGlobal.clear();


    vector < string >::iterator iter_files;

    for (iter_files = src.begin (); iter_files != src.end(); iter_files++)
    {     

    string name=FXFile::name(iter_files->c_str()).text();
    	osfile os_file;
	os_file.name=name;
	os_file.type=filesMap[name].type;
	os_file.size=filesMap[name].size;
	os_file.mode=filesMap[name].mode;
	os_file.group=filesMap[name].user;
	os_file.user=filesMap[name].group;
	filesMapGlobal[name]=os_file;

    }


   for (iterGlobal=filesMapGlobal.begin();iterGlobal!=filesMapGlobal.end(); iterGlobal++)
   {

   
        if(iterGlobal->second.type&FOLDER)
	{
 	gorecursive(iterGlobal->first,size);
	FXString di=this->dir.c_str();
	pftp->setDir(di);
	}
	else
	size = size + iterGlobal->second.size;

   }
}

int filelist_ftp::remove (thread_elem * te)
{
unsigned long size;
getRecursiveFiles(te->src,size);

for (iterGlobal=--filesMapGlobal.end();; iterGlobal--)
    {     
  
	
	if(iterGlobal->second.type&FOLDER)
	pftp->rmDir(iterGlobal->first.c_str());
	else
	pftp->del(iterGlobal->first.c_str());
	
	te->act_file_name=iterGlobal->first;
	if( iterGlobal == filesMapGlobal.begin())break;
	
    }



}
int filelist_ftp::rename (string orgname, string newname)
{

FXString tmp;
pftp->sendCmd("RNFR ", FXFile::name(orgname.c_str()), tmp);
pftp->sendCmd("RNTO ", FXFile::name(newname.c_str()), tmp);

}
int filelist_ftp::init (vector < string > *vector_name, pathtype pt, configure * conf)
{

    fieldsnum = vector_name->size ();
    fields = (*vector_name);

log=new SimpleLogger;
pftp=new PFTP(pt.server.c_str(),pt.user.c_str(),pt.password.c_str(),"",log);

	if(pftp->isConnected() && getinitialdir()!="")
        {	

	}
	else return -1;
}

int filelist_ftp::str_mode_int(string per)
{
unsigned int mode = 0;
	if (per[0]=='r')
	    mode = mode | S_IRUSR;
	if (per[1]=='w')
	    mode = mode | S_IWUSR;
	if (per[2]=='x')
	    mode = mode | S_IXUSR;
	if (per[3]=='r')
	    mode = mode | S_IRGRP;
	if (per[4]=='w')
	    mode = mode | S_IWGRP;
	if (per[5]=='x')
	    mode = mode | S_IXGRP;
	if (per[6]=='r')
	    mode = mode | S_IROTH;
	if (per[7]=='w')
	    mode = mode | S_IWOTH;
	if (per[8]=='x')
	    mode = mode | S_IXOTH;
return mode;
}

int filelist_ftp::mode (string file)
{

string per=filesMap[FXFile::name(file.c_str()).text()].mode;

return str_mode_int(per);

}
string filelist_ftp::owner (string file)
{
return filesMap[FXFile::name(file.c_str()).text()].user; 
}
string filelist_ftp::group (string file)
{
return filesMap[FXFile::name(file.c_str()).text()].group;
}
bool filelist_ftp::mode (string file, unsigned int per, bool recursive)
{

unsigned int mode=per;

int x=((mode & S_IRUSR) | (mode & S_IWUSR) | (mode & S_IXUSR))/64;
int y=((mode & S_IRGRP) | (mode & S_IWGRP) | (mode & S_IXGRP))/8;
int z=((mode & S_IROTH) | (mode & S_IWOTH) | (mode & S_IXOTH));	

file=FXFile::name(file.c_str()).text();

FXString tmp;
char chstr[20];
sprintf (chstr, "%d%d%d", x,y,z);

	if(recursive==false)
	{
	string cmd="CHMOD "+string(chstr)+" "+ file;
	pftp->sendCmd("SITE ",cmd.c_str(),tmp );
	}
	else
	{
	unsigned long size;
	vector < string >src;
	src.push_back(file);
	getRecursiveFiles(src,size);

		for (iterGlobal=--filesMapGlobal.end();; iterGlobal--)
  		  {     
   			
			
			string cmd="CHMOD "+string(chstr)+" "+ iterGlobal->first;
			pftp->sendCmd("SITE ",cmd.c_str(),tmp );
			
			
			if( iterGlobal == filesMapGlobal.begin())break;
	
   		  }

	
	}



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
size=0;
	
	string file=FXFile::name(path.c_str()).text();
	if(file=="")
	file=FXFile::name(path.substr(0,path.length()-1).c_str()).text();
	
	

	if(filesMap[file].type&FOLDER)
	{
	vector < string >src;
	src.push_back(file);
	getRecursiveFiles(src,size);
	}
	else
	size=filesMap[file].size;


	
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

int filelist_ftp::quit (void)
{
pftp->logout();
}

string filelist_ftp::getinitialdir(void)
{
	FXString base;
	pftp->pwd(base);

return base.text();
}



EXPORTFUNCTION filelist_base *get_filelist (void)
{
    FXTRACE ((5, "PLUGIN LOAD\n"));
    return new filelist_ftp ();
}
