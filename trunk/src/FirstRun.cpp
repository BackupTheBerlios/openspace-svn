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
    
    FXButton *but1 = new FXButton (vertical, "manual configuration", NULL, this,ID_CONF_MANUAL);
    FXButton *but2 = new FXButton (vertical, "semiautomatic configuration", NULL, this,ID_CONF_SEMI);
    FXButton *but3 = new FXButton (vertical, "full automatic configuration", NULL, this,ID_CONF_AUTOMATIC);
	
	
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
	prefer->show(PLACEMENT_OWNER);
	}
	else if(id==ID_CONF_SEMI)
	{
	MimeApp *mimeapp=new MimeApp(this);
	mimeapp->create();
	mimeapp->show(PLACEMENT_OWNER);
	}
	else if(id==ID_CONF_AUTOMATIC)
	{
		MimeApp *mimeapp=new MimeApp(this);
		vector <string>::iterator iter;
			
		for (iter=mimeapp->iter; iter != mimeapp->mime_vector.end(); iter++)
    		{
			mimeapp->fill(*iter);
			mimeapp->save(mimeapp->mime_label->getText().text(),mimeapp->programsbox->getText().text());
		}
	
	
	}
	

}
