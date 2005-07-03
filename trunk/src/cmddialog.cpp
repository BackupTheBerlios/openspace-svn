#include "fx.h"

#include "cmddialog.h"

    
FXIMPLEMENT(cmddialog,FXDialogBox,NULL,0)

cmddialog::cmddialog(FXWindow* owner,filelist_base *fb,string *src):
FXDialogBox(owner,"command",DECOR_TITLE|DECOR_BORDER)
{

this->src=src;
this->fb=fb;
FXObject *tgt=(FXObject*)owner;
contents=new FXVerticalFrame(this,LAYOUT_SIDE_TOP|FRAME_NONE|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH);

buttons=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y|PACK_UNIFORM_WIDTH); 
ok=new FXButton (buttons,"OK",NULL,owner,ID_COMMAND, BUTTON_INITIAL|FRAME_RAISED,0,0,0,0,0,0,0,0);
cancel=new FXButton (buttons,"CANCEL",NULL,tgt,ID_CANCEL_COMMAND,FRAME_RAISED,0,0,0,0,0,0,0,0);

					
}






cmddialog::~cmddialog()
{
fxmessage("DO PIACHU\n");
delete contents;
delete buttons;
}
