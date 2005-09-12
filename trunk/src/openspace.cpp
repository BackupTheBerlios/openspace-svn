#include "fx.h"
#include "mainwindow.h"
#include <unistd.h>

//-----MAIN------------------------------------------------------------------------------------------------------------------------------------------ 
//nothing special here, create and execute main window
int main (int argc, char **argv)
{
    string dir = FXFile::getUserDirectory ("").text ();
    dir=dir +  "/.openspace";
    string file = dir +  "/openspacerc";
	if(!FXFile::exists(file.c_str()))
	{
	FXFile::createDirectory(dir.c_str(),655);
	dir+="/plugins";
	FXFile::createDirectory(dir.c_str(),655);
	dir+="/cmddialog";
	FXFile::createDirectory(dir.c_str(),655);
		
	string src=PATH_CFG + string("/openspacerc");
	FXFile::copy(src.c_str(),file.c_str());
	}
    fxTraceLevel = 12;
    FXApp *application = new FXApp ("openspace", NULL);
    application->init (argc, argv);
    fxmessage (argv[0]);
    new MainWindow (application);
    application->create ();
    return application->run ();
}

//-----MAIN------------------------------------------------------------------------------------------------------------------------------------------       
