#ifndef SHUTTER
#define SHUTTER

//this is used in filelist popup, this scrolling popup
class shutter:public FXShutter
{
    FXDECLARE (shutter) 
    friend class FXShutterItem;
  public:
     shutter (FXComposite * p, FXWindow * x);
     shutter ()
    {
    }
    long onMouseWheel (FXObject *, FXSelector, void *);
    long onOpenItem (FXObject *, FXSelector, void *);

    int getheight (void);

};


#endif
