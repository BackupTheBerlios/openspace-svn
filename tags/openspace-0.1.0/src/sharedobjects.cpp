#include "sharedobjects.h"

objectmanager* objectmanager::pinstance = 0;// initialize pointer

objectmanager* objectmanager::instance (FXApp *app) 
{
    if (pinstance == 0)  // is it the first call?
    {  
        pinstance = new objectmanager(app); // create sole instance

    }
    return pinstance; // address of sole instance
}
objectmanager::objectmanager(FXApp *app) 
{ 
    //... perform necessary instance initializations 
    
    captionfont = new FXFont (app, "arial", 9, FONTWEIGHT_BOLD);
    captionfont->create ();

    captionfont1 = new FXFont (app, "times", 8);
    captionfont1->create ();

    captionfont2 = new FXFont (app, "arial", 9);
    captionfont2->create ();
    
}


