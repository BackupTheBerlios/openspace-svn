#include <fx.h>
#include "../../cmddialog.h"

#include <string>
using namespace std;

#ifdef WIN32
  #define EXPORTFUNCTION extern "C" __declspec(dllexport)
#else
   #define EXPORTFUNCTION extern "C"
#endif


 #ifdef WIN32
   #define SEPARATOR "\\"
 #else
#include <dlfcn.h>
   #define SEPARATOR "/"
 #endif

class rename_cmddialog : public cmddialog
{
FXDECLARE(rename_cmddialog)
protected:
 
 rename_cmddialog(const rename_cmddialog&){}
	 
public:




enum
{
ID_ENTER=cmddialog::ID_LAST,	
};


vector<FXTextField*> vec;
string dir;

rename_cmddialog(){}
rename_cmddialog(FXWindow* w,filelist_base *fb,string *src);


 virtual int rename_cmddialog::exec(void)
 {
	int error=0;
		int i=0;
		while(src[i]!="")
		{
		string newname=dir+SEPARATOR+vec[i]->getText().text();
			if(fb->rename(src[i],newname)==false)
			{
			error=-1;
			string err="can't rename " + src[i] + " to " + newname;
			
			FXLabel *lab=new FXLabel(contents,err.c_str());
			lab->create();
			this->layout();
			this->resize(this->getWidth(),this->getHeight()+lab->getHeight());
			
			}
		//fxmessage(vec[i]->getText().text());
		i++;
		}
 
// for (indx = 0; indx < vec.size(); indx++)
return error;
 
 }

	
long  rename_cmddialog::press(FXObject * sender,FXSelector,void*);

};  

    FXDEFMAP(rename_cmddialog) rename_cmddialogMap[]=
     {	  
	    FXMAPFUNC(SEL_COMMAND, rename_cmddialog::ID_ENTER,  rename_cmddialog::press),
	
     };
 FXIMPLEMENT(rename_cmddialog,cmddialog,rename_cmddialogMap,ARRAYNUMBER(rename_cmddialogMap))
 
rename_cmddialog::rename_cmddialog(FXWindow *w,filelist_base *fb,string *src):
        cmddialog(w,fb,src)
{

int i=0;

		while(src[i]!="")
		{
		
		dir=FXFile::directory(src[i].c_str()).text();
		//fxmessage(dir.c_str());
		//fxmessage("\n	woho");
		FXString name=FXFile::name(src[i].c_str());
		new FXLabel (contents, name);
		FXTextField *text=new FXTextField (contents,25,this,ID_ENTER);
		text->setFocus();
		text->setText(name);
		vec.push_back(text);
		i++;
		}



}
 
 long  rename_cmddialog::press(FXObject * sender,FXSelector,void*)
 {
	
 FXObject* target=(FXObject*)ok->getTarget();
 target->handle(ok,FXSEL(SEL_COMMAND,ID_COMMAND),NULL);
	 
 }
 

 

 
EXPORTFUNCTION cmddialog *get_cmddialog ( FXWindow*w,filelist_base *fb,string *src) { return new rename_cmddialog(w,fb,src); }
