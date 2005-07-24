#include "thread_elem.h"


thread_elem::thread_elem (void *fb, string command, string options, string * src, string dst)
{
    str1 = "start";
    gui = NULL;

    this->command = command;
    this->src = src;
    this->dst = dst;
    this->fb = fb;
    this->options = options;


    act_total_size = 0;
    total_size = 1;
    file_size = 1;
    act_file_size = 0;

    tim = new time_t;
    time (tim);

    end = false;
    question = false;
    answer = false;
    overwrite = false;
    cancel = false;
    all = false;

    act_file_name = "";

}

thread_elem::thread_elem ()
{
    gui = NULL;
}

//number to string in human readable units
string
numtostring (unsigned long k)
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
string
ntos (unsigned long k)
{
    char chstr[20];
    sprintf (chstr, "%d", k);
    return chstr;
}
