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

class OSSymlink:public OSCMDDialogBox
{
  FXDECLARE (OSSymlink) protected:

    OSSymlink (const OSSymlink &)
    {
    }

  private:
  
    vector < FXTextField * >vec;
    string dir;
    FXCheckButton *hardlink;

  public:

    enum
    {
	ID_ENTER = OSCMDDialogBox::ID_LAST,
    };

    OSSymlink ()
    {
    }
    OSSymlink (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src);


    virtual int OSSymlink::exec (void)
    {
	int error = 0;
	vector < string >::iterator iter;
	int i=0;
    	for (iter = src.begin (); iter != src.end(); iter++)
    	{
	    string newname =  vec[i]->getText ().text ();
	    
	    bool ret;
	    if (hardlink->getCheck ())
	    	ret=fb->hardlink (*iter, newname);
	    else
	        ret=fb->symlink (*iter, newname);	
			
	    if ( ret== false)
	    {
		error = -1;
		string err = "can't symlink " + *iter + " to " + newname;

		FXLabel *lab = new FXLabel (contents, err.c_str ());
		lab->create ();
		this->layout ();
		this->resize (this->getWidth (), this->getHeight () + lab->getHeight ());

	    }
	i++;
	}


	return error;

    }


    long OSSymlink::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (OSSymlink) OSSymlinkMap[] =
{
FXMAPFUNC (SEL_COMMAND, OSSymlink::ID_ENTER, OSSymlink::press),};
FXIMPLEMENT (OSSymlink, OSCMDDialogBox, OSSymlinkMap, ARRAYNUMBER (OSSymlinkMap)) 

OSSymlink::OSSymlink (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src):
OSCMDDialogBox (w, fb, src)
{


    vector < string >::iterator iter;

    for (iter = src.begin (); iter != src.end(); iter++)
    {
  

	new FXLabel(contents,"source: ");
	new FXLabel (contents, iter->c_str ());
	new FXLabel(contents,"link: ");
	FXTextField *text = new FXTextField (contents, 25, this, ID_ENTER);
	text->setFocus ();
	text->setText (iter->c_str ());
	vec.push_back (text);

    }

hardlink = new FXCheckButton (contents, "hardlink");

}

long OSSymlink::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    //target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}


EXPORTFUNCTION OSCMDDialogBox *get_cmddialog (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src)
{
    return new OSSymlink (w, fb, src);
}
