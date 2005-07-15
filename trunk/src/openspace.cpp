#include "fx.h"
#include "mainwindow.h"
#include <unistd.h>

     
//-----MAIN------------------------------------------------------------------------------------------------------------------------------------------ 

int main(int argc,char ** argv) 
{
	
fxTraceLevel=10;
	
	FXApp *application=new FXApp("openspace",NULL);
	application->init(argc,argv);
	fxmessage(argv[0]);
	
	
	
	new MainWindow(application);
	application->create(); 

	return application->run();
	
   
}     
    
//-----MAIN------------------------------------------------------------------------------------------------------------------------------------------       

