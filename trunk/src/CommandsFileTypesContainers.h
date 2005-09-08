#ifndef CFTC_H
#define CFTC_H

#include <vector>
#include <string>
#include <map>
using namespace std;



class filetype_container
{
  public:
    string name;
    string command;
    string icon;
    vector <string> commands;
    filetype_container(string name,string command);
    filetype_container(){}
    void save(void);
};

class command_container
{
  public:
    string name;
    string exec;
    bool rescan;
    bool capture;
    string text;
    string type;
    string icon;
    command_container(string name,string exec);
    command_container(){}
    void save(void);
    

};

#endif
