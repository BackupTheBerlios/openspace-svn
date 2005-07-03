#ifndef THREAD_ELEM
#define THREAD_ELEM

#include <fx.h>
#include <string>
#include <vector>
using namespace std;

#include <time.h>

class thread_elem
{
public:

thread_elem();
thread_elem(void *fb,string command,string options,string *src=NULL, string dst="");


FXMutex mutex;
void *gui;
void *fb;
void *filel;

string command;
string *src;
string dst;
string options;
int p[2];
time_t * tim;

string str1;
string str2;
string str3;

unsigned long total_size;
unsigned long act_total_size;
	
unsigned long file_size;	
unsigned long act_file_size;

string act_file_name;

bool cancel;
bool end;
bool question;
bool answer;
bool overwrite;
bool all;
pthread_t pid;
FXMenuPane *pane;

};

#endif
