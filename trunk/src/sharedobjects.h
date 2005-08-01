#ifndef sharedobjects
#define sharedobjects

#include "configure.h"
#include "fx.h"

extern configure *conf;


//some global functions ;p
string ntos (unsigned long k);
string numtostring (unsigned long k);
FXColor readcolor (string col);
FXColor readcolor2 (string col);

class objectmanager 
{
  public:
      static objectmanager* instance(FXApp *app);
       FXFont *captionfont, *captionfont1, *captionfont2;
       FXApp *app;
      
      
      
  protected:
      objectmanager(FXApp *app);
      objectmanager(const objectmanager&);
      objectmanager& operator= (const objectmanager&);
  private:
      static objectmanager* pinstance;
};




#endif
