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

class rename_cmddialog:public cmddialog
{
  FXDECLARE (rename_cmddialog) protected:

    rename_cmddialog (const rename_cmddialog &)
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

    rename_cmddialog ()
    {
    }
    rename_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src);


    virtual int rename_cmddialog::exec (void)
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


    long rename_cmddialog::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (rename_cmddialog) rename_cmddialogMap[] =
{
FXMAPFUNC (SEL_COMMAND, rename_cmddialog::ID_ENTER, rename_cmddialog::press),};
FXIMPLEMENT (rename_cmddialog, cmddialog, rename_cmddialogMap, ARRAYNUMBER (rename_cmddialogMap)) 

rename_cmddialog::rename_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src):
cmddialog (w, fb, src)
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

long rename_cmddialog::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}



EXPORTFUNCTION cmddialog *get_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src)
{
    return new rename_cmddialog (w, fb, src);
}
