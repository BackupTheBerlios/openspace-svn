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
      static objectmanager* instance();
  protected:
      objectmanager();
      objectmanager(const objectmanager&);
      objectmanager& operator= (const objectmanager&);
  private:
      static objectmanager* pinstance;
};




#endif
