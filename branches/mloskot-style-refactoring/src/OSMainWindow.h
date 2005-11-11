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

class MainWindow:public FXMainWindow
{
  FXDECLARE (MainWindow) public:
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

    MainWindow (FXApp *);
    virtual ~ MainWindow ();
    virtual void create ();



    long MainWindow::onListDirs (FXObject * sender, FXSelector, void *);
    long MainWindow::onChangeDir (FXObject * sender, FXSelector, void *);
    long MainWindow::onChangeList (FXObject * sender, FXSelector, void *);
    long MainWindow::onNewFrame (FXObject * sender, FXSelector, void *);
    long MainWindow::onNewNetworkFrame (FXObject * sender, FXSelector, void *);
    long MainWindow::onNewSearchFrame (FXObject * sender, FXSelector, void *);
    long MainWindow::onNotify (FXObject * sender, FXSelector, void *);
    long MainWindow::onOpenDir (FXObject * sender, FXSelector, void *);
    long MainWindow::onTimer (FXObject *, FXSelector, void *);
    long MainWindow::cancel (FXObject * sender, FXSelector, void *);
    long MainWindow::update (FXObject * sender, FXSelector, void *);
    long MainWindow::commandsShow (FXObject * sender, FXSelector, void *);
    long MainWindow::onListNextDir (FXObject * sender, FXSelector, void *);
    long MainWindow::onOverwrite (FXObject * sender, FXSelector, void *);
    long MainWindow::onAbout (FXObject * sender, FXSelector sel, void *);
    long MainWindow::onOpenConfigure (FXObject * sender, FXSelector sel, void *);
    long MainWindow::onUpdate (FXObject * sender, FXSelector sel, void *);
    long MainWindow::onConfigure (FXObject * sender, FXSelector sel, void *);

    int MainWindow::popupDir (filelist * current_filelist, string path, int x, int y);

    string MainWindow::parseDir (string dir);
    bool MainWindow::loadMimeSettings (string path, string type);



  private:
     MainWindow ()
    {
    }
    void MainWindow::loadicons (string dir);

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
    preferences *pref;
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
    
objectmanager *objmanager;
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
    filelist *f;
    FXButton *toleft;
    FXButton *toright;
    FXButton *toclose;
    FXButton *firstbutton;
    
    FXVerticalFrame * frame;
    FXHorizontalFrame *hf;

    Frame (FXComposite * cp, FXComposite * p, pathtype pt, FXObject * tgt, int position);
    void generate_menu (string path, FXObject * tgt);

    ~Frame ()
    {
	delete hf;
	delete f;
	delete frame;
    }



};





#endif
