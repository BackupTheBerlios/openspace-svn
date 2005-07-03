#include <fx.h>
#include <string>
using namespace std;
#include "informationpanel.h"


informationpanel::informationpanel(FXComposite *p, string command)
{

frame=new FXVerticalFrame(p,LAYOUT_FILL_X|FRAME_THICK,0,0,0,0,0,0);


lab0=new FXLabel(frame,"NONE");	

	if(command=="copy" || command=="move")
	{				
	lab1=new FXLabel(frame,"NONE");					
	lab2=new FXLabel(frame,"NONE");						
	pbar=new FXProgressBar(frame,NULL,0,LAYOUT_FILL_X|FRAME_THICK|PROGRESSBAR_PERCENTAGE);
	pbar2=new FXProgressBar(frame,NULL,0,LAYOUT_FILL_X|FRAME_THICK|PROGRESSBAR_PERCENTAGE);
	bu=new FXButton(frame, "CANCEL", NULL,NULL,999,BUTTON_TOOLBAR);
	bu->setBackColor(FXRGB(145,134,201));
	pbar->setBarColor(FXRGB(145,134,201));
	pbar2->setBarColor(FXRGB(145,134,201));
	}
	else if(command=="remove")
	{				
	lab1=new FXLabel(frame,"NONE");					
	bu=new FXButton(frame, "CANCEL", NULL,NULL,999,BUTTON_TOOLBAR);
	bu->setBackColor(FXRGB(145,134,201));
	}
frame->create();
frame->recalc();
}

informationpanel::~informationpanel()
{

delete frame;

}
