#ifndef INFORMATIONPANEL
#define INFORMATIONPANEL


class informationpanel
{

public:

	FXVerticalFrame * frame;
	FXLabel *lab0;
	FXLabel *lab1;
	FXLabel *lab2;
	FXProgressBar *pbar;
	FXProgressBar *pbar2; 
	FXButton * bu;
	
informationpanel::informationpanel(FXComposite *p, string command);
informationpanel::~informationpanel(void);

};

#endif
