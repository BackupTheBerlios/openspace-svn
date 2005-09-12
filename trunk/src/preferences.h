#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "sharedobjects.h"
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


//preferences window, this is one of the most important things to be extended
class preferences:public FXDialogBox
{
  FXDECLARE (preferences) protected:

  private:
    preferences ()
    {
    }

  

    bool saveconfiguration;
    FXTextField * mainwindow_width;
    FXTextField *mainwindow_height;
    FXTextField *leftdir,*rightdir,*defaultdir;
    FXTextField *newCommandEdit;
    FXListBox *commandsCombo;
    FXListBox *iconsList,*iconsList2;
    FXVerticalFrame *commandsPane;
    FXTextField *commandsTextfield;
    FXTextField *commandsTextfieldText;
    FXCheckButton *commandsRescan;
    FXCheckButton *commandsCapture;
    FXLabel *commandsType;
    FXLabel *commandsIcon;
    
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
    FXListBox *fileTypeList;
    FXListBox *fileTypeDefaultBox;
    string filetypestring;

    MimeApp *mimeapp;
    FXColorDialog *colordlg;

    FXListBox *commandPluginsList,*availableCommandPluginsList;
    FXList*buttonsList;

    map <string, command_container > commandsMap;
    map <string, filetype_container> filetypesMap;
    vector <shutter_container> shutterVector;

    FXVerticalFrame *filetypePane;
    
    public:
    
    enum
    {
	ID_SAVE = FXDialogBox::ID_LAST,
	ID_COMMAND_CHANGE,
	ID_FILETYPE_CHANGE,
	ID_NEW_COMMAND,
	ID_NEW_FILETYPE,
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
	ID_SHUTTER_CHANGE,
	ID_DOWNLOAD_INSTALL_CMD_PLUGIN,
	ID_UPDATE_CMD_PLUGIN_LIST,
	ID_LAST,
    };

    preferences (FXWindow * owner);

    long preferences::save (void);
    long preferences::onCommandChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onFileTypeChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onShutterChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onNewCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onRemoveCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onOpenMimeApp (FXObject * sender, FXSelector sel, void *);
    long preferences:: onAdditionalCommandChange( FXObject * sender, FXSelector sel, void *);
    long preferences::onAddFiletype (FXObject * sender, FXSelector sel, void *);
    long preferences::onChooseColor (FXObject * sender, FXSelector sel, void *);
    long preferences::onAddButtonCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onAddShutterCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::close (FXObject * sender, FXSelector sel, void *);
    long preferences::downloadInstallCommandPlugin (FXObject * sender, FXSelector sel, void *);
    long preferences::updateCommandPluginList (FXObject * sender, FXSelector sel, void *);
    
    void preferences::setAllColor(FXButton* button,FXColor color);
    
    virtual ~ preferences ();
};

#endif
