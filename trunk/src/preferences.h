#ifndef PREFERENCES_H
#define PREFERENCES_H
#include "sharedobjects.h"
#include "sharedFunctions.h"
#include "MimeApp.h"
#include "CommandsFileTypesContainers.h"
#include <vector>
#include <string>
#include <map>
using namespace std;
class shutter_container
{
public:
string shutter;
string command;
shutter_container(string s,string c):shutter(s),command(c){}
};

class toolbar_container
{
public:
string toolbar;
string command;
toolbar_container(string s,string c):toolbar(s),command(c){}
};

//preferences window, this is one of the most important things to be extended
class preferences:public FXDialogBox
{
  FXDECLARE (preferences) protected:

  private:
    preferences ()
    {
    }

    string actualvfs;

    bool saveconfiguration;
    FXTextField * mainwindow_width;
    FXTextField *mainwindow_height;
    FXTextField *leftdir,*rightdir,*defaultdir;
    FXTextField *font1,*font2,*font3;
    FXTextField *newCommandEdit;
    FXTextField *newShutterEdit;
    FXTextField *thumbsize;
    FXListBox *commandsCombo;
    FXListBox *shortcutList;	
    FXListBox *shortcutMaskList;
    FXListBox *iconsList,*iconsList2;
    FXVerticalFrame *commandsPane;
    FXTextField *commandsTextfield;
    FXTextField *commandsTextfieldText;
    FXCheckButton *commandsRescan;
    FXCheckButton *commandsCapture;
    FXLabel *commandsType;
    FXLabel *commandsIcon;
    FXListBox *iconsTheme;
    FXButton *removeCommandButton;
    FXVerticalFrame *vfsPane;
    FXListBox *vfsList;
    FXList*headersList,*availableHeadersList;
    
    FXList *additionalCommands,*additionalCommandsAvailable,*additionalCommandsAvailableForButtons;
    FXList *shutterCommands;
    
    string currentCommandName;
    string currentFileType;
    objectmanager* objmanager;
    
    FXVerticalFrame *frame;
    
    FXComboBox *allMime;
    FXButton *colorbutton;
    FXButton *backcolorbutton;
    
    FXListBox *shutterList;
    FXListBox *toolbarList;
    FXListBox *fileTypeList;
    FXListBox *fileTypeDefaultBox;
    string filetypestring;

    MimeApp *mimeapp;
    FXColorDialog *colordlg;

    FXListBox *commandPluginsList,*availableCommandPluginsList;
    FXListBox * availableVfsPluginsList;
    FXList*buttonsList;

    map <string, command_container > commandsMap;
    map <string, filetype_container> filetypesMap;   
  
    vector <shutter_container> shutterVector;
    vector <toolbar_container> toolbarVector;
    
    
    FXVerticalFrame *filetypePane;
    
    bool preferences::validateName(string name);
    void preferences::setAllColor(FXButton* button,FXColor color);

  FXColor base;
  FXColor border;
  FXColor back;
  FXColor fore;
  FXColor selback;
  FXColor selfore;
  FXColor tipback;
  FXColor tipfore;
  FXColor menuback;
  FXColor menufore;
  FXColor hilite;
  FXColor shadow;
  FXColor main;

  FXToolTip         *tooltip;
  FXTabBook         *tabbook;
  FXTabItem         *tabitem;
  FXVerticalFrame   *tabframe;
  FXVerticalFrame   *mainframe;
  FXVerticalFrame   *menuframe;
  FXHorizontalFrame *labeltextframe1;
  FXHorizontalFrame *labeltextframe2;
  FXHorizontalFrame *textframe1;
  FXHorizontalFrame *textframe2;
  FXHorizontalFrame *tabsubframe;
  FXGroupBox        *grpbox1;
  FXGroupBox        *grpbox2;
  FXLabel           *label1;
  FXLabel           *label2;
  FXLabel           *label3;
  FXLabel           *label4;
  FXLabel           *label5;
  FXLabel           *menulabels[6];
  FXTextField       *textfield1;
  FXButton          *button1;
  FXButton          *fontbutton,*fontbutton1,*fontbutton2,*fontbutton3;
  FXSeparator       *sep1;
  FXSeparator       *sep2;
  FXSeparator       *sep3;
 
 
private:

  FXFont           *font;

