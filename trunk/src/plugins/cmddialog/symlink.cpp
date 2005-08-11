#include <fx.h>
#include "../../cmddialog.h"

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

class symlink_cmddialog:public cmddialog
{
  FXDECLARE (symlink_cmddialog) protected:

    symlink_cmddialog (const symlink_cmddialog &)
    {
    }

  public:




    enum
    {
	ID_ENTER = cmddialog::ID_LAST,
    };


    vector < FXTextField * >vec;
    string dir;
    FXCheckButton *hardlink;

    symlink_cmddialog ()
    {
    }
    symlink_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src);


    virtual int symlink_cmddialog::exec (void)
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


    long symlink_cmddialog::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (symlink_cmddialog) symlink_cmddialogMap[] =
{
FXMAPFUNC (SEL_COMMAND, symlink_cmddialog::ID_ENTER, symlink_cmddialog::press),};
FXIMPLEMENT (symlink_cmddialog, cmddialog, symlink_cmddialogMap, ARRAYNUMBER (symlink_cmddialogMap)) 

symlink_cmddialog::symlink_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src):
cmddialog (w, fb, src)
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

long symlink_cmddialog::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    //target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}





EXPORTFUNCTION cmddialog *get_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src)
{
    return new symlink_cmddialog (w, fb, src);
}
