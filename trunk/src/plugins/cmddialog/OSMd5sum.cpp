#include <fx.h>
#include "../../OSCMDDialogBox.h"

#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif


#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif

class OSMd5sum:public OSCMDDialogBox
{
  FXDECLARE (OSMd5sum) protected:

    OSMd5sum (const OSMd5sum &)
    {
    }
    
  private:

 

  public:

    enum
    {
	ID_ENTER = OSCMDDialogBox::ID_LAST,
    };

    OSMd5sum ()
    {
    }
    OSMd5sum (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src);


    virtual int exec (void)
    {
	
    }


    long press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (OSMd5sum) OSMd5sumMap[] =
{
FXMAPFUNC (SEL_COMMAND, OSMd5sum::ID_ENTER, OSMd5sum::press),};
FXIMPLEMENT (OSMd5sum, OSCMDDialogBox, OSMd5sumMap, ARRAYNUMBER (OSMd5sumMap)) 

OSMd5sum::OSMd5sum (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src):
OSCMDDialogBox (w, fb, src)
{
FILE *pipe;
char readbuf[1024];

    vector < string >::iterator iter;
FXPacker *vv = new FXGroupBox (contents, "info", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0); 
new FXLabel(vv,"MD5SUM additional plugin - Mateusz Dworak - GPL ");
new FXSeparator(contents);
    for (iter = src.begin (); iter != src.end(); iter++)
    {
  
	string cmd="md5sum "+*iter;
	pipe = popen (cmd.c_str(), "r");

    	if (pipe != NULL)
   	{
		string line;
		while (fgets (readbuf, sizeof (readbuf), pipe) != NULL)
		{
		std::stringstream parser (readbuf);
		string md5;	
		parser >> md5;
		
		new FXLabel(contents,iter->c_str());
		FXTextField*textfield = new FXTextField (contents, 31);
		textfield->setText(md5.c_str());
		}
	}

    }



}

long OSMd5sum::press (FXObject * sender, FXSelector, void *)
{

 

}



EXPORTFUNCTION OSCMDDialogBox *get_cmddialog (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src)
{
    return new OSMd5sum (w, fb, src);
}
