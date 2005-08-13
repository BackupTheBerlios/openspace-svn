#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif
#include "filelist_search.h"


int filelist_search::osopendir (string dir)
{

  iter=files.begin(); 

}
osfile filelist_search::osreaddir (void)
{
    osfile os_file;

if(iter!=files.end())
{

	    os_file.type = 0;
	    os_file.name = *iter;
	    os_file.size = 300;
	    os_file.attrib = new string[2];
	    os_file.attrib[0] = "300";
	    os_file.attrib[1] = "";
	    
	    iter++;
	    fxmessage("\nN=%s",os_file.name.c_str());  
	    return os_file;
}		    


    
    os_file.name = "";
    return os_file;
    


}
int filelist_search::mkdir (string dir, int mode)
{
}
int filelist_search::copy (thread_elem * te)
{
fxmessage("COPY\n"); 
	string::size_type pos = te->options.find ("download");
	if (pos == string::npos) //upload only supported
	{
	return fil_local->copy(te);
	}
	else
	{
	 te->error=true;
         te->msg="operation not allowed";
	}

}
int filelist_search::move (thread_elem * te)
{
fxmessage("COPY\n"); 
	string::size_type pos = te->options.find ("download");
	if (pos == string::npos) //upload only supported
	{
	return fil_local->move(te);
	}
	else
	{
	 te->error=true;
         te->msg="operation not allowed";
	}

}
int filelist_search::remove (thread_elem * te)
{
return fil_local->remove(te);
}
int filelist_search::rename (string orgname, string newname)
{
fxmessage("o=%s n=%s",orgname.c_str(),newname.c_str());
	int ret=fil_local->rename(orgname,newname);
	if(ret)
	{
		vector < string >::iterator iter = find(files.begin(), files.end(), orgname.substr(1,orgname.length()-1)); // Search the list.
		if (iter != files.end())
		{
  		(*iter)=newname.substr(1,newname.length()-1);
		}
	}

	return ret;
}
int filelist_search::init (vector < string > *vector_name, pathtype pt, configure * conf)
{

fxmessage("INIT");
fil_local=new filelist_local();


    pipe = popen (pt.server.c_str(), "r");
 fxmessage(pt.server.c_str());  
    if (pipe != NULL)
    {
	string line;
	while (fgets (readbuf, sizeof (readbuf), pipe) != NULL)
	{


	    string path = readbuf;
	    path = path.substr (1, path.length () - 2);
	    files.push_back(path);
	   
	}

	pclose (pipe);
    }
    
iter=files.begin();
    

}
int filelist_search::mode (string file)
{
return fil_local->mode(file);
}
string filelist_search::owner (string file)
{
return fil_local->owner(file);
}
string filelist_search::group (string file)
{
return fil_local->group(file);
}
bool filelist_search::mode (string file, unsigned int mod, bool recursive)
{
return fil_local->mode(file,mod,recursive);
}
bool filelist_search::owner (string file, string ow, bool recursive)
{
return fil_local->owner(file,ow,recursive);
}
bool filelist_search::group (string file, string gr, bool recursive)
{
return fil_local->group(file,gr,recursive);
}
string filelist_search::info (void)
{
return "";
}
void filelist_search::totalsize (string path, unsigned long &size)
{
}
string filelist_search::symlink (string path)
{
}
bool filelist_search::symlink (string src, string dst)
{
}
bool filelist_search::hardlink (string src, string dst)
{
}
int filelist_search::supportedfunctions (void)
{
}
int filelist_search::quit (void)
{
}
EXPORTFUNCTION filelist_base *get_filelist (void)
{
    FXTRACE ((5, "PLUGIN SEARCH LOAD\n"));
    return new filelist_search ();
}
