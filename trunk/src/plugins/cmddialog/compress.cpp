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

class compress_cmddialog:public cmddialog
{
  FXDECLARE (compress_cmddialog) protected:

    compress_cmddialog (const compress_cmddialog &)
    {
    }

  public:




    enum
    {
	ID_ENTER = cmddialog::ID_LAST,
    };


    string dir;
    FXTextField *textfield;
    FXComboBox* combo;

    compress_cmddialog ()
    {
    }
    compress_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src);


    virtual int compress_cmddialog::exec (void)
    {
	int error = 0;
	vector < string >::iterator iter;
	string file_list;
	string arch_filename=textfield->getText().text();
	string type=combo->getText().text();
	
    	for (iter = src.begin (); iter != src.end(); iter++)
    	{
		string sr = (*iter);
		sr=FXFile::name(sr.c_str()).text();
		file_list+=" \"" +  sr + "\"";
	}

	string command;
	if(type=="tar.bz2")
	command="tar cjf " + arch_filename +".tar.bz2 "+ file_list;
	else
	command="tar czf " + arch_filename +".tar.gz "+ file_list;
	
	system(command.c_str());


    }


    long compress_cmddialog::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (compress_cmddialog) compress_cmddialogMap[] =
{
FXMAPFUNC (SEL_COMMAND, compress_cmddialog::ID_ENTER, compress_cmddialog::press),};
FXIMPLEMENT (compress_cmddialog, cmddialog, compress_cmddialogMap, ARRAYNUMBER (compress_cmddialogMap)) 

compress_cmddialog::compress_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src):
cmddialog (w, fb, src)
{

  new FXLabel(contents,"archive type: ");

    combo = new FXComboBox (contents, 10, NULL, 0, COMBOBOX_NO_REPLACE | FRAME_SUNKEN | FRAME_THICK);
    combo->setNumVisible (3);
    combo->appendItem("tar.bz2");
    combo->appendItem("tar.gz");

	new FXLabel(contents,"archive filename: ");
	textfield = new FXTextField (contents, 25, this, ID_ENTER);
	textfield->setFocus ();
	
	
	
	
	




}

long compress_cmddialog::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}





EXPORTFUNCTION cmddialog *get_cmddialog (FXWindow * w, filelist_base * fb, vector < string > src)
{
    return new compress_cmddialog (w, fb, src);
}
