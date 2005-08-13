#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif
#include "filelist_archive.h"
int filelist_archive::osopendir (string dir)
{
fxmessage("OPENDIR");
this->dir=dir;
iter=files.begin();
}
osfile filelist_archive::osreaddir (void)
{

osfile os_file;

while(iter!=files.end())
{
os_file.name=*iter;
	string modfile=os_file.name.substr(0,os_file.name.length () - 1);
	fxmessage("\nMOD=%s",modfile.c_str());
	string::size_type pos=modfile.rfind("/");
	if (pos != string::npos)
	{
		string dirf="/"+os_file.name.substr(0,pos);
		
		fxmessage("\nDIR=%s  DIRF=%s",dir.c_str(),dirf.c_str()); 
		if(dirf!=dir)
		{
		iter++;
		continue;
		}
		else
		os_file.name=os_file.name.substr(pos+1);
		
		
		
	}
	else if(dir!="/")
	{
	iter++;
	continue;
	}

	    os_file.type = 0;
	    os_file.size = 300;
	    os_file.attrib = new string[2];
	    os_file.attrib[0] = "300";
	    os_file.attrib[1] = "";
	    
	    string s = os_file.name.substr (os_file.name.length () - 1, 1);

	    if (s == "/")
	    {
	        os_file.name = os_file.name.substr (0,os_file.name.length () - 1);
		os_file.type = FOLDER;
	    }
	    else
		os_file.type = 0;
	    
	    iter++;
	    //fxmessage("\nN=%s",os_file.name.c_str());  
	    return os_file;
}		    


    
    os_file.name = "";
    return os_file;

}
int filelist_archive::mkdir (string dir, int mode)
{
}
int filelist_archive::copy (thread_elem * te)
{
fxmessage("ARCHIVE COPY");
	string::size_type pos = te->options.find ("upload");
	if (pos != string::npos)
	{
		 vector < string >::iterator iter;

   		 for (iter = te->src.begin (); iter != te->src.end(); iter++)
   		 {
			string sr = (*iter);
			sr=sr.substr(1,sr.length()-1);
			string command="cd " +te->dst +" && tar xf" + type + " " + archive_filename + " " + sr;
			system(command.c_str());
			fxmessage("COMMAND=%s\n",command.c_str());
   		 }

	
	}
	
te->end=true;
}
int filelist_archive::move (thread_elem * te)
{
}
int filelist_archive::remove (thread_elem * te)
{
}
int filelist_archive::rename (string orgname, string newname)
{
}
int filelist_archive::init (vector < string > *vector_name, pathtype pt, configure * conf)
{
archive_filename = pt.server;


string  command;

if (archive_filename.length () >= 6 && archive_filename.substr (archive_filename.length () - 6, 6) == "tar.gz")
type="z";
else
type="j";

command="tar tf" + type + " " + archive_filename;
fxmessage("COMMAND=%s\n",command.c_str());

pipe = popen(command.c_str(), "r");

if (pipe != NULL)
    {
	string line;
	while (fgets (readbuf, sizeof (readbuf), pipe) != NULL)
	{


	    string path = readbuf;
	    path = path.substr (0, path.length () - 1);
	    files.push_back(path);
	   
	}

	pclose (pipe);
    }
    
iter=files.begin();

}
int filelist_archive::mode (string file)
{
}
string filelist_archive::owner (string file)
{
return "";
}
string filelist_archive::group (string file)
{
return "";
}
bool filelist_archive::mode (string file, unsigned int, bool recursive)
{
}
bool filelist_archive::owner (string file, string, bool recursive)
{
}
bool filelist_archive::group (string file, string, bool recursive)
{
}
string filelist_archive::info (void)
{
return "";
}
void filelist_archive::totalsize (string path, unsigned long &size)
{
}
string filelist_archive::symlink (string path)
{
return "";
}
bool filelist_archive::symlink (string src, string dst)
{
}
bool filelist_archive::hardlink (string src, string dst)
{
}
int filelist_archive::supportedfunctions (void)
{
}
int filelist_archive::quit (void)
{
}
EXPORTFUNCTION filelist_base *get_filelist (void)
{
    FXTRACE ((5, "PLUGIN LOAD\n"));
    return new filelist_archive ();
}
