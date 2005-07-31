#include "sharedobjects.h"

objectmanager* objectmanager::pinstance = 0;// initialize pointer

objectmanager* objectmanager::instance () 
{
    if (pinstance == 0)  // is it the first call?
    {  
        pinstance = new objectmanager; // create sole instance
    }
    return pinstance; // address of sole instance
}
objectmanager::objectmanager() 
{ 
    //... perform necessary instance initializations 
}


