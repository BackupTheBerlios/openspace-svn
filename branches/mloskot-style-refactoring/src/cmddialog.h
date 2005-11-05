#ifndef CMDDIALOG
#define CMDDIALOG

#include "filelist_base.h"

class cmddialog:public FXDialogBox
{
  FXDECLARE (cmddialog) 

    
  protected:
     cmddialog ()
    {
    }
    FXVerticalFrame * contents;
    FXHorizontalFrame *buttons;
    FXButton *ok;
    FXButton *cancel;
    filelist_base *fb;
    vector < string > src;
    
  public:
     cmddialog (FXWindow * owner, filelist_base * fb = NULL, vector < string > src=vector < string >());
    virtual int exec (void)
    {
    };

    virtual ~ cmddialog ();

    enum
    {
	ID_EXEC = FXDialogBox::ID_LAST + 666,
	ID_COMMAND,
	ID_CANCEL_COMMAND,
	ID_LAST,
    };

};
#endif
