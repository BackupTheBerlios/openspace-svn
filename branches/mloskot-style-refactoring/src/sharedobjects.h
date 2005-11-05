#ifndef sharedobjects
#define sharedobjects

#include "configure.h"
#include "fx.h"
#include "filetype.h"
#include "thread_elem.h"
#include <map>
using namespace std;

extern configure *conf;

FXColor readcolor (string col);
FXColor readcolor2 (string col);

class objectmanager 
{
  public:
      static objectmanager* instance(FXApp *app);
       FXFont *captionfont1, *captionfont2, *captionfont3;
       FXApp *app;
       vector < thread_elem * >thread_vec;
       map < string, file_type * >file_type_settings;
       map < string, FXIcon * >osicons;
       FXIcon *specialicons[30];
       map < string, unsigned int >key_map;
       FXColor maincolor;
      
      
      
  protected:
      objectmanager(FXApp *app);
      objectmanager(const objectmanager&);
      objectmanager& operator= (const objectmanager&);
  private:
      static objectmanager* pinstance;
};




#endif