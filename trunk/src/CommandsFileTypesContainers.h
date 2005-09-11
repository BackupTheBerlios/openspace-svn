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
    string color;
    string backcolor;
    filetype_container(string name,string command);
    filetype_container(){}
    void save(void);
    bool load(string name);
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
