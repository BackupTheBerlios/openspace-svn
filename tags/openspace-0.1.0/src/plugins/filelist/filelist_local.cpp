
#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif

#include <fx.h>
#include <errno.h>
#include "filelist_local.h"
#include "../../sharedFunctions.h"
#include "OSFXFile.h"

#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif

#include <fstream>
#include <sstream>
#include <sys/mount.h>

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>


vector < string > name;


vfs filelist_local::setup (void)
{
	vfs v;
	
	v.vfsheaders.push_back(vfsheader_container("name"));
	v.vfsheaders.push_back(vfsheader_container("size","size"));
	v.vfsheaders.push_back(vfsheader_container("owner"));
	v.vfsheaders.push_back(vfsheader_container("group"));
	v.vfsheaders.push_back(vfsheader_container("accessed","date"));
	v.vfsheaders.push_back(vfsheader_container("modified","date"));
	v.information="Local file list - default plugin";
	v.version="1";
	
return v;	
	
}

int filelist_local::init (vector < string > *vector_name,pathtype pt, configure * conf)
{

    fieldsnum = vector_name->size ();
    fields = (*vector_name);

    return 0;
}


osfile filelist_local::osreaddir ()
{

    osfile os_file;
    if (dp == NULL)
    {
	os_file.name = "";
	return os_file;
    }
    if ((dirrec = readdir (dp)) == NULL)
    {
	closedir (dp);
	os_file.name = "";
	return os_file;
    }

    os_file.name = dirrec->d_name;
    os_file.type = 0;

    os_file.size = FXFile::size (os_file.name.c_str ());

    if (stat (dirrec->d_name, &status) != -1)
    {

	struct stat status2;
	if (lstat (dirrec->d_name, &status2) != -1)
	{
	    if (S_ISLNK (status2.st_mode))
	    {
		os_file.type = os_file.type | SYMLINK;
	    }

	}


	if (S_ISDIR (status.st_mode))
	{
	    os_file.type = os_file.type | FOLDER;
	}
	else
	{
	    /*
	       char line[20];
	       FILE *fr;    
	       fr = fopen (dirrec->d_name, "rt"); 
	       fgets(line, sizeof(line), fr);   
	       fclose(fr);  
	       string con=line;

	       os_file.content=con.substr(0,con.size()-1);
	     */
	}




	if (S_ISCHR (status.st_mode))
	    os_file.type = os_file.type | CHARDEV;

	if (S_ISBLK (status.st_mode))
	    os_file.type = os_file.type | BLOCKDEV;

	if (access (dirrec->d_name, X_OK) == 0)
	    os_file.type = os_file.type | EXECUTABLE;
    }





    for (int i = 0; i < fieldsnum - 1; i++)
    {

	if (fields[i + 1] == "size")
	    os_file.attrib.push_back(numtostring (os_file.size));
	else if (fields[i + 1] == "owner")
	   os_file.attrib.push_back(FXFile::owner (os_file.name.c_str ()).text ());
	else if (fields[i + 1] == "group")
	   os_file.attrib.push_back(FXFile::group (os_file.name.c_str ()).text ());
	else if (fields[i + 1] == "accessed")
	    os_file.attrib.push_back(FXFile::time ("%H:%M %d/%m/%y", FXFile::accessed (os_file.name.c_str ())).text ());
	else if (fields[i + 1] == "created")
	    os_file.attrib.push_back(FXFile::time ("%H:%M %d/%m/%y", FXFile::created (os_file.name.c_str ())).text ()); 
	else if (fields[i + 1] == "modified")
 	    os_file.attrib.push_back(FXFile::time ("%H:%M %d/%m/%y", FXFile::modified (os_file.name.c_str ())).text ());       
	else if (fields[i + 1] == "mode")
	{

	    struct stat info;
	    string str = "---------";
	    if (lstat (os_file.name.c_str (), &info) == 0)
	    {

		(info.st_mode & S_IRUSR) ? str[0] = 'r' : str[0] = '-';
		(info.st_mode & S_IWUSR) ? str[1] = 'w' : str[1] = '-';
		(info.st_mode & S_IXUSR) ? str[2] = 'x' : str[2] = '-';
		(info.st_mode & S_IRGRP) ? str[3] = 'r' : str[3] = '-';
		(info.st_mode & S_IWGRP) ? str[4] = 'w' : str[4] = '-';
		(info.st_mode & S_IXGRP) ? str[5] = 'x' : str[5] = '-';
		(info.st_mode & S_IROTH) ? str[6] = 'r' : str[6] = '-';
		(info.st_mode & S_IWOTH) ? str[7] = 'w' : str[7] = '-';
		(info.st_mode & S_IXOTH) ? str[8] = 'x' : str[8] = '-';

	    }


	    os_file.attrib.push_back(str);

	}

    }



    return os_file;


}


int filelist_local::osopendir (string dir)
{
    this->dir = dir;

#ifndef WIN32
    std::string line;
    std::ifstream infile ("/etc/fstab");

    while (std::getline (infile, line))
    {

	std::stringstream parser (line);
	std::string command;
	std::string field;
	string partition;
	parser >> partition;

	string type;

	if (partition[0] != '#')
	{

	    parser >> command;

	    if (command == dir)
	    {

		parser >> type;
		parser >> field;
		string::size_type pos = field.find ("noauto", 0);
		if (pos != string::npos)
		{

		    if (mount (partition.c_str (), command.c_str (), type.c_str (), MS_RDONLY, "") == 0)
			FXTRACE ((5, "MOUNT\n"));

		}
	    }

	}
    }

#endif

    dp = opendir (dir.c_str ());
    if (dp == NULL)
	return -1;

    chdir (dir.c_str ());


}

