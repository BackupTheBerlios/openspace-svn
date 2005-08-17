#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include "filelist_sftp.h"




bool filelist_sftp::getprompt (void)
{


    if (readbuffer[0] == prompt[end])
    {
	if (end == 4)
	{
	    fxmessage ("END\n\n");
	    nbytes = read (p[0], readbuffer, 1);	//read ENTER        
	    end = 0;
	    return true;

	}
	else
	    end++;
    }
    else
	end = 0;

    return false;
}



int filelist_sftp::osopendir (string dir)
{

this->dir = dir;


    string command = "cd '" + dir + "'\n";
    write (o[1], command.c_str (), command.size ());
    nbytes = read (p[0], readbuffer, prompt.size ());
    fxmessage ("WRITE\n");
    fxmessage (command.c_str ());

    command = "ls -l\n";
    write (o[1], command.c_str (), command.size ());
    nbytes = read (p[0], readbuffer, prompt.size ());
    fxmessage ("WRITE\n");
    fxmessage (command.c_str ());


}
osfile filelist_sftp::osreaddir (void)
{
    osfile os_file;

    string line;

    while (1)
    {
	nbytes = read (p[0], readbuffer, 1);
	if (nbytes > 0)
	{

	    if (getprompt ())
		break;


	    if (readbuffer[0] == '\n')
	    {

		//fxmessage(line.c_str());
		//fxmessage("\n");
		std::stringstream parser (line);
		std::string field;
		parser >> field;
		if (field[0] == 'd')
		    os_file.type = FOLDER;
		else
		    os_file.type = 0;

		parser >> field;
		parser >> field;
		parser >> field;

		parser >> field;
		char *pEnd;
		os_file.size = strtoul (field.c_str (), &pEnd, 0);

		parser >> field;
		parser >> field;
		parser >> field;
		string name;
		while (parser >> field)
		{
		    name = name + " " + field;
		}


		os_file.name = name.erase (0, 1);
		//fxmessage(field.c_str());
		//fxmessage("\n");




		return os_file;

		line = "";
	    }
	    else
	    {
		line = line + readbuffer[0];
	    }

	}
	else
	{
	    fxmessage ("KONIEC\n");
	    break;
	}



    }


    os_file.name = "";
    return os_file;

}
int filelist_sftp::mkdir (string dir, int mode)
{
}
int filelist_sftp::copy (thread_elem * te)
{
}
int filelist_sftp::move (thread_elem * te)
{
}
int filelist_sftp::remove (thread_elem * te)
{
}
int filelist_sftp::rename (string orgname, string newname)
{
}
int filelist_sftp::init (vector < string > *vector_name, pathtype pt, configure * conf)
{

    pipe (p);
    pipe (o);
    end = 0;

    prompt = "sftp>";

    if (fork () == 0)
    {
	dup2 (p[1], 1);
	dup2 (o[0], 0);
	close (p[0]);
	close (o[1]);
	string param0="-b";
	string param1="/dev/stdin";
	string param2 = pt.user + "@" + pt.server;

	execl ("/usr/bin/sftp", "sftp",param0.c_str(),param1.c_str (), param2.c_str (), NULL);
	fxmessage ("wylalzem :D");
	_exit (0);


    }
    else
    {
	fxmessage ("MAIN\n");
	close (p[1]);
	close (o[0]);
	write(o[1],pt.password.c_str(),pt.password.size());
	fxmessage("A");
	   string askpass="Password: ";
	   read(p[0], readbuffer, 1);
	   fxmessage("ocztane=");
	   string s;
	   s=s+readbuffer[0];
	   fxmessage(s.c_str());
	   if(readbuffer[0]=='P')
	   {
	   fxmessage("haselo\n");
	   read(p[0], readbuffer, askpass.size()-1);
	   
	   write(o[1],pt.password.c_str(),pt.password.size());
	   read(p[0], readbuffer, 1);
	   }
	   //else //key authorization
	
	if (read (p[0], readbuffer, prompt.size ()) == -1)	//read PROMPT
	{
	    fxmessage ("failed\n");
	    return -1;
	}

    }

  

}
int filelist_sftp::mode (string file)
{
}
string filelist_sftp::owner (string file)
{
return "";
}
string filelist_sftp::group (string file)
{
return "";
}
bool filelist_sftp::mode (string file, unsigned int, bool recursive)
{
}
bool filelist_sftp::owner (string file, string, bool recursive)
{
}
bool filelist_sftp::group (string file, string, bool recursive)
{
}
string filelist_sftp::info (void)
{
return "";
}
void filelist_sftp::totalsize (string path, unsigned long &size)
{
}
string filelist_sftp::symlink (string path)
{
return "";
}
bool filelist_sftp::symlink (string src, string dst)
{
}
bool filelist_sftp::hardlink (string src, string dst)
{
}
int filelist_sftp::supportedfunctions (void)
{
}
int filelist_sftp::quit (void)
{
}
EXPORTFUNCTION filelist_base *get_filelist (void)
{
    FXTRACE ((5, "PLUGIN LOAD\n"));
    return new filelist_sftp ();
}
