#include "fx.h"
#include "config.h"
#include "mainwindow.h"
#include <unistd.h>

//-----MAIN--------------------------------------------------------------------------------------- 
//nothing special here, create and execute main window,
//creating needed directoris if doesn't exist
int main (int argc, char **argv)
{

	for(int i=0;i<argc;i++)
	{
		if(!strcmp(argv[i],"-v") || !strcmp(argv[i],"--version") )
		{
		fxmessage("\n%s\n",PACKAGE_STRING);
		return 0;
		}
		
		if(!strcmp(argv[i],"-h") || !strcmp(argv[i],"--help") )
		{
		fxmessage("\n-h, --help print this help\n");
		fxmessage("-v, --version print version\n");
		return 0;
		}
	}
	
        string tmpdir="/tmp/openspace";	
        if(!FXFile::exists(tmpdir.c_str()))
	{
	FXFile::createDirectory(tmpdir.c_str(),493);	
	}
	
        string dir = FXFile::getUserDirectory ("").text ();
        dir=dir +  "/.openspace";
        string file = dir +  "/openspacerc";
	
	if(!FXFile::exists(file.c_str()))
	{
	FXFile::createDirectory(dir.c_str(),493);
	string dir2=dir+"/icons";
	FXFile::createDirectory(dir2.c_str(),493);	
	dir+="/plugins";
	FXFile::createDirectory(dir.c_str(),493);
	dir2=dir+"/cmddialog";
	FXFile::createDirectory(dir2.c_str(),493);
	dir2=dir+"/filelist";
	FXFile::createDirectory(dir2.c_str(),493);
	
		
	string src=PATH_CFG + string("/openspacerc");
	FXFile::copy(src.c_str(),file.c_str());
	}
    fxTraceLevel = 0;
    FXApp *application = new FXApp ("openspace", NULL);
    application->init (argc, argv);
 
    new MainWindow (application);
    application->create ();
    return application->run ();
}

//-----MAIN------------------------------------------------------------------------------------------------------------------------------------------       
