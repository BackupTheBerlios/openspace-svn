#include "fx.h"
#include "FirstRun.h"
#include "MimeApp.h"
#include "preferences.h"

using namespace std;

FXDEFMAP (FirstRun) FirstRunMap[] =
{
    //________Message_Type_____________________ID____________Message_Handler_______  
	FXMAPFUNCS (SEL_COMMAND, FirstRun::ID_CONF_MANUAL,FirstRun::ID_CONF_AUTOMATIC, FirstRun::onConfigure),
};


FXIMPLEMENT (FirstRun, FXDialogBox, FirstRunMap, ARRAYNUMBER (FirstRunMap)) FirstRun::FirstRun (FXWindow * owner):FXDialogBox (owner, "FirstRun", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4)
{


    FXVerticalFrame *vertical = new FXVerticalFrame (this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);    
    
    FXGroupBox *vv = new FXGroupBox (vertical, "", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0);
    new FXLabel(vv,"Best for newbie and first run");	
    FXButton *but3 = new FXButton (vv, "full automatic configuration", NULL, this,ID_CONF_AUTOMATIC);
     	
    vv = new FXGroupBox (vertical, "", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0);
    new FXLabel(vv,"Choose which program will open given file type");	
    FXButton *but2 = new FXButton (vv, "semiautomatic configuration", NULL, this,ID_CONF_SEMI);    
    
    vv = new FXGroupBox (vertical, "", LAYOUT_SIDE_TOP | FRAME_GROOVE | LAYOUT_FILL_X, 0, 0, 0, 0);
    new FXLabel(vv,"Not recomended - try better semi-auto configuration\n and then manual rather than only manual"); 
    FXButton *but1 = new FXButton (vv, "manual configuration", NULL, this,ID_CONF_MANUAL);
	
	
}



FirstRun::~FirstRun ()
{
}



long FirstRun::onConfigure(FXObject * sender, FXSelector sel, void *)
{
	FXushort id = FXSELID(sel);
	if(id==ID_CONF_MANUAL)
	{
	preferences *prefer=new preferences(this);
	prefer->create();
	prefer->execute(PLACEMENT_OWNER);
	}
	else if(id==ID_CONF_SEMI)
	{
	MimeApp *mimeapp=new MimeApp(this);
	mimeapp->create();
	mimeapp->execute(PLACEMENT_OWNER);
	}
	else if(id==ID_CONF_AUTOMATIC)
	{
		MimeApp *mimeapp=new MimeApp(this);
		mimeapp->doAutomaticConfiguration();
	
	}
	FXMessageBox about (this, "restart", "restart openspace to apply changes", NULL, MBOX_OK | DECOR_TITLE | DECOR_BORDER);
   	about.execute ();

}
