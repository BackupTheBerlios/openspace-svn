#ifndef FILELIST
#define FILELIST

#include "filelist_base.h"
#include "pathtype.h"
#include "thread_elem.h"
#include "filetype.h"
#include "cmddialog.h"
#include <string>
#include <map>
#include "preferences.h"
using namespace std;


//filelist
class filelist : public FXIconList
{


 FXDECLARE(filelist)
public:


filelist_base *fb;
filelist *filelist_opposite;
bool active;
enum
{
ID_ICO=FXIconList::ID_LAST,
ID_TEXTFIELD_REG,
ID_TEXTFIELD_GO,
ID_TEXTFIELD_GET,
ID_SORT_CHANGE,
ID_SELECT_ALL,
ID_REMOVE,
ID_REFRESH,
ID_MAXIMIZE,
ID_LAST,
};


string path;
string type;
FXWindow *notifyparent;
FXLabel *label;
FXLabel *info;
FXMenuPane *popupmenu; 
FXMenuPane *popupwindow; 
string commands_tab[50]; //IMPORTANT
FXFont *captionfont;
FXFont *captionfont1;
FXTextField *textfield;
FXOptionMenu *sortmenu;
FXPopup*  sortpop;
vector<FXIcon*> icon_vec;
vector<string> header_vec;
FXHorizontalFrame *bottomframe;

vector<thread_elem*>* thread_vec;
map<string, file_type*>* file_type_settings; 
FXGIFIcon **osicons; 

vector<string>	vector_name;
vector<int>	vector_type;
vector<int>	vector_width;
pathtype *pt;

string *fields_name;
unsigned int *fields_type;
unsigned int *fields_width;
int sort_nr;
unsigned long thumb_size;

static bool ascend;
static bool strcase;

bool processing;
bool maximize;
int popup_x;
int popup_y;

cmddialog *dial;




FXDragAction  dropaction;     // Drop action
FXString      dragfiles;      // Dragged files



filelist(){}
virtual ~filelist();

//==============================================
filelist(
	FXComposite *p,
	pathtype pt,vector<thread_elem*>* thread_vec,
	map<string, file_type*> *file_type_settings,
	FXGIFIcon **specialicons
);
//==============================================

virtual void create();

void    filelist::opendir(string dir);
long	filelist::setFocus(FXObject * obj ,FXSelector sel ,void* ptr);	
long	filelist::lostFocus(FXObject * obj ,FXSelector sel ,void* ptr);	
long    filelist::openfile(FXObject *,FXSelector,void*);
long    filelist::gotoparentdir(FXObject *,FXSelector,void*);
long    filelist::onPopup(FXObject *,FXSelector,void*);
long    filelist::file_operation(FXObject *,FXSelector,void*);
long    filelist::onCmdHeader(FXObject*,FXSelector,void* ptr);
long    filelist::onCommand(FXObject*,FXSelector,void* ptr);
long    filelist::onCommandCancel(FXObject*,FXSelector,void* ptr);
long 	filelist::click(FXObject*,FXSelector,void* ptr);
long    filelist::parseTextField(FXObject * sender,FXSelector,void*);
long    filelist::keyPress(FXObject * sender,FXSelector,void*);
long    filelist::onMaximize(FXObject * sender,FXSelector,void*);

  long onDNDEnter(FXObject*,FXSelector,void*);
  long onDNDLeave(FXObject*,FXSelector,void*);
  long onDNDMotion(FXObject*,FXSelector,void*);
  long onDNDDrop(FXObject*,FXSelector,void*);
  long onDNDRequest(FXObject*,FXSelector,void*);
  long onBeginDrag(FXObject*,FXSelector,void*);
  long onEndDrag(FXObject*,FXSelector,void*);
  long onDragged(FXObject*,FXSelector,void*);

void filelist::selectitem(void);
void filelist::refresh(void);
void filelist::start_thread(thread_elem *te);
string filelist::getdefaultcommand(string name,bool resolve);
void filelist::copymoveremove(string com_name);
static void * filelist::thread_func(void * data);
static FXint cmp(const FXIconItem* a,const FXIconItem* b);
void filelist::init(void);


};


#endif
