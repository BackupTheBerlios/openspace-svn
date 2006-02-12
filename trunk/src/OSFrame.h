#ifndef OSFRAME
#define OSFRAME

#include <string>
#include "fx.h" 

#include "OSFileList.h"

using namespace std;

class OSFrame;

class box
{

  public:

    OSFrame * fr;
    FXButton *prebutton;
    FXButton *bt;
    FXButton *nextbutton;
    

     box (OSFrame * fr, FXButton * prebutton, FXButton * bt, FXButton * nextbutton)
    {
	this->bt = bt;
	this->fr = fr;
	this->prebutton = prebutton;
	this->nextbutton = nextbutton;
    }

};


class OSFrame
{
  private:
	
	FXDockSite* dock1;
	FXDockSite* dock2;
  public:

    long id;
    string pathdir;
    string type;
    OSFileList *f;
    FXButton *toleft;
    FXButton *toright;
    FXButton *toclose;
    FXButton *firstbutton;
    
    FXVerticalFrame * frame;
    FXHorizontalFrame *hf;

    OSFrame ( FXComposite * cp, FXComposite * p, OSPathType pt, FXObject * tgt,FXDockSite* dock1,FXDockSite* dock2,long id,OSFileListController* controller);
    void generateMenu (string path, FXObject * tgt);
    void moveToFront(FXComposite * controlframeContainer,FXComposite * frameContainer,OSFrame * frameOpposite);
    void moveToBack(FXComposite * controlframeContainer);
    bool openDir(string dir);
    string getDir(void);
    
    ~OSFrame ();
    void create(void);




};

#endif
