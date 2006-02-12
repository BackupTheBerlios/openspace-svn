#include <fx.h>
#include "../../OSCMDDialogBox.h"

#include <string>
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

class OSRename:public OSCMDDialogBox
{
  FXDECLARE (OSRename) protected:

    OSRename (const OSRename &)
    {
    }
    
  private:

    vector < FXTextField * >vec;
    string dir;  

  public:

    enum
    {
	ID_ENTER = OSCMDDialogBox::ID_LAST,
    };

    OSRename ()
    {
    }
    OSRename (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src);


    virtual int OSRename::exec (void)
    {
	int error = 0;
	vector < string >::iterator iter;
	int i=0;
    	for (iter = src.begin (); iter != src.end(); iter++)
    	{
	    string newname = dir + SEPARATOR + vec[i]->getText ().text ();
	    if (fb->rename (*iter, newname) == false)
	    {
		error = -1;
		string err = "can't rename " + *iter + " to " + newname;

		FXLabel *lab = new FXLabel (contents, err.c_str ());
		lab->create ();
		this->layout ();
		this->resize (this->getWidth (), this->getHeight () + lab->getHeight ());

	    }
	i++;
	}


	return error;

    }


    long OSRename::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (OSRename) OSRenameMap[] =
{
FXMAPFUNC (SEL_COMMAND, OSRename::ID_ENTER, OSRename::press),};
FXIMPLEMENT (OSRename, OSCMDDialogBox, OSRenameMap, ARRAYNUMBER (OSRenameMap)) 

OSRename::OSRename (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src):
OSCMDDialogBox (w, fb, src)
{


    vector < string >::iterator iter;

    for (iter = src.begin (); iter != src.end(); iter++)
    {
  

	dir = FXFile::directory (iter->c_str ()).text ();
	//fxmessage(dir.c_str());
	//fxmessage("\n woho");
	FXString name = FXFile::name (iter->c_str ());
	new FXLabel (contents, name);
	FXTextField *text = new FXTextField (contents, 25, this, ID_ENTER);
	text->setFocus ();
	text->setText (name);
	vec.push_back (text);

    }



}

long OSRename::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}



EXPORTFUNCTION OSCMDDialogBox *get_cmddialog (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src)
{
    return new OSRename (w, fb, src);
}
