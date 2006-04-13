#include <fx.h>
#include "../../OSCMDDialogBox.h"

#include <string>
using namespace std;
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



#ifdef WIN32
#define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
#define EXPORTFUNCTION extern "C"
#endif


class OSAccess:public OSCMDDialogBox
{
  FXDECLARE (OSAccess) protected:

    OSAccess (const OSAccess &)
    {
    }

  private:
  
    FXCheckButton *ur;
    FXCheckButton *uw;
    FXCheckButton *ux;
    FXCheckButton *gr;
    FXCheckButton *gw;
    FXCheckButton *gx;
    FXCheckButton *or_;
    FXCheckButton *ow;
    FXCheckButton *ox;
    FXDataTarget radiotarget;
    FXRadioButton *set;
    FXRadioButton *clear;
    FXRadioButton *add;
    FXint cmd;
    FXCheckButton *rec;
    FXCheckButton *dironly;
    FXComboBox *user;
    FXComboBox *userbis;
    FXComboBox *grp;
    FXTextField *text;



  public:
  
    enum
    {
	ID_PRESS = OSCMDDialogBox::ID_LAST,
	ID_RUSR,
	ID_WUSR,
	ID_XUSR,
	ID_RGRP,
	ID_WGRP,
	ID_XGRP,
	ID_ROTH,
	ID_WOTH,
	ID_XOTH,
	ID_LAST
    };



    
    OSAccess ()
    {
    }
    OSAccess (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src);

    int exec (void)
    {

	unsigned int mode = 0;
	if (ur->getCheck ())
	    mode = mode | S_IRUSR;
	if (uw->getCheck ())
	    mode = mode | S_IWUSR;
	if (ux->getCheck ())
	    mode = mode | S_IXUSR;
	if (gr->getCheck ())
	    mode = mode | S_IRGRP;
	if (gw->getCheck ())
	    mode = mode | S_IWGRP;
	if (gx->getCheck ())
	    mode = mode | S_IXGRP;
	if (or_->getCheck ())
	    mode = mode | S_IROTH;
	if (ow->getCheck ())
	    mode = mode | S_IWOTH;
	if (ox->getCheck ())
	    mode = mode | S_IXOTH;

	bool recursive = false;
	if (rec->getCheck ())
	    recursive = true;
	
	vector < string >::iterator iter;

    	for (iter = src.begin (); iter != src.end(); iter++)
   	{
	
	    fb->mode (*iter, mode, recursive);
	   
	    fb->owner (*iter, user->getText ().text (), recursive);
	    fb->group (*iter, grp->getText ().text (), recursive);
	
	}

    }


    long OSAccess::press (FXObject * sender, FXSelector, void *);

};

FXDEFMAP (OSAccess) OSAccessMap[] =
{
FXMAPFUNC (SEL_COMMAND, OSAccess::ID_PRESS, OSAccess::press),};
FXIMPLEMENT (OSAccess, OSCMDDialogBox, OSAccessMap, ARRAYNUMBER (OSAccessMap)) 

OSAccess::OSAccess (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src):
OSCMDDialogBox (w, fb, src)
{

   vector < string >::iterator iter;
    for (iter = src.begin (); iter != src.end(); iter++)
    {
	new FXLabel (contents, iter->c_str ());
    }

    FXHorizontalFrame *accessframe = new FXHorizontalFrame (contents, LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXHorizontalFrame *chmodframe = new FXHorizontalFrame (contents, PACK_UNIFORM_WIDTH | PACK_UNIFORM_HEIGHT | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXGroupBox *group1 = new FXGroupBox (accessframe, "User", GROUPBOX_TITLE_LEFT | FRAME_GROOVE | LAYOUT_FILL_X);
    ur = new FXCheckButton (group1, "Read", this, ID_RUSR);
    uw = new FXCheckButton (group1, "Write", this, ID_WUSR);
    ux = new FXCheckButton (group1, "Execute", this, ID_XUSR);
    FXGroupBox *group2 = new FXGroupBox (accessframe, "Group", GROUPBOX_TITLE_LEFT | FRAME_GROOVE | LAYOUT_FILL_X);
    gr = new FXCheckButton (group2, "Read", this, ID_RGRP);
    gw = new FXCheckButton (group2, "Write", this, ID_WGRP);
    gx = new FXCheckButton (group2, "Execute", this, ID_XGRP);
    FXGroupBox *group3 = new FXGroupBox (accessframe, "Others", GROUPBOX_TITLE_LEFT | FRAME_GROOVE | LAYOUT_FILL_X);
    or_ = new FXCheckButton (group3, "Read", this, ID_ROTH);
    ow = new FXCheckButton (group3, "Write", this, ID_WOTH);
    ox = new FXCheckButton (group3, "Execute", this, ID_XOTH);

    FXGroupBox *group4 = new FXGroupBox (chmodframe, "Options", GROUPBOX_TITLE_LEFT | FRAME_GROOVE | LAYOUT_FILL_X | LAYOUT_FILL_Y);

    rec = new FXCheckButton (group4, "Recursive folders", NULL, 0);


    FXGroupBox *group5 = new FXGroupBox (chmodframe, "Owner", GROUPBOX_TITLE_LEFT | FRAME_GROOVE | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    new FXLabel (group5, "User");
    user = new FXComboBox (group5, 10, NULL, 0, COMBOBOX_NO_REPLACE | FRAME_SUNKEN | FRAME_THICK);
    user->setNumVisible (5);
    new FXLabel (group5, "Group");
    grp = new FXComboBox (group5, 10, NULL, 0, COMBOBOX_NO_REPLACE | FRAME_SUNKEN | FRAME_THICK);
    grp->setNumVisible (5);


    if (fb->mode (src[0]) & S_IRUSR)
	ur->setCheck ();
    if (fb->mode (src[0]) & S_IWUSR)
	uw->setCheck ();
    if (fb->mode (src[0]) & S_IXUSR)
	ux->setCheck ();
    if (fb->mode (src[0]) & S_IRGRP)
	gr->setCheck ();
    if (fb->mode (src[0]) & S_IWGRP)
	gw->setCheck ();
    if (fb->mode (src[0]) & S_IXGRP)
	gx->setCheck ();


    if (fb->mode (src[0]) & S_IROTH)
	or_->setCheck ();
    if (fb->mode (src[0]) & S_IWOTH)
	ow->setCheck ();
    if (fb->mode (src[0]) & S_IXOTH)
	ox->setCheck ();



    // User names (sorted in ascending order)
    struct passwd *pwde;
    while ((pwde = getpwent ()))
    {
	user->appendItem (pwde->pw_name);

    }
    endpwent ();
    user->setSortFunc (FXList::ascending);
    user->sortItems ();
    string own = fb->owner (src[0]);
    user->setCurrentItem (user->findItem (own.c_str ()));

    // Group names (sorted in ascending order)
    struct group *grpe;
    while ((grpe = getgrent ()))
    {
	grp->appendItem (grpe->gr_name);
    }
    endgrent ();
    grp->setSortFunc (FXList::ascending);
    grp->sortItems ();
    string grou = fb->group (src[0]);
    grp->setCurrentItem (grp->findItem (grou.c_str ()));

}

long OSAccess::press (FXObject * sender, FXSelector, void *)
{
}


EXPORTFUNCTION OSCMDDialogBox *get_cmddialog (FXWindow * w, OSVirtualFileSystemBase * fb, vector < string > src)
{
    return new OSAccess (w, fb, src);
}
