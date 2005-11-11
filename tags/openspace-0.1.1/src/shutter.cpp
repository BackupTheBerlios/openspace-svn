#include "fx.h"
#include "shutter.h"


FXDEFMAP (shutter) shutterMap[] =
{
FXMAPFUNC (SEL_MOUSEWHEEL, 0, shutter::onMouseWheel), 
FXMAPFUNC (SEL_COMMAND, shutter::ID_OPEN_SHUTTERITEM, shutter::onOpenItem),};

FXIMPLEMENT (shutter, FXShutter, shutterMap, ARRAYNUMBER (shutterMap))
//-----SHUTTER------------------------------------------------------------------------------------------------------------------------------------------
shutter::shutter (FXComposite * p, FXWindow * x):
FXShutter (p, x, 6667, FRAME_NONE | LAYOUT_FILL_Y | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT | LAYOUT_FIX_WIDTH, 0, 0, 90, 0, 0, 0, 0, 0, 0, 0)
{
    setLayoutHints (FRAME_NONE | LAYOUT_FILL_Y | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT);

}


//----------------------------------------------------


long shutter::onMouseWheel (FXObject * sender, FXSelector, void *ptr)
{

    FXEvent *event = (FXEvent *) ptr;


    int x;
    if (event->code < 0)
    {
	x = getCurrent () + 1;
	if (x > numChildren () - 1)
	    x = 0;
	setCurrent (x);
    }

    if (event->code > 0)
    {
	x = getCurrent () - 1;
	if (x == -1)
	    x = numChildren () - 1;
	setCurrent (x);
    }


    FXMenuPane *menu = (FXMenuPane *) this->getParent ();
    menu->resize (90, getheight ());


    return 1;
}

//----------------------------------------------------

long shutter::onOpenItem (FXObject * sender, FXSelector sel, void *ptr)
{
    FXShutter::onOpenItem (sender, sel, ptr);


    FXMenuPane *menu = (FXMenuPane *) ((FXWindow *) sender)->getParent ()->getParent ();
    menu->resize (90, getheight ());

    return 1;
}

int shutter::getheight ()
{
    int height_size = 0;
    FXShutterItem *test = (FXShutterItem *) this->getFirst ();
    int x = getCurrent ();
    int zz = 0;

    while (test)
    {
	if (zz == x)
	{
	    FXWindow *wind = test->getContent ()->getFirst ();

	    while (wind)
	    {
		height_size += wind->getDefaultHeight ();
		wind = wind->getNext ();
	    }

	}
	test = (FXShutterItem *) test->getNext ();
	zz++;
    }
    if (this->getFirst () != NULL)
	height_size += (((FXShutterItem *) this->getFirst ())->getButton ()->getDefaultHeight () + 2) * zz - 1;
    return height_size;
}


//-----SHUTTER------------------------------------------------------------------------------------------------------------------------------------------
