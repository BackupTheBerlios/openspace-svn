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
    error = false;

    act_file_name = "";
    msg="operation failed";

}

thread_elem::thread_elem ()
{
    gui = NULL;
}

