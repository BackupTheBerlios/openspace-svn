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
    
    //captionfont = new FXFont (app, "arial", 9, FONTWEIGHT_BOLD);
    captionfont = new FXFont(app,conf->readonestring ("/OpenspaceConfig/fonts/captionfont").c_str());
    captionfont->create ();

    //captionfont1 = new FXFont (app, "times", 8);
    captionfont1 = new FXFont(app,conf->readonestring ("/OpenspaceConfig/fonts/captionfont1").c_str());
    captionfont1->create ();

    //captionfont2 = new FXFont (app, "arial", 9);
    captionfont2 = new FXFont(app,conf->readonestring ("/OpenspaceConfig/fonts/captionfont2").c_str());
    captionfont2->create ();
    
}


