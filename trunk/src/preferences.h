#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "sharedobjects.h"
#include "MimeApp.h"
#include "CommandsFileTypesContainers.h"


#include <vector>
#include <string>
#include <map>
using namespace std;




//preferences window, this is one of the most important things to be extended
class preferences:public FXDialogBox
{
  FXDECLARE (preferences) protected:

  private:
    preferences ()
    {
    }

  public:

     FXTextField * mainwindow_width;
    FXTextField *mainwindow_height;

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
    
    FXList *additionalCommands,*additionalCommandsAvailable;
    
    string currentCommandName;
    string currentFileType;
    objectmanager* objmanager;
    
    FXVerticalFrame *frame;
    
    FXComboBox *allMime;
    FXButton *colorbutton;
    FXButton *backcolorbutton;
    

    FXListBox *fileTypeList;
    FXListBox *fileTypeDefaultBox;
    string filetypestring;

    MimeApp *mimeapp;
    FXColorDialog *colordlg;

    FXListBox *commandPluginsList;

    map <string, command_container > commandsMap;
    map <string, filetype_container> filetypesMap;

    FXVerticalFrame *filetypePane;
    enum
    {
	ID_SAVE = FXDialogBox::ID_LAST,
	ID_COMMAND_CHANGE,
	ID_FILETYPE_CHANGE,
	ID_NEW_COMMAND,
	ID_NEW_FILETYPE,
	ID_REMOVE_COMMAND,
	ID_MIME_APP,
	ID_ADD_COMMAND_ADDITIONAL,
	ID_DEL_COMMAND_ADDITIONAL,
	ID_ADD_FILETYPE,
	ID_CHOOSE_COLOR,
	ID_CHOOSE_BACKCOLOR,
	ID_COMMANDPLUGIN_CHANGE,
	ID_LAST,
    };

    preferences (FXWindow * owner);

    long preferences::onSave (FXObject * sender, FXSelector sel, void *);
    long preferences::onCommandChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onFileTypeChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onNewCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onRemoveCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onOpenMimeApp (FXObject * sender, FXSelector sel, void *);
    long preferences:: onAdditionalCommandChange( FXObject * sender, FXSelector sel, void *);
    long preferences::onAddFiletype (FXObject * sender, FXSelector sel, void *);
    long preferences::onChooseColor (FXObject * sender, FXSelector sel, void *);
    void preferences::setAllColor(FXButton* button,FXColor color);
    
    virtual ~ preferences ();
};

#endif
