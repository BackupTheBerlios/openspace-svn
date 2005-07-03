#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "sharedobjects.h"

#include <vector>
#include <string>
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
FXTextField *textfield;
FXPacker *vv;
FXCheckButton* rescancheck;
FXCheckButton* capturecheck;
FXVerticalFrame *frame;

};



class preferences : public FXDialogBox {
  FXDECLARE(preferences)
protected:
 
private:
  preferences(){}

public:

FXTextField *mainwindow_width;
FXTextField *mainwindow_height;

FXTextField *newcommandedit;

FXPopup* commandspop;
FXOptionMenu *commandsmenu;
FXVerticalFrame* commandspane;
FXVerticalFrame* staticcommandpane;
FXVerticalFrame* commandsmainpane;

FXPopup* filetypepop;
FXOptionMenu *filetypemenu;
FXComboBox *filetypedefaultbox;
string filetypestring;






vector<command_container*> command_vec;
vector<filetype_container*> filetype_vec;

FXVerticalFrame* filetypepane;
enum
{
ID_SAVE=FXDialogBox::ID_LAST,
ID_COMMAND_CHANGE,
ID_FILETYPE_CHANGE,
ID_NEW_COMMAND,
ID_NEW_FILETYPE,
ID_REMOVE_COMMAND,
ID_LAST,
};

  preferences(FXWindow *owner);
  
	long preferences::onSave(FXObject * sender,FXSelector sel,void*);
	long preferences::onCommandChange(FXObject * sender,FXSelector sel,void*);
	long preferences::onFileTypeChange(FXObject * sender,FXSelector sel,void*);
	long preferences::onNewCommand(FXObject * sender,FXSelector sel,void*);
	long preferences::onRemoveCommand(FXObject * sender,FXSelector sel,void*);
	
  virtual ~preferences();
  };

#endif
