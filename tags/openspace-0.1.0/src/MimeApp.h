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

 

  
   
    FXComboBox *programsbox;
    FXLabel *mime_label;  
  
    vector <string> mime_vector;
    vector <string>::iterator iter;
 public:
    enum
    {	 
	ID_NEXT=FXDialogBox::ID_LAST,
	ID_PREVIOUS,
	ID_LAST,
    };

    MimeApp (FXWindow * owner);

    long MimeApp::onNextPrevious(FXObject * sender, FXSelector sel, void *);
    long MimeApp::onClose (FXObject * sender, FXSelector sel, void *ptr);
    
    void MimeApp::fill(string tmp);
    static void MimeApp::save(string mime, string program);
    void MimeApp::doAutomaticConfiguration(void);
    
    

    virtual ~ MimeApp ();
};

#endif
