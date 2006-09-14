#ifndef MAINWINDOW
#define MAINWINDOW

#include <string>
#include <vector>
#include <map>

#include "fx.h" 

#include "OSFrame.h"
#include "OSPathType.h"
#include "OSFileList.h"
#include "OSFileTypeSymbol.h"
#include "OSFileListController.h"
#include "OSPreferences.h"
using namespace std;


//the main application window

class OSMainWindow:public FXMainWindow, public OSFileListController
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

    long onListDirs (FXObject * sender, FXSelector, void *);
    long onChangeDir (FXObject * sender, FXSelector, void *);
    long onChangeList (FXObject * sender, FXSelector, void *);
    long onNewFrame (FXObject * sender, FXSelector, void *);
    long onNewNetworkFrame (FXObject * sender, FXSelector, void *);
    long onNewSearchFrame (FXObject * sender, FXSelector, void *);
    long onNotify (FXObject * sender, FXSelector, void *);
    long onOpenDir (FXObject * sender, FXSelector, void *);
    long onTimer (FXObject *, FXSelector, void *);
    long cancel (FXObject * sender, FXSelector, void *);
    long update (FXObject * sender, FXSelector, void *);
    long commandsShow (FXObject * sender, FXSelector, void *);
    long onListNextDir (FXObject * sender, FXSelector, void *);
    long onOverwrite (FXObject * sender, FXSelector, void *);
    long onAbout (FXObject * sender, FXSelector sel, void *);
    long onOpenConfigure (FXObject * sender, FXSelector sel, void *);
    long onUpdate (FXObject * sender, FXSelector sel, void *);
    long onConfigure (FXObject * sender, FXSelector sel, void *);


    void dirChange(long id);
    void getFocus(long id);
    void splitFileList(long id);
    void openVfs(long id,string file);

  private:
    OSMainWindow ()
    {
    }
    
    void loadicons (string dir);
    void loadSpecialIcon (string icondir, string type, int nr );
    int popupDir (OSFileList * current_filelist, string path, int x, int y);
    string parseDir (string dir);
    bool loadMimeSettings (string path, string type);
    
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

    OSFrame *current_frame;
    OSFrame *left_frame;
    OSFrame *right_frame;
    FXMenuPane *pane;
    OSPreferences *pref;
    float ratio;
    
    long lastId;


    FXToolBarShell *dragshell1;	
    FXToolBarShell *dragshell2;	
    FXToolBarShell *dragshell3;
    FXToolBarShell *dragshell4;
    FXDockSite *topdock;
    FXDockSite *bottomdock;
    FXDockSite *leftdock;
    FXDockSite *rightdock;
    
    FXComboBox *filelisttypecombobox;    
    OSObjectManager *objmanager;
    
    std::map <long, OSFrame*> OSFrameMap;
};


#endif
