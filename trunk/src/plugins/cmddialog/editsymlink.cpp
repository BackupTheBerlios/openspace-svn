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

class editsymlink_cmddialog:public cmddialog
{
  FXDECLARE (editsymlink_cmddialog) protected:

    editsymlink_cmddialog (const editsymlink_cmddialog &)
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

    editsymlink_cmddialog ()
    {
    }
    editsymlink_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src);


    virtual int editsymlink_cmddialog::exec (void)
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
	    thread_elem *el = new thread_elem (fb, "remove", "",srcfile);
	    fb->remove(el);
	    
	    fxmessage("\nSRC=%s, DST=%s",iter->c_str(),newname.c_str());

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



    long editsymlink_cmddialog::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (editsymlink_cmddialog) editsymlink_cmddialogMap[] =
{
FXMAPFUNC (SEL_COMMAND, editsymlink_cmddialog::ID_ENTER, editsymlink_cmddialog::press),};
FXIMPLEMENT (editsymlink_cmddialog, cmddialog, editsymlink_cmddialogMap, ARRAYNUMBER (editsymlink_cmddialogMap)) 

editsymlink_cmddialog::editsymlink_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src):
cmddialog (w, fb, src)
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

long editsymlink_cmddialog::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    //target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}





EXPORTFUNCTION cmddialog *get_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src)
{
    return new editsymlink_cmddialog (w, fb, src);
}
