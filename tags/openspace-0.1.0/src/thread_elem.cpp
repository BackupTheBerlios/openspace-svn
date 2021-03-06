#include "thread_elem.h"


thread_elem::thread_elem (void *fb, string command, string options, vector<string> src, string dst)
{
    

    this->command = command;
    this->src = src;
    this->dst = dst;
    this->fb = fb;
    this->options = options;

    init(); 

}

thread_elem::thread_elem (void *fb, string command, string options, string src, string dst)
{
    
    vector<string> src2;
    src2.push_back(src);
    
    this->command = command;
    this->src = src2;
    this->dst = dst;
    this->fb = fb;
    this->options = options;

    init(); 

}


thread_elem::thread_elem ()
{
    gui = NULL;
}

void thread_elem::init(void)
{
    str1 = "start";
    gui = NULL;
    
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