int filelist_local::mkdir (string dir, int mode)
{
    FXTRACE ((5, "MKDIR\n"));
    string d = this->dir + SEPARATOR + dir;

    return FXFile::createDirectory (d.c_str (), mode);
    return 0;
}

int filelist_local::copy (thread_elem * te)
{
    copymove (te, true);
}

int filelist_local::move (thread_elem * te)
{
    copymove (te, false);
}


int filelist_local::remove (thread_elem * te)
{

    bool canc = false;

    vector < string >::iterator iter;

    for (iter = te->src.begin (); iter != te->src.end(); iter++)
    {
  
	if (!canc)
	{

	    string sr = (*iter);
	    if (FXFile::isDirectory (sr.c_str ()))
		sr.append (SEPARATOR);
	    FXTRACE ((5, "REMOVE: %s", sr.c_str ()));

	    if (OSFXFile::remove (sr.c_str (), te) == FALSE)
		canc = true;
		
	}


    }
    if(canc)
    {
    te->error=true;
    te->msg="operation failed";
    }
    


}

int filelist_local::rename (string orgname, string newname)
{

    FXString newfile = newname.c_str ();
    FXString oldfile = orgname.c_str ();

#ifndef WIN32
    if (!FXFile::exists (oldfile))
	return false;
    if (FXFile::exists (newfile))
    {
	return false;

    }

    if (::rename (oldfile.text (), newfile.text ()) == 0)
	return true;
    if (errno != EXDEV)
	return false;
	
#else
    if (!FXFile::exists (oldfile))
	return false;
    if (FXFile::exists (newfile))
    {
	return false;

    }

    if (::MoveFile (oldfile.text (), newfile.text ()) != 0)
	return true;
#endif
}


int filelist_local::copymove (thread_elem * te, bool copy)
{

    unsigned long size = 0;

 
     vector < string >::iterator iter;

    for (iter = te->src.begin (); iter != te->src.end(); iter++)
    {
	string sr = (*iter);
	if (FXFile::isDirectory (sr.c_str ()))
	    sr.append (SEPARATOR);
	totalsize (sr, size);

    }

    te->total_size = size;

    bool canc = false;



    for (iter = te->src.begin (); iter != te->src.end(); iter++)
    {
	if (!canc)
	{
	    string ds = te->dst;
	    ds.append (SEPARATOR);
	    ds.append (FXFile::name (iter->c_str ()).text ());


	    string sr = (*iter);
	    if (FXFile::isDirectory (sr.c_str ()))
		sr.append (SEPARATOR);



	    if (copy)
	    {
		if (OSFXFile::copy (sr.c_str (), ds.c_str (), te) == FALSE)
		    canc = true;
	    }
	    else
	    {
		FXTRACE ((5, "MOVE;]"));
		if (OSFXFile::move (sr.c_str (), ds.c_str (), te) == FALSE)
		    canc = true;
	    }

	}

    }
    if(canc)
    {
    te->error=true;
    te->msg="operation failed";
    }

}



void filelist_local::totalsize (string path, unsigned long &size)
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

string filelist_local::info (void)
{
    return "";
}


int filelist_local::mode (string file)
{
    struct stat status;
    return !file.empty () && (::stat (file.c_str (), &status) == 0) ? status.st_mode : 0;
}

bool filelist_local::mode (string file, unsigned int mod, bool recursive)
{
    FXTRACE ((5, "MODE"));
    if (!recursive)
    {
	return FXFile::mode (file.c_str (), mod);
    }
    else
    {
	FXFile::mode (file.c_str (), mod);
	string path = file + SEPARATOR;

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
		    mode (file, mod, true);

		}
	    }

	    closedir (dirp);
	}


    }
}

string filelist_local::owner (string file)
{
    return FXFile::owner (file.c_str ()).text ();
}
string filelist_local::group (string file)
{
    return FXFile::group (file.c_str ()).text ();
}

bool filelist_local::owner (string file, string ownername, bool recursive)
{
    FXTRACE ((5, "MODE"));
    if (!recursive)
    {


	passwd *p = getpwnam (ownername.c_str ());
	return chown (file.c_str (), p->pw_uid, -1);

    }
    else
    {
	passwd *p = getpwnam (ownername.c_str ());
	chown (file.c_str (), p->pw_uid, -1);

	string path = file + SEPARATOR;

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
		    owner (file, ownername, true);

		}
	    }

	    closedir (dirp);
	}


    }
}

bool filelist_local::group (string file, string groupname, bool recursive)
{
    FXTRACE ((5, "MODE"));
    if (!recursive)
    {
	::group * grp = getgrnam (groupname.c_str ());
	return chown (file.c_str (), -1, grp->gr_gid);


    }
    else
    {
	::group * grp = getgrnam (groupname.c_str ());
	chown (file.c_str (), -1, grp->gr_gid);
	string path = file + SEPARATOR;

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
		    group (file, groupname, true);

		}
	    }

	    closedir (dirp);
	}


    }
}

string filelist_local::symlink (string path)
{
    return FXFile::symlink (path.c_str ()).text ();
}
bool filelist_local::symlink (string src, string dst)
{
    return FXFile::symlink (src.c_str (), dst.c_str ());
}
bool filelist_local::hardlink (string src, string dst)
{
    return FXFile::link (src.c_str (), dst.c_str ());
}


int filelist_local::quit (void)
{
    return 0;
}

EXPORTFUNCTION filelist_base *get_filelist (void)
{
    FXTRACE ((5, "PLUGIN LOAD\n"));
    return new filelist_local ();
}
