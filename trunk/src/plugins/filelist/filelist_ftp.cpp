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



int filelist_ftp::level=0;




int filelist_ftp::priv_osopendir (string dir,string prefix,map <string,osfile> & filesMap,map <string,osfile>::iterator & iter)
{

filesMap.clear();

fxmessage("\nSWITCH TO=%s ",dir.c_str());
	FXString di=dir.c_str();	
	pftp->setDir(di);
	FXString di2;
	pftp->pwd(di2);

	if(FXFile::name(di)!=FXFile::name(di2))
	return -1;

fxmessage("\nDIR=%s\n",dir.c_str());

	
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
parser>>date;
parser>>date;
parser>>date;

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
	{
	attrib_nr=i;
        os_file.attrib.push_back(attrib);

	}

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

int filelist_ftp::copy (thread_elem * te)
{

 vector < string >::iterator iter;
    for (iter = te->src.begin (); iter != te->src.end(); iter++)
    {
    
    
    string::size_type pos = te->options.find ("upload");
	if (pos == string::npos)
	{
        FXString file=iter->c_str();
	pftp->upload(file,0,false);
	}
	else 
	{
	
	    string ds = te->dst;
	    ds.append ("/");
	    ds.append (FXFile::name (iter->c_str ()).text ());

fxmessage("\nDOWNLOAD %s TO %s",iter->c_str(),ds.c_str());

    FXMemoryStream str;
    str.open(FXStreamSave, NULL);

    pftp->download(iter->c_str(), str,false );

    // now write it to a file
    FXString name = ds.c_str();
    FXuchar* buffer;
    unsigned long sp;
    str.takeBuffer(buffer, sp);

    FXFileStream out;
    out.open(name, FXStreamSave);

    if(out.direction() != FXStreamSave)
        return -1;

    out.save(buffer, sp);
    out.close();
	}
		
    }



}
int filelist_ftp::move (thread_elem * te)
{
}


void filelist_ftp::gorecursive(string file)
{

fxmessage("\nFILE=%s",file.c_str());
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
			gorecursive(os_file.name);
			}
		 }

	    }


}

void filelist_ftp::getRecursiveFiles(thread_elem *te)
{

filesMapGlobal.clear();


    vector < string >::iterator iter_files;

    for (iter_files = te->src.begin (); iter_files != te->src.end(); iter_files++)
    {     
    
    string name=FXFile::name(iter_files->c_str()).text();
    	osfile os_file;
	os_file.name=name;
	os_file.type=filesMap[name].type;
	filesMapGlobal[name]=os_file;

    }


   for (iterGlobal=filesMapGlobal.begin();iterGlobal!=filesMapGlobal.end(); iterGlobal++)
   {

   
        if(iterGlobal->second.type&FOLDER)
	{
 	gorecursive(iterGlobal->first);
	FXString di=this->dir.c_str();
	pftp->setDir(di);
	}

   }
}

int filelist_ftp::remove (thread_elem * te)
{
getRecursiveFiles(te);

    

for (iterGlobal=--filesMapGlobal.end();; iterGlobal--)
    {     
   	fxmessage("\nEL=%s",iterGlobal->first.c_str());
	
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

	if(pftp->isConnected())
        {	
	fxmessage("CONNECTED :d :d :d");

	}
}
int filelist_ftp::mode (string file)
{

string per=filesMap[FXFile::name(file.c_str()).text()].attrib[attrib_nr];

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
string filelist_ftp::owner (string file)
{
return ""; 
}
string filelist_ftp::group (string file)
{
return "";
}
bool filelist_ftp::mode (string file, unsigned int per, bool recursive)
{

unsigned int mode=per;

int x=((mode & S_IRUSR) | (mode & S_IWUSR) | (mode & S_IXUSR))/64;
int y=((mode & S_IRGRP) | (mode & S_IWGRP) | (mode & S_IXGRP))/8;
int z=((mode & S_IROTH) | (mode & S_IWOTH) | (mode & S_IXOTH));	



FXString tmp;

char chstr[20];
sprintf (chstr, "%d%d%d", x,y,z);
fxmessage("PER=%d",chstr);

string cmd="CHMOD "+string(chstr)+" "+ file;
pftp->sendCmd("SITE ",cmd.c_str(),tmp );
fxmessage("\nCOMMAND=%s\n",cmd.c_str());

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
pftp->logout();
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
