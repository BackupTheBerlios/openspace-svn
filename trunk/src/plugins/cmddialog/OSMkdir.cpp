#include <fx.h>
#include "../../OSCMDDialogBox.h"

#include <string>
using namespace std;

#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif


class OSMkdir:public OSCMDDialogBox
{
  FXDECLARE (OSMkdir) protected:

    OSMkdir (const OSMkdir &)
    {
    }
  private:
  FXTextField *text;

  public:

    enum
    {
	ID_ENTER = OSCMDDialogBox::ID_LAST,
    };

    OSMkdir ()
    {
    }
    OSMkdir (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src);

    int exec (void)
    {
	string s = text->getText ().text ();
	fb->mkdir (s, 493);
    }


    long press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (OSMkdir) OSMkdirMap[] =
{
FXMAPFUNC (SEL_COMMAND, OSMkdir::ID_ENTER, OSMkdir::press),};
FXIMPLEMENT (OSMkdir, OSCMDDialogBox, OSMkdirMap, ARRAYNUMBER (OSMkdirMap)) 

OSMkdir::OSMkdir (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src):
OSCMDDialogBox (w, fb, src)
{


    text = new FXTextField (contents, 25, this, ID_ENTER);
    text->setFocus ();

}

long OSMkdir::press (FXObject * sender, FXSelector, void *)
{

    FXObject *target = (FXObject *) ok->getTarget ();
    target->handle (ok, FXSEL (SEL_COMMAND, ID_COMMAND), NULL);

}



EXPORTFUNCTION OSCMDDialogBox *get_cmddialog (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src)
{
    return new OSMkdir (w, fb, src);
}
