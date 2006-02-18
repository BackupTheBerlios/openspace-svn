#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif

#include "OSVfsSearch.h"

#include "../../OSVirtualFileSystemBase.h"
#include "../../OSVirtualFileSystemInfo.h"
#include "../../OSFile.h"
#include "../../OSPathType.h"
#include "../../OSConfigure.h"
#include "../../OSThreadExec.h"

#include <string>
#include <vector>

OSVirtualFileSystemInfo OSVfsSearch::setup (void)
{
	OSVirtualFileSystemInfo v;
	
	v.vfsheaders.push_back(OSVirtualFileSystemHeader("name"));
	v.information="search vfs - default plugin";
	v.version="1";
	v.type="search";
return v;	
	
}


int OSVfsSearch::osopendir (std::string dir)
{

  iter=files.begin(); 

}
OSFile OSVfsSearch::osreaddir (void)
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
int OSVfsSearch::mkdir (std::string dir, int mode)
{
}
int OSVfsSearch::copy (OSThreadExec * te)
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
int OSVfsSearch::move (OSThreadExec * te)
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
int OSVfsSearch::remove (OSThreadExec * te)
{
return fil_local->remove(te);
}
int OSVfsSearch::rename (std::string orgname, std::string newname)
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
int OSVfsSearch::init(long id, std::vector<std::string>* vector_name, OSPathType pt, OSConfigure* conf)
{
    fil_local=new OSVfsLocal();
    this->id=id;
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
int OSVfsSearch::mode (std::string file)
{
return fil_local->mode(file);
}
std::string OSVfsSearch::owner (std::string file)
{
return fil_local->owner(file);
}
std::string OSVfsSearch::group (std::string file)
{
return fil_local->group(file);
}
bool OSVfsSearch::mode (std::string file, unsigned int mod, bool recursive)
{
return fil_local->mode(file,mod,recursive);
}
bool OSVfsSearch::owner (std::string file, std::string ow, bool recursive)
{
return fil_local->owner(file,ow,recursive);
}
bool OSVfsSearch::group (std::string file, std::string gr, bool recursive)
{
return fil_local->group(file,gr,recursive);
}
std::string OSVfsSearch::info (void)
{
return "";
}
void OSVfsSearch::totalsize (std::string path, unsigned long &size)
{
}
std::string OSVfsSearch::symlink (std::string path)
{
}
bool OSVfsSearch::symlink (std::string src, std::string dst)
{
}
bool OSVfsSearch::hardlink (std::string src, std::string dst)
{
}

int OSVfsSearch::quit (void)
{
}

EXPORTFUNCTION OSVirtualFileSystemBase *get_filelist(void)
{
    FXTRACE ((5, "PLUGIN SEARCH LOAD\n"));
    return new OSVfsSearch ();
}
