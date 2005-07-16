#ifndef osListItem
#define osListItem

#include "osfile.h"

//added some features to standard fxiconitem
//for example new big icon view
class os_ListItem : public FXIconItem 
{
FXDECLARE(os_ListItem)
private:

protected:

void drawDetails(const FXIconList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
void drawBigIcon(const FXIconList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
FXint hitItem(const FXIconList* list,FXint rx,FXint ry,FXint rw,FXint rh) const;
FXint getHeight(const FXIconList* list) const;
 
/*
  // Overrides the base class version of draw()
 
  virtual void drawMiniIcon(const FXIconList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
  virtual void drawBigIcon(const FXIconList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const;
  virtual FXint hitItem(const FXIconList* list,FXint rx,FXint ry,FXint rw=1,FXint rh=1) const;
*/
public:
  // Constructor
  os_ListItem(FXIconList* list,osfile osf,FXColor col,FXColor backcol,FXIcon* ic1=NULL,FXIcon* ic2=NULL);
  os_ListItem(){}

osfile osf;	  
FXColor color;
FXColor backcolor;
FXIconList *list;

};

#endif
