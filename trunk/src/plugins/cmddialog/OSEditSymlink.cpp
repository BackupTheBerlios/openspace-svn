#include <fx.h>
#include "../../OSCMDDialogBox.h"
#include "../../OSThreadExec.h"

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

class OSEditSymlink:public OSCMDDialogBox
{
  FXDECLARE (OSEditSymlink) protected:

    OSEditSymlink (const OSEditSymlink &)
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

    OSEditSymlink ()
    {
    }
    OSEditSymlink (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src);


    int exec (void)
    {
    
        int error = 0;
    
	
	vector < string >::iterator iter;
	int i=0;
    	for (iter = src.begin (); iter != src.end(); iter++)
    	{
	
	string newname = vec[i]->getText ().text ();
	delete vec[i];
	i++;
	if(newname=="")
	continue;

	    
  
	int error = 0;
	    vector<string> srcfile;
	    srcfile.push_back(*iter);
	    OSThreadExec *el = new OSThreadExec (fb, "remove", "",srcfile);
	    fb->remove(el);
	
           bool ret=fb->symlink ( newname,*iter);	
	
	    if ( ret== false)
	    {
		error = -1;
		string err = "can't symlink " + *iter + " to " + newname;

		FXLabel *lab = new FXLabel (contents, err.c_str ());
		lab->create ();
		this->layout ();
		this->resize (this->getWidth (), this->getHeight () + lab->getHeight ());

	    }
	
	}

    	
	return error;
	
    }



    long OSEditSymlink::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (OSEditSymlink) OSEditSymlinkMap[] =
{
FXMAPFUNC (SEL_COMMAND, OSEditSymlink::ID_ENTER, OSEditSymlink::press),};
FXIMPLEMENT (OSEditSymlink, OSCMDDialogBox, OSEditSymlinkMap, ARRAYNUMBER (OSEditSymlinkMap)) 

OSEditSymlink::OSEditSymlink (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src):
OSCMDDialogBox (w, fb, src)
{


    vector < string >::iterator iter;

    for (iter = src.begin (); iter != src.end(); iter++)
    {
	string sym=fb->symlink(*iter);
	if(sym=="")
	continue;

	new FXLabel(contents,"source: ");
	new FXLabel (contents, iter->c_str ());
	new FXLabel(contents,"link: ");
	FXTextField *text = new FXTextField (contents, 25, this, ID_ENTER);
	text->setFocus ();
	text->setText (sym.c_str());
	vec.push_back (text);

    }


}

long OSEditSymlink::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    //target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}


EXPORTFUNCTION OSCMDDialogBox *get_cmddialog (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src)
{
    return new OSEditSymlink (w, fb, src);
}
