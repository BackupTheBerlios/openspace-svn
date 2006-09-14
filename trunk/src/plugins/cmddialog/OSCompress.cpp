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

class OSCompress:public OSCMDDialogBox
{
  FXDECLARE (OSCompress) protected:

    OSCompress (const OSCompress &)
    {
    }

  private:
   
    string dir;
    FXTextField *textfield;
    FXComboBox* combo;

  public:


    enum
    {
	ID_ENTER = OSCMDDialogBox::ID_LAST,
    };


       OSCompress ()
    {
    }
    OSCompress (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src);


    int exec (void)
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


    long press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (OSCompress) OSCompressMap[] =
{
FXMAPFUNC (SEL_COMMAND, OSCompress::ID_ENTER, OSCompress::press),};
FXIMPLEMENT (OSCompress, OSCMDDialogBox, OSCompressMap, ARRAYNUMBER (OSCompressMap)) 

OSCompress::OSCompress (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src):
OSCMDDialogBox (w, fb, src)
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

long OSCompress::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}





EXPORTFUNCTION OSCMDDialogBox *get_cmddialog (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src)
{
    return new OSCompress (w, fb, src);
}
