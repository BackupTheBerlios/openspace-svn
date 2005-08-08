#ifndef MIMEAPP_H
#define MIMEAPP_H


#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class MimeApp:public FXDialogBox
{
  FXDECLARE (MimeApp) protected:

  private:
    MimeApp ()
    {
    }

  public:
  
    vector <string> mime_vector;
    vector <string>::iterator iter;
    FXComboBox *programsbox;
    FXLabel *mime_label;

    enum
    {
	ID_SAVE = FXDialogBox::ID_LAST,
	ID_NEXT,
	ID_PREVIOUS,
	ID_LAST,
    };

    MimeApp (FXWindow * owner);

    long MimeApp::onSave (FXObject * sender, FXSelector sel, void *);
    long MimeApp::onNextPrevious(FXObject * sender, FXSelector sel, void *);
    
    void MimeApp::fill(string tmp);
    static void MimeApp::save(string mime, string program);
    

    virtual ~ MimeApp ();
};

#endif
