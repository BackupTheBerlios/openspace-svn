#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "sharedobjects.h"
#include "MimeApp.h"

#include <vector>
#include <string>
#include <map>
using namespace std;



class filetype_container
{
  public:
    string name;
    string command;
};

class command_container
{
  public:
    string name;
    string exec;
    bool rescan;
    bool capture;
    string text;
    string type;
    string icon;
    
    

};


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
    //FXScrollPane *commandsPop;
   // FXOptionMenu *commandsMenu;
    FXListBox *commandsCombo;
    FXListBox *iconsList;
    FXVerticalFrame *commandsPane;
    FXTextField *commandsTextfield;
    FXTextField *commandsTextfieldText;
    FXCheckButton *commandsRescan;
    FXCheckButton *commandsCapture;
    FXLabel *commandsType;
    FXLabel *commandsIcon;
    
    string currentCommandName;
    objectmanager* objmanager;
    
    FXVerticalFrame *frame;
    
    
    

    FXPopup *filetypepop;
    FXOptionMenu *filetypemenu;
    FXComboBox *filetypedefaultbox;
    string filetypestring;

    MimeApp *mimeapp;




    map <string, command_container >commandsMap;
    vector < filetype_container * >filetype_vec;

    FXVerticalFrame *filetypepane;
    enum
    {
	ID_SAVE = FXDialogBox::ID_LAST,
	ID_COMMAND_CHANGE,
	ID_FILETYPE_CHANGE,
	ID_NEW_COMMAND,
	ID_NEW_FILETYPE,
	ID_REMOVE_COMMAND,
	ID_MIME_APP,
	ID_LAST,
    };

    preferences (FXWindow * owner);

    long preferences::onSave (FXObject * sender, FXSelector sel, void *);
    long preferences::onCommandChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onFileTypeChange (FXObject * sender, FXSelector sel, void *);
    long preferences::onNewCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onRemoveCommand (FXObject * sender, FXSelector sel, void *);
    long preferences::onOpenMimeApp (FXObject * sender, FXSelector sel, void *);

    virtual ~ preferences ();
};

#endif
