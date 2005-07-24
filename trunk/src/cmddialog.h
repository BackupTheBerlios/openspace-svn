#ifndef CMDDIALOG
#define CMDDIALOG

#include "filelist_base.h"

class cmddialog:public FXDialogBox
{
  FXDECLARE (cmddialog) public:
    FXVerticalFrame * contents;
    FXHorizontalFrame *buttons;
  protected:
     cmddialog ()
    {
    }
  public:
     cmddialog (FXWindow * owner, filelist_base * fb = NULL, string * src = NULL);
    virtual int exec (void)
    {
    };


    virtual ~ cmddialog ();
    filelist_base *fb;
    string *src;
    FXButton *ok;
    FXButton *cancel;

    enum
    {
	ID_EXEC = FXDialogBox::ID_LAST + 666,
	ID_COMMAND,
	ID_CANCEL_COMMAND,
	ID_LAST,
    };

};
#endif
