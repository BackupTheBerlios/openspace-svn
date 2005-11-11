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

class clone_cmddialog:public cmddialog
{
  FXDECLARE (clone_cmddialog) protected:

    clone_cmddialog (const clone_cmddialog &)
    {
    }

  private:
    vector < FXTextField * >vec;
    string dir;

  public:

    enum
    {
	ID_ENTER = cmddialog::ID_LAST,
    };


    clone_cmddialog ()
    {
    }
    clone_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src);


    virtual int clone_cmddialog::exec (void)
    {
	int error = 0;
	vector < string >::iterator iter;
	int i=0;
    	for (iter = src.begin (); iter != src.end(); iter++)
    	{
	    string newname = dir + SEPARATOR + vec[i]->getText ().text ();
	    string newonlyname= vec[i]->getText ().text ();
	    
	    string destdir="/tmp/openspace";
	    string filename=FXFile::name(iter->c_str()).text();
	    string dirname=FXFile::directory(iter->c_str()).text();
	    
	    thread_elem *el2 = new thread_elem (fb, "copy", "upload", *iter,destdir);
	    fb->copy (el2);
	    delete el2;
	    
	    	    
	    
	    if (fb->rename (destdir+"/"+filename,destdir+"/"+newonlyname) == false)
	    {
		error = -1;
		string err = "can't clone " + *iter + " to " + newname;

		FXLabel *lab = new FXLabel (contents, err.c_str ());
		lab->create ();
		this->layout ();
		this->resize (this->getWidth (), this->getHeight () + lab->getHeight ());

	    }
	    else
	    {
	    
	    el2 = new thread_elem (fb, "move", "upload", destdir+"/"+newonlyname,dirname);
	    fb->copy (el2);
	    delete el2;
	    
	    }
	    
	    
	i++;
	}


	return error;

    }


    long clone_cmddialog::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (clone_cmddialog) clone_cmddialogMap[] =
{
FXMAPFUNC (SEL_COMMAND, clone_cmddialog::ID_ENTER, clone_cmddialog::press),};
FXIMPLEMENT (clone_cmddialog, cmddialog, clone_cmddialogMap, ARRAYNUMBER (clone_cmddialogMap)) 

clone_cmddialog::clone_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src):
cmddialog (w, fb, src)
{


    vector < string >::iterator iter;

    for (iter = src.begin (); iter != src.end(); iter++)
    {
  

	dir = FXFile::directory (iter->c_str ()).text ();

	FXString name = FXFile::name (iter->c_str ());
	new FXLabel (contents, name);
	FXTextField *text = new FXTextField (contents, 25, this, ID_ENTER);
	text->setFocus ();
	text->setText (name);
	vec.push_back (text);

    }



}

long clone_cmddialog::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}





EXPORTFUNCTION cmddialog *get_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src)
{
    return new clone_cmddialog (w, fb, src);
}
