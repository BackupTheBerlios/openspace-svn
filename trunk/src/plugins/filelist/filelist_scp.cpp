
#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif

#include <fx.h>
#include "filelist_scp.h"
#include "../../sharedobjects.h"


#include <fstream>
#include <sstream>
#include <stdlib.h>

#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif


int filelist_scp::init (string ** name, unsigned int **type, unsigned int **width, pathtype pt, configure * conf)
{
    configure conflocal = *conf;
    int size = conflocal.countxpath ("/OpenspaceConfig/filelist/scp/headers/header") + 1;
    fields = new string[size];
    fieldsnum = size;

    (*name) = new string[size];
    (*type) = new unsigned int[size];
    (*width) = new unsigned int[size];
    string wi;
    (*name)[0] = "name";
    fields[0] = "name";
    if ((wi = conflocal.readonestring ("/OpenspaceConfig/filelist/scp/properties/name/width")) != "")
	(*width)[0] = atoi (wi.c_str ());
    else
	(*width)[0] = 100;
    (*type)[0] = 0;
    int counter = 1;

    display_size = conflocal.readonestring ("/OpenspaceConfig/filelist/scp/properties/name/display");

    if (conflocal.openxpath ("/OpenspaceConfig/filelist/scp/headers/header") != -1)
    {
	while (1)
	{
	    string res = conflocal.getnextstring ();
	    if (res == "")
		break;

	    if ((wi = conflocal.readonestring ("/OpenspaceConfig/filelist/scp/properties/" + res + "/width")) != "")
	    {

		(*width)[counter] = atoi (wi.c_str ());
	    }
	    else
		(*width)[counter] = 40;

	    (*name)[counter] = res;
	    fields[counter] = res;
	    if (res == "size")
	    {
		(*type)[counter] = 1;
	    }
	    else
	    {
		(*type)[counter] = 0;
	    }


	    counter++;
	}
    }

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
	string param = pt.user + "@" + pt.server;

	execl ("/usr/bin/sftp", "sftp", param.c_str (), NULL);
	fxmessage ("wylalzem :D");
	_exit (0);


    }
    else
    {
	fxmessage ("MAIN\n");
	close (p[1]);
	close (o[0]);

	/*
	   string askpass="Password: ";
	   read(p[0], readbuffer, 1);
	   fxmessage("ocztane=");
	   string s;
	   s=s+readbuffer[0];
	   fxmessage(s.c_str());
	   if(readbuffer[0]=='P')
	   {
	   read(p[0], readbuffer, askpass.size()-1);
	   string pass="aaa\n";
	   write(o[1],pass.c_str(),pass.size());
	   read(p[0], readbuffer, 1);
	   }
	   else //key authorization
	 */
	if (read (p[0], readbuffer, prompt.size ()) == -1)	//read PROMPT
	{
	    fxmessage ("failed\n");
	    return -1;
	}

    }

    return size;
}


bool filelist_scp::getprompt (void)
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

osfile filelist_scp::osreaddir ()
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



		os_file.attrib = new string[fieldsnum];
		os_file.attrib[0] = numtostring (os_file.size);
		os_file.attrib[1] = "";

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


int filelist_scp::osopendir (string dir)
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

int filelist_scp::mkdir (string dir, int mode)
{


    string command = "mkdir '" + dir + "'\n";
    write (o[1], command.c_str (), command.size ());
    nbytes = read (p[0], readbuffer, prompt.size ());
    fxmessage ("WRITE\n");
    fxmessage (command.c_str ());

}

int filelist_scp::copy (thread_elem * te)
{

    fxmessage ("COPY SCP\n");
    if (te->options == "upload")
    {
	int c = 0;
	while (te->src[c] != "")
	{
	    string command = "put '" + te->src[c] + "'\n";
	    write (o[1], command.c_str (), command.size ());
	    c++;
	    while (1)
	    {
		nbytes = read (p[0], readbuffer, 1);
		if (nbytes > 0)
		{
		    if (getprompt ())
			break;
		}
		else
		    break;
	    }
	}

    }
    else
    {
	int c = 0;
	while (te->src[c] != "")
	{
	    string command = "lcd '" + te->dst + "'\n";
	    write (o[1], command.c_str (), command.size ());
	    nbytes = read (p[0], readbuffer, prompt.size ());

	    command = "get '" + te->src[c] + "'\n";
	    write (o[1], command.c_str (), command.size ());
	    c++;
	    while (1)
	    {
		nbytes = read (p[0], readbuffer, 1);
		if (nbytes > 0)
		{
		    if (getprompt ())
			break;
		}
		else
		    break;
	    }
	}

    }
}

int filelist_scp::move (thread_elem * te)
{

}


int filelist_scp::remove (thread_elem * te)
{


}


string filelist_scp::info (void)
{
    return "buu";
}


void filelist_scp::totalsize (string path, unsigned long &size)
{


}

int filelist_scp::rename (string orgname, string newname)
{
    string command = "rename '" + orgname + "' '" + newname + "'\n";
    write (o[1], command.c_str (), command.size ());
    nbytes = read (p[0], readbuffer, prompt.size ());
    fxmessage ("WRITE\n");
    fxmessage (command.c_str ());


}

unsigned int filelist_scp::mode (string file)
{

}

bool filelist_scp::mode (string file, unsigned int mod, bool recursive)
{
}

int filelist_scp::quit (void)
{


    string command = "quit\n";
    write (o[1], command.c_str (), command.size ());
//nbytes = read(p[0], readbuffer, prompt.size());
//fxmessage(readbuffer);

    close (p[0]);
    close (o[1]);
    fxmessage ("QUIT\n");


}


EXPORTFUNCTION filelist_base *get_filelist (void)
{
    fxmessage ("zwracamy");
    return new filelist_scp ();
}
