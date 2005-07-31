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

//number to string in human readable units
string numtostring (unsigned long k)
{


    string metric ("B");
    if (k > 1024)
    {
	k /= 1024;
	metric = "KB";
	if (k > 1024)
	{
	    k /= 1024;
	    metric = "MB";
	}
    }

    char chstr[20];
    sprintf (chstr, "%d", k);

    string retstrin = string (chstr) + " " + metric;

    return retstrin;
}

//number to string
string ntos (unsigned long k)
{
    char chstr[20];
    sprintf (chstr, "%d", k);
    return chstr;
}

