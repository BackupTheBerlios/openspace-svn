#ifndef FirstRun_H
#define FirstRun_H


#include <string>

using namespace std;

class FirstRun:public FXDialogBox
{
  FXDECLARE (FirstRun) protected:

  private:
    FirstRun ()
    {
    }

  public:
  
  

    enum
    {
	ID_CONF_MANUAL = FXDialogBox::ID_LAST,
	ID_CONF_SEMI,
	ID_CONF_AUTOMATIC,
	ID_LAST,
    };

    FirstRun (FXWindow * owner);
 
    long FirstRun::onConfigure(FXObject * sender, FXSelector sel, void *);
    
    virtual ~ FirstRun ();
};

#endif
