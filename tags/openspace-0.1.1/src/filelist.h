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
class filelist:public FXIconList
{


FXDECLARE (filelist) 
  
private:  

    
    string previous_path;
    string type;
    pathtype *pt;
    
    FXWindow *notifyparent;
    FXLabel *label;
    FXLabel *info;
    FXMenuPane *popupmenu;
    FXMenuPane *popupwindow;   
    FXTextField *textfield;
    FXOptionMenu *sortmenu;
    FXPopup *sortpop;
    FXHorizontalFrame *bottomframe;
    
    vector < string > commands_tab;
    vector < string > button_commands_tab;
    vector < FXIcon * >icon_vec;
    vector < string > header_vec;
    vector < string > vector_name;
    vector < int >vector_type;
    vector < int >vector_width;
    

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
    
    FXDragAction dropaction;	// Drop action
    FXString dragfiles;		// Dragged files

    void dropData (bool clipboard);
    void filelist::selectitem (void);
    void filelist::start_thread (thread_elem * te);
    
    void filelist::copymoveremove (string com_name);
    static void *filelist::thread_func (void *data);
    static FXint cmp (const FXIconItem * a, const FXIconItem * b);
    
    
    string returnpath(string dirname);
    string filelist::getdefaultcommand (string name);
    string filelist::resolvecommand(string command,string name);
    int filelist::runCommand(string name);
    static string filelist::getfiletype (string name);
    long filelist::setKeys (void);
    
public:  

     filelist ()
    {
    }
    virtual ~ filelist ();

//==============================================
    filelist (FXComposite * p, pathtype pt);
//==============================================


objectmanager *objmanager;
FXIcon **specialicons;
FXToolBar *toolbar,*toolbar2;

string path;
bool active;
filelist_base * fb;
filelist *filelist_opposite;

enum
    {
	ID_ICO = FXIconList::ID_LAST,
	ID_TEXTFIELD_REG,
	ID_TEXTFIELD_GO,
	ID_TEXTFIELD_GET,
	ID_SORT_CHANGE,
	ID_SELECT_ALL,
	ID_REMOVE,
	ID_REFRESH,
	ID_MAXIMIZE,
	ID_CLIP_COPY,
	ID_CLIP_CUT,
	ID_CLIP_PASTE,
	ID_CHANGE_VIEW_SMALL,
	ID_CHANGE_VIEW_BIG,
	ID_CHANGE_VIEW_DETAILS,
	ID_HOME,
	ID_PARENTDIR,
	ID_LAST,
    };


    virtual void create ();

    void filelist::refresh (void);
    bool filelist::init (void);
    bool filelist::opendir (string dir);
       
    long filelist::setFocus (FXObject * obj, FXSelector sel, void *ptr);
    long filelist::lostFocus (FXObject * obj, FXSelector sel, void *ptr);
    long filelist::openfile (FXObject *, FXSelector, void *);
    long filelist::gotoparentdir (FXObject *, FXSelector, void *);
    long filelist::onPopup (FXObject *, FXSelector, void *);
    long filelist::file_operation (FXObject *, FXSelector, void *);
    long filelist::onCmdHeader (FXObject *, FXSelector, void *ptr);
    long filelist::onCommand (FXObject *, FXSelector, void *ptr);
    long filelist::onCommandCancel (FXObject *, FXSelector, void *ptr);
    long filelist::click (FXObject *, FXSelector, void *ptr);
    long filelist::parseTextField (FXObject * sender, FXSelector, void *);
    long filelist::keyPress (FXObject * sender, FXSelector, void *);
    long filelist::onMaximize (FXObject * sender, FXSelector, void *);
    long filelist::onGoHome (FXObject * sender, FXSelector, void *);    
    long filelist::onCmdResize(FXObject * sender, FXSelector, void *);
    long onChangeView (FXObject * sender, FXSelector, void *);
    
    long onDNDEnter (FXObject *, FXSelector, void *);
    long onDNDLeave (FXObject *, FXSelector, void *);
    long onDNDMotion (FXObject *, FXSelector, void *);
    long onDNDDrop (FXObject *, FXSelector, void *);
    long onDNDRequest (FXObject *, FXSelector, void *);
    long onBeginDrag (FXObject *, FXSelector, void *);
    long onEndDrag (FXObject *, FXSelector, void *);
    long onDragged (FXObject *, FXSelector, void *);

    long onClipboardLost (FXObject *, FXSelector, void *);
    long onClipboardGained (FXObject *, FXSelector, void *);
    long onClipboardRequest (FXObject *, FXSelector, void *);

    long onCmdCopySel (FXObject *, FXSelector, void *);
    long onCmdPasteSel (FXObject *, FXSelector, void *);


    
    

};


#endif
