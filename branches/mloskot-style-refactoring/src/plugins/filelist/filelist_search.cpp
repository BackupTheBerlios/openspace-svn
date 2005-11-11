#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif

#include "filelist_search.h"

#include "../../OSVirtualFileSystemBase.h"
#include "../../OSVirtualFileSystemInfo.h"
#include "../../OSFile.h"
#include "../../OSPathType.h"
#include "../../OSConfigure.h"
#include "../../OSThreadExec.h"

#include <string>
#include <vector>

OSVirtualFileSystemInfo filelist_search::setup (void)
{
	OSVirtualFileSystemInfo v;
	
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("name"));
	v.information="search vfs - default plugin";
	v.version="1";
	v.type="search";
return v;	
	
}


int filelist_search::osopendir (std::string dir)
{

  iter=files.begin(); 

}
OSFile filelist_search::osreaddir (void)
{
    OSFile os_file;

if(iter!=files.end())
{

	    os_file.type = 0;
	    os_file.name = *iter;
	    os_file.size = 300;
	
	    iter++;
	 
	    return os_file;
}		    


    
    os_file.name = "";
    return os_file;
    


}
int filelist_search::mkdir (std::string dir, int mode)
{
}
int filelist_search::copy (OSThreadExec * te)
{

	std::string::size_type pos = te->options.find ("download");
	if (pos == std::string::npos) //upload only supported
	{
	return fil_local->copy(te);
	}
	else
	{
	 te->error=true;
         te->msg="operation not allowed";
	}

}
int filelist_search::move (OSThreadExec * te)
{

	std::string::size_type pos = te->options.find ("download");
	if (pos == std::string::npos) //upload only supported
	{
	return fil_local->move(te);
	}
	else
	{
	 te->error=true;
         te->msg="operation not allowed";
	}

}
int filelist_search::remove (OSThreadExec * te)
{
return fil_local->remove(te);
}
int filelist_search::rename (std::string orgname, std::string newname)
{

	int ret=fil_local->rename(orgname,newname);
	if(ret)
	{
		std::vector < std::string >::iterator iter = find(files.begin(), files.end(), orgname.substr(1,orgname.length()-1)); // Search the list.
		if (iter != files.end())
		{
  		(*iter)=newname.substr(1,newname.length()-1);
		}
	}

	return ret;
}
int filelist_search::init(std::vector<std::string>* vector_name, OSPathType pt, OSConfigure* conf)
{
    fil_local=new filelist_local();

    pipe = popen (pt.server.c_str(), "r");

    if (pipe != NULL)
    {
	std::string line;
	while (fgets (readbuf, sizeof (readbuf), pipe) != NULL)
	{


	    std::string path = readbuf;
	    path = path.substr (1, path.length () - 2);
	    files.push_back(path);
	   
	}

	pclose (pipe);
    }
    
iter=files.begin();
    

}
int filelist_search::mode (std::string file)
{
return fil_local->mode(file);
}
std::string filelist_search::owner (std::string file)
{
return fil_local->owner(file);
}
std::string filelist_search::group (std::string file)
{
return fil_local->group(file);
}
bool filelist_search::mode (std::string file, unsigned int mod, bool recursive)
{
return fil_local->mode(file,mod,recursive);
}
bool filelist_search::owner (std::string file, std::string ow, bool recursive)
{
return fil_local->owner(file,ow,recursive);
}
bool filelist_search::group (std::string file, std::string gr, bool recursive)
{
return fil_local->group(file,gr,recursive);
}
std::string filelist_search::info (void)
{
return "";
}
void filelist_search::totalsize (std::string path, unsigned long &size)
{
}
std::string filelist_search::symlink (std::string path)
{
}
bool filelist_search::symlink (std::string src, std::string dst)
{
}
bool filelist_search::hardlink (std::string src, std::string dst)
{
}

int filelist_search::quit (void)
{
}

EXPORTFUNCTION OSVirtualFileSystemBase *get_filelist(void)
{
    FXTRACE ((5, "PLUGIN SEARCH LOAD\n"));
    return new filelist_search ();
}