  FXuint            typingSpeed;
  FXuint            clickSpeed;
  FXuint            scrollSpeed;
  FXuint            scrollDelay;
  FXuint            blinkSpeed;
  FXuint            animSpeed;
  FXuint            menuPause;
  FXuint            tooltipPause;
  FXuint            tooltipTime;
  FXint             dragDelta;
  FXint             wheelLines;


private:

  FXDataTarget      target_base;
  FXDataTarget      target_back;
  FXDataTarget      target_border;
  FXDataTarget      target_fore;
  FXDataTarget      target_hilite;
  FXDataTarget      target_shadow;
  FXDataTarget      target_selfore;
  FXDataTarget      target_selback;
  FXDataTarget      target_tipfore;
  FXDataTarget      target_tipback;
  FXDataTarget      target_menufore;
  FXDataTarget      target_menuback;
    
  FXDataTarget      target_main;

  FXDataTarget      target_typingspeed;
  FXDataTarget      target_clickspeed;
  FXDataTarget      target_scrollspeed;
  FXDataTarget      target_scrolldelay;
  FXDataTarget      target_blinkspeed;
  FXDataTarget      target_animspeed;
  FXDataTarget      target_menupause;
  FXDataTarget      target_tooltippause;
  FXDataTarget      target_tooltiptime;
  FXDataTarget      target_dragdelta;
  FXDataTarget      target_wheellines;

    
    public:
    
    enum
    {
	ID_SAVE = FXDialogBox::ID_LAST,
	ID_COMMAND_CHANGE,
	ID_FILETYPE_CHANGE,
	ID_VFS_CHANGE,
	ID_NEW_COMMAND,
	ID_NEW_FILETYPE,
	ID_NEW_SHUTTER,
	ID_REMOVE_SHUTTER,
	ID_REMOVE_COMMAND,
	ID_MIME_APP,
	ID_MIME_APP_AUTO,
	ID_ADD_COMMAND_ADDITIONAL,
	ID_DEL_COMMAND_ADDITIONAL,
	ID_ADD_FILETYPE,
	ID_CHOOSE_COLOR,
	ID_CHOOSE_BACKCOLOR,
	ID_COMMANDPLUGIN_CHANGE,
	ID_ADD_BUTTON_COMMAND,
	ID_DEL_BUTTON_COMMAND,
	ID_ADD_SHUTTER_COMMAND,
	ID_DEL_SHUTTER_COMMAND,
	ID_ADD_HEADER,
	ID_DEL_HEADER,
	ID_SHUTTER_CHANGE,
	ID_TOOLBAR_CHANGE,
	ID_DOWNLOAD_INSTALL_CMD_PLUGIN,
	ID_DOWNLOAD_INSTALL_VFS_PLUGIN,
	ID_UPDATE_CMD_PLUGIN_LIST,
	ID_UPDATE_VFS_PLUGIN_LIST,
	ID_COLORS,
	ID_CHOOSE_FONT,
	ID_CHOOSE_CAPTIONFONT1,
	ID_CHOOSE_CAPTIONFONT2,
	ID_CHOOSE_CAPTIONFONT3,
	ID_LAST,
    };

    preferences (FXWindow * owner);

    long preferences::save (void);
    long preferences::onCommandChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onFileTypeChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onShutterChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onToolbarChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onNewCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onRemoveCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onNewShutter (FXObject * sender, FXSelector sel, void *);
    long preferences::onRemoveShutter (FXObject * sender, FXSelector sel, void *);
    long preferences::onOpenMimeApp (FXObject * sender, FXSelector sel, void *);
    long preferences::onAdditionalCommandChange( FXObject * sender, FXSelector sel, void *);
    long preferences::onAddFiletype (FXObject * sender, FXSelector sel, void *);
    long preferences::onChooseColor (FXObject * sender, FXSelector sel, void *);
    long preferences::onAddButtonCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onAddShutterCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::close (FXObject * sender, FXSelector sel, void *);
    long preferences::downloadInstallPlugin (FXObject * sender, FXSelector sel, void *);
    long preferences::updatePluginList (FXObject * sender, FXSelector sel, void *);
    long preferences::onVfsChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onAddHeader (FXObject * sender, FXSelector sel, void *);
    long preferences::onColorChanged(FXObject*,FXSelector,void*);
    long preferences::onChooseFont(FXObject*,FXSelector,void*);
    
    void preferences::setupColors(void);
    void preferences::setupFont(int nr);
    virtual ~ preferences ();
};

#endif
