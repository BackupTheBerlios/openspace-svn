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

class md5sum_cmddialog:public cmddialog
{
  FXDECLARE (md5sum_cmddialog) protected:

    md5sum_cmddialog (const md5sum_cmddialog &)
    {
    }
    
  private:

 

  public:

    enum
    {
	ID_ENTER = cmddialog::ID_LAST,
    };

    md5sum_cmddialog ()
    {
    }
    md5sum_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src);


    virtual int md5sum_cmddialog::exec (void)
    {
	
    }


    long md5sum_cmddialog::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (md5sum_cmddialog) md5sum_cmddialogMap[] =
{
FXMAPFUNC (SEL_COMMAND, md5sum_cmddialog::ID_ENTER, md5sum_cmddialog::press),};
FXIMPLEMENT (md5sum_cmddialog, cmddialog, md5sum_cmddialogMap, ARRAYNUMBER (md5sum_cmddialogMap)) 

md5sum_cmddialog::md5sum_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src):
cmddialog (w, fb, src)
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

long md5sum_cmddialog::press (FXObject * sender, FXSelector, void *)
{

 

}



EXPORTFUNCTION cmddialog *get_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src)
{
    return new md5sum_cmddialog (w, fb, src);
}
