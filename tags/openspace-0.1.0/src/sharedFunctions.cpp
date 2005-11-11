#include "sharedFunctions.h"

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

string mime2xml(string str)
{
	for(int i=0;str[i]!= '\0';i++)
	{
	 	if(str[i]=='+')
		str[i]='P';
	}


return str;
}

string xml2mime(string str)
{
	for(int i=0;str[i]!= '\0';i++)
	{
	 	if(str[i]=='P')
		str[i]='+';
	}


return str;
}
