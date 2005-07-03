#include <fx.h>
#include "../../cmddialog.h"

#include <string>
using namespace std;

#ifdef WIN32
  #define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
   #define EXPORTFUNCTION extern "C"
#endif


class special_cmddialog : public cmddialog
{
FXDECLARE(special_cmddialog)
protected:
 
 special_cmddialog(const special_cmddialog&){}
	 
public:

enum
{
ID_ENTER=cmddialog::ID_LAST,		
};

FXTextField *text;
special_cmddialog(){}
special_cmddialog(FXWindow* w,filelist_base *fb,string *src);

 virtual int special_cmddialog::exec(void)
 {
 string s=text->getText().text();
 fb->mkdir(s,0);
 }

	
long  special_cmddialog::press(FXObject * sender,FXSelector,void*);

};  

    FXDEFMAP(special_cmddialog) special_cmddialogMap[]=
     {	  
	    FXMAPFUNC(SEL_COMMAND, special_cmddialog::ID_ENTER,  special_cmddialog::press),
	
     };
 FXIMPLEMENT(special_cmddialog,cmddialog,special_cmddialogMap,ARRAYNUMBER(special_cmddialogMap))
 
special_cmddialog::special_cmddialog(FXWindow *w,filelist_base *fb,string *src):
        cmddialog(w,fb,src)
{


text=new FXTextField (contents,25,this,ID_ENTER);	
text->setFocus();

}
 
 long  special_cmddialog::press(FXObject * sender,FXSelector,void*)
 {

 FXObject* target=(FXObject*)ok->getTarget();
 target->handle(ok,FXSEL(SEL_COMMAND,ID_COMMAND),NULL);
	 
 }
 

 

 
EXPORTFUNCTION cmddialog *get_cmddialog ( FXWindow*w,filelist_base *fb,string *src) { return new special_cmddialog(w,fb,src); }
