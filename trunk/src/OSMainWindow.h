#ifndef MAINWINDOW
#define MAINWINDOW

#include <map>
using namespace std;

#include "OSPathType.h"
#include "OSFileList.h"
#include "OSFileTypeSymbol.h"


#ifdef WIN32
#define SEPARATOR "\\"
#else
#include <dlfcn.h>
#define SEPARATOR "/"
#endif


//--------------------------------------------------------------------------------------------------------------------------------------------------    

class box;
class Frame;


//the main application window

class OSMainWindow:public FXMainWindow
{
  FXDECLARE (OSMainWindow) public:
    enum
    {
	ID_DIR = FXMainWindow::ID_LAST,
	ID_DIRCH,
	ID_TOLEFT,
	ID_TOCLOSE,
	ID_TORIGHT,
	ID_NEWFRAME,
	ID_TIMER,
	ID_CANCEL,
	ID_COMMANDS_SHOW,
	ID_NEW_NETWORK,
	ID_OVERWRITE,
	ID_OVERWRITE_ALL,
	ID_SKIP,
	ID_SKIP_ALL,
	ID_ABOUT,
	ID_CONFIGURE,
	ID_NEW_SEARCH,
	ID_LAST,
    };

    OSMainWindow (FXApp *);
    virtual ~ OSMainWindow ();
    virtual void create ();



    long OSMainWindow::onListDirs (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onChangeDir (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onChangeList (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onNewFrame (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onNewNetworkFrame (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onNewSearchFrame (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onNotify (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onOpenDir (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onTimer (FXObject *, FXSelector, void *);
    long OSMainWindow::cancel (FXObject * sender, FXSelector, void *);
    long OSMainWindow::update (FXObject * sender, FXSelector, void *);
    long OSMainWindow::commandsShow (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onListNextDir (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onOverwrite (FXObject * sender, FXSelector, void *);
    long OSMainWindow::onAbout (FXObject * sender, FXSelector sel, void *);
    long OSMainWindow::onOpenConfigure (FXObject * sender, FXSelector sel, void *);
    long OSMainWindow::onUpdate (FXObject * sender, FXSelector sel, void *);
    long OSMainWindow::onConfigure (FXObject * sender, FXSelector sel, void *);

    int OSMainWindow::popupDir (OSFileList * current_filelist, string path, int x, int y);

    string OSMainWindow::parseDir (string dir);
    bool OSMainWindow::loadMimeSettings (string path, string type);



  private:
     OSMainWindow ()
    {
    }
    void OSMainWindow::loadicons (string dir);

    FXVerticalFrame *leftframe;
    FXVerticalFrame *rightframe;
    FXVerticalFrame *infoframe;
    FXVerticalFrame *controlframe;

    FXVerticalFrame *rightcontrolframe;
    FXVerticalFrame *leftcontrolframe;
    FXVerticalFrame *left;
    FXVerticalFrame *right;
    FXHorizontalFrame *networkframe, *searchframe;
    FXTextField *search_path,*search_filefilter,*search_size_greater,*search_size_less;

    FXSplitter *splitter;
    FXMenuPane *filemenu;

    FXTextField *server;
    FXTextField *user;
    FXTextField *password;
    FXTextField *port;

    Frame *fr;

    Frame *current_frame;
    Frame *left_frame;
    Frame *right_frame;
    FXMenuPane *pane;
    OSPreferences *pref;
    float ratio;


    FXToolBarShell *dragshell1;	// Shell for floating menubar
    FXToolBarShell *dragshell2;	// Shell for floating toolbar
    FXToolBarShell *dragshell3;	// Shell for floating toolbar
    FXToolBarShell *dragshell4;	// Shell for floating toolbar
    FXDockSite *topdock;
    FXDockSite *bottomdock;
    FXDockSite *leftdock;
    FXDockSite *rightdock;
    
    FXComboBox *filelisttypecombobox;
    
OSObjectManager *objmanager;
};






//--------------------------------------------------------------------------------------------------------------------------------------------------

class Frame;

class box
{

  public:

    Frame * fr;
    FXButton *prebutton;
    FXButton *bt;
    FXButton *nextbutton;


     box (Frame * fr, FXButton * prebutton, FXButton * bt, FXButton * nextbutton)
    {
	this->bt = bt;
	this->fr = fr;
	this->prebutton = prebutton;
	this->nextbutton = nextbutton;
    }

};


class Frame
{
  private:
	
  public:

    string pathdir;
    string type;
    OSFileList *f;
    FXButton *toleft;
    FXButton *toright;
    FXButton *toclose;
    FXButton *firstbutton;
    
    FXVerticalFrame * frame;
    FXHorizontalFrame *hf;

    Frame (FXComposite * cp, FXComposite * p, OSPathType pt, FXObject * tgt, int position);
    void generate_menu (string path, FXObject * tgt);

    ~Frame ()
    {
	delete hf;
	delete f;
	delete frame;
    }



};





#endif
